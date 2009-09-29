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

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <SDL/SDL.h>

#include "../config_platform.h"

#include <helpers/helpers.h>
#include <helpers/log.h>
#include "../../../base/syscall.h"
#include "../../../base/FileStream.h"
#include "../fastevents.h"
#include "../sdl_syscall.h"

extern "C" int MAMain();

int main(int argc, char** argv) {
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#ifdef LOGGING_ENABLED
	InitLog();
#endif

	const char *resourceFile = "resources";
	bool resChanged = false;
	Syscall::STARTUP_SETTINGS settings;
	int width, height;

	settings.model = "default";
	settings.vendor = "default";

	settings.iconPath = NULL;

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-resource")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resource");			
				return 1;
			}
			resourceFile = argv[i];
		}
		else if(strcmp(argv[i], "-size")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -size");			
				return 1;
			}
			width = atoi(argv[i]);
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -size");			
				return 1;
			}
			height = atoi(argv[i]);
			resChanged = true;
		} else if(strcmp(argv[i], "-noscreen")==0) {
			settings.showScreen = false;
		} else if(strcmp(argv[i], "-nomophone")==0) {
			settings.shouldHaveMophone = false;
		} else if(strcmp(argv[i], "-resmem")==0) {
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -resmem");			
				return 1;
			}
			settings.resmem = atoi(argv[i]);
		} else {
			LOG("unknown parameter: \"%s\"\n", argv[i]);
			return 1;
		}
	}
        
#ifdef __USE_FULLSCREEN__
        settings.shouldHaveMophone = false;
#endif

	Base::Syscall *syscall;
	if(!resChanged)
		syscall = new Base::Syscall(settings);
	else
		syscall = new Base::Syscall(width, height, settings);

	Base::FileStream file(resourceFile);
	TEST(syscall->loadResources(file, resourceFile));

	int i = MAMain();
	LOG("MAMain() returned %i\n", i);
	MoSyncExit(i);
}

void* Base::Syscall::GetValidatedMemRange(int address, int size) {
	return (void*)address;
}
void Base::Syscall::ValidateMemRange(const void* ptr, int size) {
}
int Base::Syscall::ValidatedStrLen(const char* ptr) {
	return strlen(ptr);
}
const char* Base::Syscall::GetValidatedStr(int address) {
	return (const char*)address;
}

void Base::Syscall::VM_Yield() {
}

SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
}

void MoSyncError::addRuntimeSpecificPanicInfo(char* ptr, bool newLines) {
}

void reportIp(int r, const char* message) {
}

void Base::reloadProgram() {
}

int Base::getRuntimeIp() {
	return -1;
}

void Base::reportCallStack() {
}

#ifdef MEMORY_PROTECTION
void Base::Syscall::protectMemory(int start, int length) {
}

void Base::Syscall::unprotectMemory(int start, int length) {
}

void Base::Syscall::setMemoryProtection(int enable) {
}

int Base::Syscall::getMemoryProtection() {
	return 0;
}
#endif
