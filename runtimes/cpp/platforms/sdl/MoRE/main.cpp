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

//--------------------------------------------------------------------//
// MoRE                                                               //
// MoSync Runtime Environment (SDL Version)                           //
// (c) Mobile Sorcery 2007                                            //
//--------------------------------------------------------------------//

#include "../config_platform.h"


#include <string.h>
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#include <core/Core.h>
#include <core/sld.h>
#include <base/Syscall.h>
#include <helpers/helpers.h>

#include "../sdl_syscall.h"
#include "../report.h"


#ifdef ENABLE_DEBUGGER
#include <core/debugger.h>
#endif
#ifdef GDB_DEBUG
#include <core/GdbStub.h>
#endif



static int gReloadHandle = 0;

struct ReloadException { ReloadException() {} };

static void DeleteCore() {
	Core::DeleteCore(gCore);
}

int main2(int argc, char **argv);

#ifndef _MSC_VER
#undef main
#endif



int main(int argc, char **argv) {
	int result = main2(argc, argv);
	reportClose();
	return result;
}

int main2(int argc, char **argv) {
	const char *programFile="program";
	const char *resourceFile="resources";
	bool resChanged = false;
	int width=0, height=0;
	Syscall::STARTUP_SETTINGS settings;
	settings.model = "default";
	settings.vendor = "default";
	settings.iconPath = NULL;
	const char* sldFile = NULL;
#ifdef GDB_DEBUG
	bool gdb = false;
#endif

	//NOTE: could have a -no-console option used by MoBuild, otherwise use a console for error output.
	//would be nice to detect whether launched from command line or from graphical shell.

	LOG("Hello World!\n");
	LOG("argc: %i\n", argc);

#if 0
	//int fd = _open("logg.txt", _O_TRUNC | _O_CREAT, _S_IWRITE);
	int fd = _fileno(fopen("logg.txt", "w"));
	_dup2(fd, 1);	//temp hack
	puts("Hello World!\n");
	_write(fd, "Hello World!\n", 10);
	_flushall();
#endif

	for(int i = 1; i < argc; i++) {
		LOG("%i:%s\n", i, argv[i]);
		if(strcmp(argv[i], "-id") == 0) {
			i++;
			settings.id = argv[i];
		}
		else if(strcmp(argv[i], "-fd") == 0) {
			i++;
			int writeFd = atoi(argv[i]);
			//_dup2(writeFd, 1);	//redirect stdout
			reportSetFd(writeFd);
			REPORT("Emulator connected.");
			//reportClose();
		}
		else if(strcmp(argv[i], "-program")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -program");			
				return 1;
			}
			programFile = argv[i];
		}

		else if(strcmp(argv[i], "-resource")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resource");			
				return 1;
			}
			resourceFile = argv[i];
		} else if(strcmp(argv[i], "-resolution")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resolution");			
				return 1;
			}
			width = atoi(argv[i]);
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resolution");			
				return 1;
			}
			height = atoi(argv[i]);
			resChanged = true;
		} else if(strcmp(argv[i], "-icon")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -icon");			
				return 1;
			}

			FILE *f;
			if((f=fopen(argv[i], "rb"))!=0) {
				fclose(f);
				settings.iconPath = argv[i];
			}
		} else if(strcmp(argv[i], "-noscreen")==0) {
			settings.showScreen = false;
		} else if(strcmp(argv[i], "-nomophone")==0) {
			settings.shouldHaveMophone = false;
		} else if(strcmp(argv[i], "-model")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -model");			
				return 1;
			}
			settings.model = argv[i];
		} else if(strcmp(argv[i], "-vendor")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -vendor");			
				return 1;
			}
			settings.vendor = argv[i];
		} else if(strcmp(argv[i], "-sld")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -sld");			
				return 1;
			}
			sldFile = argv[i];
		} else if(strcmp(argv[i], "-resmem")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resmem");			
				return 1;
			}
			settings.resmem = atoi(argv[i]);
#ifdef GDB_DEBUG
		} else if(strcmp(argv[i], "-gdb")==0) {
			gdb = true;
#endif
		} else {
			LOG("unknown parameter: \"%s\"\n", argv[i]);
			return 1;
		}
	}

#ifdef LOGGING_ENABLED
	InitLog();
#endif

#ifdef FAKE_CALL_STACK
	if(sldFile != NULL) {
		MYASSERT(loadSLD(sldFile), ERR_SLD_LOAD_FAILED);
	}
#endif

	Base::Syscall *syscall;

#ifdef __USE_FULLSCREEN__
        settings.shouldHaveMophone = false;
#endif

	if(!resChanged)
		syscall = new Base::Syscall(settings);
	else
		syscall = new Base::Syscall(width, height, settings);

	gCore = Core::CreateCore(*syscall);
#ifdef GDB_DEBUG
	gCore->mGdbOn = gdb;
#endif
	if(!Core::LoadVMApp(gCore, programFile, resourceFile)) {
		BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
	}

#ifdef ENABLE_DEBUGGER
	Core::initDebugger(gCore, 4711);
	atexit(Core::closeDebugger);
#endif
	atexit(DeleteCore);

	while(1) {
		try {
			Core::Run2(gCore);

			if(gReloadHandle > 0) {
#ifdef FAKE_CALL_STACK
				clearSLD();
#endif
				reportIp(0, "LoadProgram");
				report(REPORT_LOAD_PROGRAM);
				Base::Stream* stream = Base::gSyscall->resources.extract_RT_BINARY(gReloadHandle);
				bool res = Core::LoadVMApp(gCore, *stream);
				delete stream;
				gReloadHandle = 0;
				if(!res) {
					BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
					return 1;
				}	//if
			}	//if
		}	catch(ReloadException) {
			LOG("Caught ReloadException.\n");
			if(!Core::LoadVMApp(gCore, programFile, resourceFile)) {
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
				return 1;
			}
		}
	}
}

SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
	Base::gSyscall->VM_Yield(NULL);
	gReloadHandle = data;
	gReload |= (reload != 0);
}

void Base::reloadProgram() {
	report(REPORT_RELOAD);
	gReload = false;
	LOG("Throwing ReloadException.\n");
	throw ReloadException();
}

#ifdef TRANSLATE_PANICS
void MoSyncError::addRuntimeSpecificPanicInfo(char* ptr, bool newLines) {
	if(newLines)
		ptr += sprintf(ptr, "\n");
	else
		ptr += sprintf(ptr, " ");
	ptr += sprintf(ptr, "The panic occurred in the ");
	char buffer[32];
	const char* css = Core::CurrentSyscallString(gCore, buffer, sizeof(buffer));
	if(css) {
		ptr += sprintf(ptr, "syscall %s.", css);
	} else {
		ptr += sprintf(ptr, "MoSync core.");
	}
	if(newLines)
		ptr += sprintf(ptr, "\n");
}
#endif

#ifdef FAKE_CALL_STACK
void Base::reportCallStack() {
	if(gCore == NULL)
		return;
	report(REPORT_CALL_STACK, Core::GetFakeCallStack(gCore),
		Core::GetFakeCallStackDepth(gCore) << 2);
}
#endif

#if 0//def INSTRUCTION_PROFILING
void reportInstructionProfile() {
	if(gCore == NULL)
		return;
	report(REPORT_INSTRUCTION_PROFILE, Core::GetInstructionProfile(gCore),
		Core::GetInstructionProfileSize(gCore) << 2);
}
#endif

void reportIp(int r, const char* message) {
	if(gCore == NULL)
		return;

#ifdef FAKE_CALL_STACK
	reportCallStack();
#endif
#if 0//def INSTRUCTION_PROFILING
	reportInstructionProfile();
#endif

	char buffer[1024];
	int ip = Core::GetIp(gCore);
	int count = _snprintf(buffer, 1024, "%s @ IP 0x%x  SP 0x%x", message, ip,
		gCore->regs[Core::REG_sp]);
	LOG("%s\n", buffer);
	report(REPORT_STRING, buffer, count);
	report(REPORT_IP, &ip, sizeof(int));

#ifdef GDB_DEBUG
	if(gCore->mShared.mGdbOn) {
		gCore->mShared.mGdbStub->exitHandler(r);
		gCore->mShared.mGdbStub->waitForRemote();
	}
#endif
}

int Base::getRuntimeIp() {
	if(gCore)
		return Core::GetIp(gCore);
	return -1;
}

#if 0
extern "C" const char* FileNameFromPath(const char* path) {
	const char* ptr = strrchr(path, '\\');
	return ptr ? (ptr + 1) : path;
}

extern "C" void bfeHandler(int value, const char* filename, int line) {
	LOG("BFE %i %s:%i\n", value, filename, line);
	MoSyncErrorExit(value);
}
#endif
