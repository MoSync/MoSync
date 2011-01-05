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

#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "helpers/log.h"

#include "sld.h"
#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"
#include "globals.h"

#ifdef DARWIN
#include <spawn.h>
#include <unistd.h>
extern char **environ;
#endif


using namespace std;

void target_select(const string& args);

#ifdef WIN32
HANDLE gSimProcess;
#endif

namespace Callback {
	static void connected();
}

static void remote_connect(const vector<string>& argv);
static bool start_sim();

void target_select(const string& args) {
	//parse args
	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() < 1) {
		error("Need 1 argument");
		return;
	}
	if(argv[0] == "sim") {
		if(!start_sim())
			return;
		argv.resize(2);
		argv[1] = "localhost:50000";
	} else if(argv[0] != "remote") {
		error("Unsupported target type");
		return;
	}
	remote_connect(argv);
}

//requires that program, resources and sld.tab be specified on the command line.
//returns false on fail. in that case, error() will have been called.
static bool start_sim() {
	if(gProgramFilename.size() == 0 || gSldFilename.size() == 0) {
		error("no program or sld file specified");
		return false;
	}
#ifdef WIN32
	const char *mosyncdir = getenv("MOSYNCDIR");
	if(!mosyncdir) error("MOSYNCDIR environment variable unavailable");
	string cmdLine = "\"" + string(mosyncdir) + "/bin/more.exe\" -gdb -program \""+gProgramFilename+"\"";
	if(gResourceFilename.size() > 0)
		cmdLine += " -resource \""+gResourceFilename+"\"";
	cmdLine += " -sld \""+gSldFilename+"\"";
#ifdef _DEBUG
	cmdLine += " -resolution 320 240"; 
#endif

	LOG("%s\n", cmdLine.c_str());

	PROCESS_INFORMATION pi;
	char* ptr = new char[cmdLine.size() + 1];
	strcpy(ptr, cmdLine.c_str());
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = INVALID_HANDLE_VALUE;
	//we could redirect to a file, if we find it interesting.
	//this is just to avoid fouling up our own stdout.
	si.hStdOutput = INVALID_HANDLE_VALUE;
	si.hStdError = INVALID_HANDLE_VALUE;
	int res = CreateProcess(NULL, ptr, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	delete ptr;
	if(res == 0) {
		LOG("GLE %lu\n", GetLastError());
		error("CreateProcess failed");
		return false;
	}
	gSimProcess = pi.hProcess;
	Sleep(2000);	//hack
	
#elif defined(DARWIN)
	// Mac OS X specific code.
	// This code should work on linux too :)
	
	// Check for MOSYNCDIR environment variable
	const char *mosyncdir = getenv("MOSYNCDIR");
	if(!mosyncdir) error("MOSYNCDIR environment variable unavailable");
	
	// Check if there are resources or not,
	// because it changes the arguments to pass to MoRE
	if(gResourceFilename.size() > 0) {
		// Copy the file names from global to local vars
		char *programFilename = new char [gProgramFilename.length()];
		strcpy(programFilename, gProgramFilename.c_str());
		
		char *sldFilename = new char [gSldFilename.length()];
		strcpy(sldFilename, gSldFilename.c_str());
		
		char *resourceFilename = new char [gResourceFilename.length()];
		strcpy(resourceFilename, gResourceFilename.c_str());
		
		// Form the arguments to pass to MoRE
		char * const argv_with_res[] = { (char*)"-gdb", (char*)"-program", 
			programFilename, (char*)"-resource", resourceFilename, (char*)"-sld", sldFilename };
		
		delete(programFilename);
		delete(sldFilename);
		delete(resourceFilename);
		
		// Launch MoRE in a new process
		// using the appropriate arguments
		if ( posix_spawnp(NULL, "moemu", NULL, NULL, argv_with_res, environ ) ) {
			error("CreateProcess failed");
			return false;
		}
	}
	else {
		// Copy the file names from global to local vars
		char *programFilename = new char [gProgramFilename.length()];
		strcpy(programFilename, gProgramFilename.c_str());
		
		char *sldFilename = new char [gSldFilename.length()];
		strcpy(sldFilename, gSldFilename.c_str());
		
		// Form the arguments to pass to MoRE
		char * const argv_no_res[] = { (char*)"-gdb", (char*)"-program", 
			programFilename, (char*)"-sld", sldFilename };
		
		delete(programFilename);
		delete(sldFilename);
		
		// Launch MoRE in a new process
		// using the appropriate arguments
		if ( posix_spawnp(NULL, "moemu", NULL, NULL, argv_no_res, environ ) ) {
			error("CreateProcess failed");
			return false;
		}
	}
	
#else
	error("unsupported platform");
	return false;
#endif
	return true;
}

static void remote_connect(const vector<string>& argv) {
	if(argv.size() != 2) {
		error("Need 2 arguments");
		return;
	}
	const char* params = argv[1].c_str();
	const char* firstColon = strchr(params, ':');
	const char* portStr;
	string host;
	if(firstColon == NULL) {
		error("Syntax error");
		return;
	} else if(firstColon == params) {
		portStr = firstColon + 1;
		host = "localhost";
		if(strchr(portStr, ':') != NULL) {
			error("Syntax error");
			return;
		}
	} else {
		const char* secondPart = firstColon + 1;
		const char* secondColon = strchr(secondPart, ':');
		if(secondColon != NULL) {
			portStr = secondColon;
			const char* thirdColon = strchr(secondColon + 1, ':');
			if(thirdColon != NULL) {
				error("Syntax error");
				return;
			}
			host = string(secondPart, secondColon - secondPart);
			string protocol(params, firstColon - params);
			if(protocol != "tcp") {
				error("Unsupported protocol");
				return;
			}
		} else {
			portStr = firstColon;
			host = string(params, firstColon - params);
		}
	}

	//connect, asynchronously
	StubConnection::connect(host, atoi(portStr + 1), Callback::connected);
}

static void Callback::connected() {
	//get address from stub
	ASSERT_REG;

	const char* fname = mapFunction(r.pc);
	oprintToken();
	oprintf("^connected,addr=\"0x%08x\",func=\"%s\",args=[]\n", r.pc, fname ? fname : "??");
	commandComplete();
}
