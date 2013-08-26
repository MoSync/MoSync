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

#ifndef _MOSYNC_INVOKE_EXTENSION_H_
#define _MOSYNC_INVOKE_EXTENSION_H_

#include "extension.h"
#include "maapi_defs.h"
#include "cpp_defs.h"
#include "CoreCommon.h"

#define LOGSC(...)

#define CALL_SYSCALL(func) func

#define DLLEXPORT __declspec(dllexport)
extern "C"
void DLLEXPORT initializeExtension(ExtensionData* ed, const CoreData* cd);

// requirements for syscall_arguments.h
#define mem_ds gMemDs
#define SAFUNC __inline
#define SCDEBUG_ARG(a)
void ValidateMemRange(const void*, unsigned);
void ValidateMemStringAddress(int address);
void ValidateMemWStringAddress(int address);

#define _MOSYNC_SYSCALL_ARGUMENTS_H_
#include "syscall_arguments.h"

#endif	//_MOSYNC_INVOKE_EXTENSION_H_
