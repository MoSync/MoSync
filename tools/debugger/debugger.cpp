/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#ifdef _MSC_VER
#include <io.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include "config.h"
#include "helpers/helpers.h"

#include "bluetooth/btinit.h"
#include "sld.h"
#include "stabs/stabs.h"

#include "command.h"
#include "Thread.h"
#include "async.h"
#include "userInputThread.h"
#include "remoteReadThread.h"
#include "StubConnLow.h"
#include "helpers.h"
#include "globals.h"
#include "commandInterface.h"
#include "StubConnection.h"
#include "cmd_stack.h"

using namespace std;

/**
 * General debugger architecture
 *
 * The MoSync debugger consists of 3 communicating components: The Eclipse 
 * GDB session, the MoSync Debug Bridge (this source file) and a MoRE instance. 
 * An overview of their communication is given in the picture below.
 *
 * ---------------           ----------------           -----------------
 * | GDB session |  <------  | MoSync Debug |  <------  | MoRE instance |
 * |             |           | Bridge (mdb) |           |               |
 * |             |  ------>  |              |  ------>  |               |
 * ---------------           ----------------           -----------------
 *               GDB/MI command            internal format
 *
 * A command originates in the Eclipse GDB Session (referred to as the GDB 
 * session from here) when a user instructs the debugger to do something. The
 * commands are actually part of a protocol called the GDB/MI interface, which 
 * defines the behavior and structure of the commands. The commands arrive
 * at mdb through its standard input stream.
 *
 * Once in mdb the command is passed to a handler function that parses
 * the command and executes it. The mapping between a handler and its command
 * is defined in initCommands.cpp, note that not all of the GDB/MI commands
 * are implemented. The handlers are then implemented in the cmd_* files which
 * each group a set of related handlers.
 *
 * Some commands require communication with the MoRE instance that runs the 
 * program being debugged. For example some commands needs the value at
 * a specific memory address. mdb accomplishes such and similar tasks by 
 * communicating with MoRE over TCP with an internal protocol.
 *
 * Another class of commands are those that controls the execution of the 
 * program, e.g. breakpoints and stepping. A breakpoint can be set at a 
 * particular address in the code memory by replacing the instruction at that
 * address with a special debug instruction that halts the execution. The 
 * execution can then be resumed by replacing it with the original instruction.
 *
 * Once a command has been fully executed, the results are passed back to 
 * Eclispe via the stdout stream. New commands can then be processed.
 *
 * Notes
 * It should be noted that eclipse sometimes diverts from the GDB/MI interface,
 * so care should be taken.
 */

/* Filename, resources and slds specified as parameters to mdb */
string gProgramFilename, gResourceFilename, gSldFilename;

bool gTestWaiting;

/* Generic info about the program */
MA_HEAD gHead;

/* Buffer that contains the code section */
byte* gMemCs = NULL;

/* Buffer that contains the offset constants (the var pool) */
int* gMemCp = NULL;

//extern char* sMemBuf;	//TODO: fixme

static string sToken;

/**
 * Executes a command on GDB/MI format. The command must be completed with
 * the commandComplete function, so that another function can be processed.
 *
 * @param line String that contains a GDB/MI command.
 */
static void executeCommand(const string& line);

/**
 * Reads a MoSync program file into memory.
 * 
 * Side effects:
 * Results are placed in gHead, gMemCs and gMemCp.
 *
 * @param filename path to the file to open.
 * @return true if the program file could be read, false otherwise.
 */
static bool readProgramFile(const char* filename);

/**
 * Prints the given fmt string to stderr and logs
 * it if logging is activated.
 *
 * @param fmt printf style format string.
 */
int eprintf(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
#ifdef LOGGING_ENABLED
	LogV(fmt, argptr);
#endif
	return vfprintf(stderr, fmt, argptr);
}

/**
 * Prints the given fmt string to stdout and logs
 * it if logging is activated.
 *
 * @param fmt printf style format string.
 */
int oprintf(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
#ifdef LOGGING_ENABLED
	LogV(fmt, argptr);
#endif
	return vprintf(fmt, argptr);
}

/**
 * Prints the given character to stdout and logs
 * it if logging is activated.
 *
 * @param c character to print.
 */
void oputc(int c) {
#ifdef LOGGING_ENABLED
	LogBin(&c, 1);
#endif
	putc(c, stdout);
}

#ifdef _MSC_VER
#ifdef _DEBUG
#define COMMAND_LOGGING_ENABLED
#endif
#endif

#ifdef COMMAND_LOGGING_ENABLED
FILE *gCommandLog;
#endif

static const char* sUsageString =
"usage: mdb [options]\n"
"\n"
"The MoSync Debugger exposes a subset of the GDB/MI interface.\n"
"Commands are read from stdin, responses are written to stdout.\n"
"\n"
"It can connect to a remote stub, or start a simulator and connect to that.\n"
"If you're going to use a simulator, you'll need to provide program,\n"
"resource (if the program has any) and SLD files to load.\n"
"\n"
"Options:\n"
"\n"
"-p <program file>\n"
"Loads the specified program file. This argument must always be specified.\n"
"\n"
"-r <resource file>\n"
"Loads the specified resource file, if the simulator target is chosen.\n"
"\n"
"-sld <sld file>\n"
"Loads the specified SLD file.\n"
"Can be specified with the -file-exec-and-symbols command instead of on the\n"
"command line.\n"
"\n"
"An SLD file contains MoSync debugging information.\n"
"It is generated by pipe-tool.\n"
"\n"
"-stabs <stabs file>\n"
"Loads the specified Stabs file.\n"
"\n"
"A Stabs file contains extra debugging information.\n"
"It is generated by pipe-tool.\n"
"\n"
"-s <script file>\n"
"Executes the mdb commands in the script file before reading from stdin.\n"
"\n"
;

#undef main
int main(int argc, char** argv) {
#ifdef LOGGING_ENABLED
	InitLog("mdb_log.txt");
#endif
#ifdef COMMAND_LOGGING_ENABLED
	gCommandLog = fopen("mdb_commands.txt", "w");
	if(!gCommandLog) {
		eprintf("Could not open command logging output file.\n");
		return 1;
	}

	for(int i = 0; i < argc; i++) {
		fprintf(gCommandLog, "%s", argv[i]);
		if(i!=argc-1) fprintf(gCommandLog, " ");
	}
	fprintf(gCommandLog, "\n");

#endif
	for(int i=0; i<argc; i++) {
		LOG("%s ", argv[i]);
	}
	LOG("\n");

	istream* input = &cin;
	const char* stabsFilename = NULL;

	//parse arguments
	int i = 1;
	while(i < argc) {
		if(strcmp(argv[i], "-s") == 0) {
			i++;
			if(i > argc) {
				eprintf("%s", sUsageString);
				return 1;
			}
			input = new ifstream(argv[i]);
			if(!input->good()) {
				eprintf("Could not open script file '%s'\n", argv[2]);
				return 1;
			}
		} else if(strcmp(argv[i], "-p") == 0) {
			i++;
			if(i > argc) {
				eprintf("%s", sUsageString);
				return 1;
			}
			gProgramFilename = argv[i];
		} else if(strcmp(argv[i], "-r") == 0) {
			i++;
			if(i > argc) {
				eprintf("%s", sUsageString);
				return 1;
			}
			gResourceFilename = argv[i];
		} else if(strcmp(argv[i], "-sld") == 0) {
			i++;
			if(i > argc) {
				eprintf("%s", sUsageString);
				return 1;
			}
			gSldFilename = argv[i];
		} else if(strcmp(argv[i], "-stabs") == 0) {
			i++;
			if(i > argc) {
				eprintf("%s", sUsageString);
				return 1;
			}
			stabsFilename = argv[i];
		} else {
			eprintf("%s", sUsageString);
			return 1;
		}
		i++;
	}

	if(gProgramFilename.size() > 0) {
		if(!readProgramFile(gProgramFilename.c_str())) {
			eprintf("Could not load program file '%s'\n", gProgramFilename.c_str());
			return 1;
		}
	} else {
		eprintf("Must specify a program file!\n");
		return 1;
	}

#define STABS_MSG ". This is probably because you linked with non-debug libraries."\
	" Check your settings."
	if(stabsFilename) {
		//load stabs and SLD
		if(!loadStabs(gSldFilename.c_str(), stabsFilename)) {
			eprintf("Could not load Stabs file '%s'"STABS_MSG"\n", stabsFilename);
			return 1;
		}
	} else if(gSldFilename.size() > 0) {
		//if no stabs, load only SLD.
		if(!loadSLD(gSldFilename.c_str())) {
			eprintf("Could not load SLD file '%s'"STABS_MSG"\n", gSldFilename.c_str());
			return 1;
		}
	}

	//initialize subsystems
	Bluetooth::MABtInit();
	initCommands();
	initEventSystem();

	StubConnection::addContinueListener(stackContinued);

	MoSyncThread sRemoteReadThread, sUserInputThread;
	sUserInputThread.start(userInputThreadFunc, input);
	sRemoteReadThread.start(remoteReadThreadFunc, NULL);

	oprintf(GDB_PROMPT);	//undocumented startup prompt, expected by ccdebug.
	fflush(stdout);

	//main loop
	LOG("Starting main loop...\n");
	string savedLine;
	while(1) {
		DebuggerEvent* de;
		getEvent(&de);
		//LOG("Event %i\n", de->type);
		switch(de->type) {
		case DebuggerEvent::eRecv:
			StubConnLow::recvHandler(de->result);
			break;
		case DebuggerEvent::eUserInput:
			if(StubConnection::isIdle())
				executeCommand(de->str);
			else {
				_ASSERT(savedLine.empty());
				savedLine = de->str;
			}
			break;
		case DebuggerEvent::eReadMemory:
			StubConnection::readMemory(gMemBuf + de->src, de->src, de->len, de->rmcb);
			break;
		case DebuggerEvent::eLocateSymbol:
			locate_symbol(de->str, de->lscb);
			break;
		case DebuggerEvent::eExpressionEvaluated:
			setErrorCallback(NULL);
			de->ecb(de->erv, de->err);
			break;
		default:
			_ASSERT(false);
		}
		delete de;
		if(StubConnection::isIdle() && savedLine.size() > 0) {
			executeCommand(savedLine);
			savedLine.clear();
		}
		if(gTestWaiting && 
			!execIsRunning() &&
			!StubConnection::isRunning())
		{
			gTestWaiting = false;
			oprintDone();
			oprintf(",test-wait\n");
			commandComplete();
		}
	}
}

void oprintToken() {
	oprintf("%s", sToken.c_str());
}
void oprintDone() {
	oprintf("%s^done", sToken.c_str());
}
void oprintDoneLn() {
	oprintf("%s^done\n", sToken.c_str());
}

static ErrorCallback sErrorCallback = NULL;
void setErrorCallback(ErrorCallback ecb) {
	sErrorCallback = ecb;
}

/**
 * Sends a GDB/MI error message with the given
 * message string.
 *
 * @param fmt printf style format string.
 */
void error(const char* fmt, ...) {
	if(sErrorCallback) sErrorCallback();
	varErrorFunction(); // this is a bit ugly, but necessary. (sets sVar to NULL etc.)

	va_list argptr;
	va_start(argptr, fmt);
	oprintf("%s^error,msg=\"", sToken.c_str());
#ifdef LOGGING_ENABLED
	LogV(fmt, argptr);
#endif
	vprintf(fmt, argptr);
	oprintf("\"\n");
	commandComplete();
}

/**
 * Indicates whether we are currently executing a command.
 */
static bool sExecutingCommand = false;

void commandComplete() {
	_ASSERT(sExecutingCommand);
	sExecutingCommand = false;

	oprintf(GDB_PROMPT);
	fflush(stdout);

	//user input is automatically paused after one line is read,
	//so now that we've processed it, we can resume.
	resumeUserInput();
}

/**
 * Executes the given GDB/MI command.
 *
 * If no suitable command handler is found an error message is sent to the GDB
 * session.
 */
static void executeCommand(const string& line) {
	LOG("Command: %s\n", line.c_str());
	_ASSERT(!sExecutingCommand);
	sExecutingCommand = true;

#ifdef COMMAND_LOGGING_ENABLED
	fprintf(gCommandLog, "%s\n", line.c_str());
	fflush(gCommandLog);
#endif

	size_t offset = 0;
	if(isdigit(line[0])) {	//we've got a token
		//special handling for Eclipse' broken use of "whatis"
		size_t dash = line.find('-');
		size_t space = line.find(' ');
		if(dash == string::npos && space == string::npos) {
			error("Command syntax error");
			return;
		}
		offset = MIN(dash, space);
		sToken = line.substr(0, offset);
	} else {
		sToken.clear();
	}

	size_t firstSpaceIndex = line.find_first_of(' ', offset + 1);
	string command = line.substr(offset, firstSpaceIndex - offset);
	CommandIterator itr = sCommands.find(command);
	if(itr != sCommands.end()) {
		Command cmd = itr->second;
		int argIndex = line.find_first_not_of(' ', firstSpaceIndex);
		string args = argIndex > 0 ? line.substr(argIndex) : "";
		cmd(args);
	} else {
		error("Undefined MI command: '%s'", line.c_str());
	}
}

void MoSyncErrorExit(int code) {
	error("MoSyncErrorExit(%i)", code);
	exit(code);
}

/**
 * Reads len bytes from the file descriptor and
 * returns true if len bytes could be read.
 *
 * @param fd Points to file to read from.
 * @param dst Buffer to read into.
 * @param len Number of bytes to read.
 * @return true if len bytes could be read, false otherwise.
 */
static bool readAll(int fd, void* dst, int len) {
	char* p = (char*)dst;
	int pos = 0;
	while(pos < len) {
		int remain = len - pos;
		int res = read(fd, p, remain);
		if(res <= 0) {
			LOG("read error: %i, %i\n", res, errno);
			FAIL;
		}
		pos += res;
	}
	return true;
}


/**
 * Reads an opened program file.
 *
 * @param valid file descriptor.
 * @return True if the file could be read, false otherwise.
 */
static bool readOpenProgramFile(int fd) {
	MA_HEAD& h(gHead);
	TEST(readAll(fd, &h, sizeof(h)));

	if(h.Magic != 0x5844414d) {	//MADX, big-endian
		LOG("Magic error: 0x%08x should be 0x5844414d\n", h.Magic);
		FAIL;
	}

	//ok, we read it. now apply...
	setMemSize(h.DataSize);

	gMemCs = new byte[h.CodeLen];
	TEST(readAll(fd, gMemCs, h.CodeLen));

	if(lseek(fd, h.DataLen, SEEK_CUR) < 0) {
		FAIL;
	}

	gMemCp = new int[h.IntLen];
	TEST(readAll(fd, gMemCp, h.IntLen * 4));
	return true;
}

/**
 * Reads a program file with the given file name and
 * reads it to memory. The contents are stored in a 
 * global structure.
 *
 * Side effects:
 * Modifies gHead, gMemCs and gMemCp.
 *
 * @param filename The program file to be read.
 * @return true if the file could be read, false otherwise.
 */
static bool readProgramFile(const char* filename) {
	int fd = open(filename, O_RDONLY
#ifdef WIN32
		| O_BINARY
#endif
		);
	FAILIF(fd < 0);

	bool res = readOpenProgramFile(fd);

	close(fd);

	return res;
}
