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
// (c) MoSync AB 2010                                                 //
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
#include <core/extensions.h>
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

#include "Skinning/SkinManager.h"
#include "Skinning/GenericSkin.h"

static int gReloadHandle = 0;

struct ReloadException { ReloadException() {} };

static void DeleteCore() {
	Core::DeleteCore(gCore);
}

int main2(int argc, char **argv);

#if defined(WIN32) && !defined(_MSC_VER)
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
	/*
     * Deprecated ?
	int width=0, height=0;
	*/

	MoRE::SkinManager::getInstance()->addSkinFactory(new MoRE::GenericSkinFactory());

	Syscall::STARTUP_SETTINGS settings;
	settings.profile.mScreenWidth = 240;
	settings.profile.mScreenHeight = 320;
	settings.profile.mKeyboardType = MoRE::DeviceProfile::DKT_KEYPAD;
	settings.profile.mModel = "default";
	settings.profile.mVendor = "default";
	settings.haveSkin = true;
	settings.iconPath = NULL;
#ifdef EMULATOR
	settings.timeout = 0;
#endif
	const char* sldFile = NULL;
	const char* xFile = NULL;
#ifdef GDB_DEBUG
	bool gdb = false;
#endif
#ifdef EMULATOR
	bool allowDivZero = false;
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
		if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
			static const char sInfo[] = 
				"MoSync Runtime Environment (MoRE) (c) MoSync AB 2010\n"
				"\n"
				"  Options (Optional):\n"
				"\n"
				"  -id <integer>                          instance id.\n"
				"  -fd <integer>                          file descriptor to which certain binary data is written. used with IDEs.\n"
				"  -program <filename:string>             program file to be executed (default: 'program' in cwd).\n"
				"  -resource <filename:string>            resource file to be loaded (default: 'resources' in cwd).\n"
				"  -resolution <x:integer> <y:integer>    resolution to use (defaults to 240 x 320).\n"
				"  -icon <filename:string>                icon to use, to identify the instance.\n"
				"  -noscreen                              don't open a display window. useful for automated tests from a console script.\n"
				"  -nomophone                             run emulator without skin, just show the screen.\n"
				"  -vendor <string>                       set vendor. Used to choose skin.\n"
				"  -model <string>                        set model. Used to choose skin.\n"
				"  -sld <filename:string>                 load sld-file.\n"
				"  -resmem <bytes:integer>                set resource memory limit.\n"
				"  -gdb                                   start gdb stub.\n"
				"  -x <filename:string>                   load extension config file.\n"
#ifdef EMULATOR
				"  -allowdivzero                          allow floating-point division by zero. this produces ieee standard results.\n"
				"  -timeout <seconds:integer>             close the program if it runs longer than the timeout.\n"
#endif
				"\n";
			printf("%s", sInfo);
			return 1;

		}
		else if(strcmp(argv[i], "-id") == 0) {
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
			settings.profile.mScreenWidth = atoi(argv[i]);
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resolution");			
				return 1;
			}
			settings.profile.mScreenHeight = atoi(argv[i]);
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
				settings.haveSkin = false;
		} else if(strcmp(argv[i], "-model")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -model");			
				return 1;
			}
			settings.profile.mModel = argv[i];
		} else if(strcmp(argv[i], "-vendor")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -vendor");			
				return 1;
			}
			settings.profile.mVendor = argv[i];
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
		} else if(strcmp(argv[i], "-x")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -x");			
				return 1;
			}
			xFile = argv[i];
#ifdef GDB_DEBUG
		} else if(strcmp(argv[i], "-gdb")==0) {
			gdb = true;
#endif
#ifdef EMULATOR
		} else if(strcmp(argv[i], "-allowdivzero")==0) {
			allowDivZero = true;
		} else if(strcmp(argv[i], "-timeout")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -timeout");			
				return 1;
			}
			settings.timeout = atoi(argv[i]);
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
        settings.haveSkin = false;
#endif

	if(!resChanged)
		syscall = new Base::Syscall(settings);
	else
		syscall = new Base::Syscall(settings.profile.mScreenWidth, settings.profile.mScreenHeight, settings);

	gCore = Core::CreateCore(*syscall);
#ifdef GDB_DEBUG
	gCore->mGdbOn = gdb;
#endif
#ifdef EMULATOR
	syscall->mAllowDivZero = allowDivZero;
#endif
	if(!Core::LoadVMApp(gCore, programFile, resourceFile)) {
		BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
	}

	if(xFile) {
		loadExtensions(xFile);
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
				delete gCore;
				gCore = Core::CreateCore(*syscall);
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
			delete gCore;
			gCore = Core::CreateCore(*syscall);
			if(!Core::LoadVMApp(gCore, programFile, resourceFile)) {
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
				return 1;
			}
		}
	}
}

SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
	Base::gSyscall->VM_Yield();
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
	maDumpCallStackEx("reportCallStack", 0);
	report(REPORT_CALL_STACK, Core::GetFakeCallStack(gCore),
		Core::GetFakeCallStackDepth(gCore) << 2);
}

int Base::maDumpCallStackEx(const char* str, int data) {
	if(gCore == NULL)
		return -2;
	LOG("DumpCallStackEx:\n%s\n%i\n", str, data);
	LOG("IP: 0x%x\n", Core::GetIp(gCore));
	int depth = Core::GetFakeCallStackDepth(gCore);
	LOG("Call stack: %i frames\n", depth);
	const int* fcs = Core::GetFakeCallStack(gCore);
	for(int i=0; i<depth; i++) {
		//todo: translate
		int ip = fcs[i];
		int line;
		std::string file;
		bool res = mapIp(ip, line, file);
		LOG("%i: 0x%x", i, ip);
		if(res) {
			LOG("(%s:%i)", file.c_str(), line);
		}
		LOG("\n");
	}
	return 0;
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
	if(gCore->mGdbOn) {
		gCore->mGdbStub->exitHandler(r);
		gCore->mGdbStub->waitForRemote();
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
