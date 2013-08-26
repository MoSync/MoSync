/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "../config_platform.h"

#include <helpers/helpers.h>
#include <helpers/attribute.h>
#include <helpers/log.h>
#include "../../../base/Syscall.h"
#include "../../../base/FileStream.h"
#include "../fastevents.h"
#include "../sdl_syscall.h"
#include "../report.h"
#include "mosynclibmain.h"

extern "C" int mosyncLibMain(int argc, char** argv, mainfunc maMain) {
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#ifdef LOGGING_ENABLED
	InitLog();
#endif

	const char *resourceFile = "resources";
	bool resChanged = false;
	Syscall::STARTUP_SETTINGS settings;
	settings.profile.mScreenWidth = 240;
	settings.profile.mScreenHeight = 320;
	settings.profile.mKeyboardType = MoRE::DeviceProfile::DKT_KEYPAD;
	settings.profile.mVendor = "default";
	settings.profile.mModel = "default";
	settings.haveSkin = true;


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
			settings.profile.mScreenWidth = atoi(argv[i]);
			i++;
			if(i>=argc) {
				LOG("not enough parameters for -size");			
				return 1;
			}
			settings.profile.mScreenHeight = atoi(argv[i]);
			resChanged = true;
		} else if(strcmp(argv[i], "-noscreen")==0) {
			settings.showScreen = false;
		} else if(strcmp(argv[i], "-nomophone")==0) {
			settings.haveSkin = false;
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

	Base::Syscall *syscall;
	if(!resChanged)
		syscall = new Base::Syscall(settings);
	else
		syscall = new Base::Syscall(settings.profile.mScreenWidth, settings.profile.mScreenHeight, settings);

	Base::FileStream file(resourceFile);
	TEST(syscall->loadResources(file, resourceFile));

	int i = maMain();
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
int Base::Syscall::GetValidatedStackValue(int offset) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);	
}
const char* Base::Syscall::GetValidatedStr(int address) {
	return (const char*)address;
}

void Base::Syscall::VM_Yield() {
}

extern "C" void GCCATTRIB(noreturn) maLoadProgram(MAHandle data, int reload) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
}

void MoSyncError::addRuntimeSpecificPanicInfo(char* ptr, bool newLines) {
}

void reportIp(int r, const char* message) {
}

void Base::reloadProgram() {
	exit(1);
}

int Base::getRuntimeIp() {
	return -1;
}

void Base::reportCallStack() {
}

int Base::maDumpCallStackEx(const char*, int) {
	return -1;
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
