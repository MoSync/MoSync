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

#ifndef _MOSYNC_EXTENSION_COMMON_H_
#define _MOSYNC_EXTENSION_COMMON_H_

// Each function is void(void), but has access to registers and memory.
// No function needs to know its number.
typedef void (*VoidFunction)(void);

struct ExtensionData {
	int idlHash;
	unsigned int nFunctions;
	VoidFunction* functions;
};
struct CoreData {
	int* regs;
	void* memDs;
};

// Each Extension has this function.
// Don't mix 32-bit and 64-bit code. (Should be impossible on most systems.)
typedef void (*InitializeExtension)(ExtensionData*, const CoreData*);

#endif	//_MOSYNC_EXTENSION_COMMON_H_
