#ifndef _MOSYNC_INVOKE_EXTENSION_H_
#define _MOSYNC_INVOKE_EXTENSION_H_

#include "extension.h"
#include "maapi_defs.h"
#include "cpp_defs.h"
#include "CoreCommon.h"

#define LOGSC(...)

#define CALL_SYSCALL(func) func

#define DLLEXPORT __declspec(dllexport)
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
