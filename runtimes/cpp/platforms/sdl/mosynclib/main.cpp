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
#include "Skinning/SkinManager.h"
#include "Skinning/GenericSkin.h"

#ifdef _MSC_VER
extern "C" int MAMain();
int main(int argc, char** argv) {
#else
#include "mosynclibmain.h"
extern "C" int mosyncLibMain(int argc, char** argv, mainfunc MAMain) {
#endif

#ifdef _MSC_VER
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#ifdef LOGGING_ENABLED
	InitLog();
#endif

	MoRE::SkinManager::getInstance()->addSkinFactory(new MoRE::GenericSkinFactory());

	const char *resourceFile = "resources";
	bool resChanged = false;
	Syscall::STARTUP_SETTINGS settings;

	settings.profile.mScreenWidth = 240;
	settings.profile.mScreenHeight = 320;
	settings.profile.mKeyboardType = MoRE::DeviceProfile::DKT_KEYPAD;
	settings.profile.mVendor = "default";
	settings.profile.mModel = "default";
	settings.haveSkin = true;
	settings.iconPath = NULL;
#ifdef EMULATOR
	settings.timeout = 0;
#endif

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
			settings.haveSkin = false;
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

#ifdef __USE_FULLSCREEN__
        settings.haveSkin = false;
#endif

	Base::Syscall *syscall;
	if(!resChanged)
		syscall = new Base::Syscall(settings);
	else
		syscall = new Base::Syscall(settings.profile.mScreenWidth, settings.profile.mScreenHeight, settings);

	Base::FileStream file(resourceFile);
	TEST(syscall->loadResources(file, resourceFile));

	int i = MAMain();
	LOG("MAMain() returned %i\n", i);
	MoSyncExit(i);
}

void* Base::Syscall::GetValidatedMemRange(int address, int size) {
	DEBUG_ASSERT(sizeof(void*) == sizeof(int));
	return (void*)(size_t)address;
}
void Base::Syscall::ValidateMemRange(const void* ptr, int size) {
}
int Base::Syscall::ValidatedStrLen(const char* ptr) {
	return strlen(ptr);
}
int Base::Syscall::GetValidatedStackValue(int offset, va_list argptr) {
	DEBUG_ASSERT((offset % 4) == 0);
	for(int i=0; i<offset; i+=4) {
		va_arg(argptr, int);
	}
	return va_arg(argptr, int);
}
const char* Base::Syscall::GetValidatedStr(int address) {
	DEBUG_ASSERT(sizeof(void*) == sizeof(int));
	return (const char*)(size_t)address;
}
const wchar* Base::Syscall::GetValidatedWStr(int address) {
	DEBUG_ASSERT(sizeof(void*) == sizeof(int));
	return (const wchar*)(size_t)address;
}

void Base::Syscall::VM_Yield() {
}

#ifdef _MSC_VER
SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
#else
extern "C" void GCCATTRIB(noreturn) maLoadProgram(MAHandle data, int reload) {
#endif
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
}

SYSCALL(longlong, maExtensionFunctionInvoke(int function, int a, int b, int c)) {
	return -1;
}

void MoSyncError::addRuntimeSpecificPanicInfo(char* ptr, bool newLines) {
}

void reportIp(int r, const char* message) {
}

void Base::reloadProgram() {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
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
