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

#include "TranslateSyscall.h"

extern "C" const char* translateSyscall(int num) {
#include "asm_config.h"
#define SYSCALL_NAME(number,reType,name,arg1,argD) ,#name
	static const char* const syscallStrings[] = { 0 SYSCALLS(SYSCALL_NAME, , , ) };
	static const int nSyscalls = (sizeof(syscallStrings) / sizeof(char*)) - 1;
	if(num > 0 && num <= nSyscalls) {
		return syscallStrings[num];
	} else {
		return 0;
	}
}
