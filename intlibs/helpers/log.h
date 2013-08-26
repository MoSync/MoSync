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

#ifndef LOG_H
#define LOG_H

#ifndef CONFIG_H
#error Config file required!
#endif

#ifdef __SYMBIAN32__
#include <e32def.h>
#define SYMBIAN 1
#elif defined(_android)
#include <android/log.h>
#else
#include <stdarg.h>
#endif

#ifndef __SYMBIAN32__
#define VA_LIST va_list
#define VA_START va_start
#endif

#include "attribute.h"

//******************************************************************************
//Function declarations that ought to be portable
//******************************************************************************

#ifdef LOGGING_ENABLED
#ifndef NULL
#define NULL 0
#endif
void InitLog(const char* filenameOverride = NULL);

void LogV(const char* fmt, VA_LIST vaList) PRINTF_ATTRIB(1, 0);
void LogBin(const void* data, int size);
void LogTime();

__inline void Log(const char* fmt, ...) PRINTF_ATTRIB(1, 2);

__inline void Log(const char* fmt, ...) {
	VA_LIST argptr;
	VA_START(argptr, fmt);
	LogV(fmt, argptr);
}

//also logs some platform-specific time stamp and adds a '\n'
void LogTime(const char* fmt, ...) PRINTF_ATTRIB(1, 2);
#endif

void failFunction();

//******************************************************************************
//Macros
//******************************************************************************

#ifdef TRANSLATE_PANICS
#define PUBLIC_DEBUG
#endif

#ifdef PUBLIC_DEBUG
#define UPDATE_IP
#define TRACK_SYSCALL_ID
#endif

#ifdef SYMBIAN
#define TRACK_SYSCALL_ID
#endif

#ifdef LOGGING_ENABLED
#define LOG Log
#define LOGT LogTime
#define LOGBIN LogBin
#define LOG_ARG(arg) arg
#else
#ifdef SYMBIAN
#define LOG(a...)
#define LOGT(a...)
#else
#define LOG(...)
#define LOGT(...)
#endif	//SYMBIAN
#define LOGBIN(v, i)
#define LOG_ARG(arg)
#endif	//LOGGING_ENABLED

#ifdef DEBUGGING_MODE
#define LOGD LOG
#define LOGDT LOGT
#define DEBUG_ARG(arg) arg
#define MEMORY_DEBUG
#define PUBLIC_DEBUG
#ifndef SYMBIAN
#define TRANSLATE_PANICS
#endif
#else
#ifdef SYMBIAN
#define LOGD(a...)
#define LOGDT(a...)
#else
#define LOGD(...)
#define LOGDT(...)
#endif	//SYMBIAN
#define DEBUG_ARG(arg)
#endif  //DEBUGGING_MODE

#ifdef CORE_DEBUGGING_MODE
#define LOGC LOG
#ifdef SYMBIAN
#define LOGR(a...)
#else
#define LOGR(...)
#endif	//SYMBIAN
#define MEMORY_DEBUG
#define TRANSLATE_PANICS
#define SYSCALL_DEBUGGING_MODE
#define COREDEBUG_ARG(arg) arg
#else
#define LOGR LOG
#ifdef SYMBIAN
#define LOGC(a...)
#else
#define LOGC(...)
#endif	//SYMBIAN
#define COREDEBUG_ARG(arg)
#endif  //CORE_DEBUGGING_MODE

#ifdef SYSCALL_DEBUGGING_MODE
#define LOGSC\
	if(currentSyscallId >= MINIMUM_SYSCALL_ID && currentSyscallId <= MAX_SYSCALL_ID)\
	LOG
#define MEMORY_DEBUG
#define TRANSLATE_PANICS
#define SCDEBUG_ARG(arg) arg
#else
#ifdef SYMBIAN
#define LOGSC(a...)
#else
#define LOGSC(...)
#endif	//SYMBIAN
#define SCDEBUG_ARG(arg)
#endif  //CORE_DEBUGGING_MODE

#ifdef SOCKET_DEBUGGING_MODE
#define LOGS LOG
#define LOGST LOGT
#define SDEBUG_ARG(arg) arg
#else
#ifdef SYMBIAN
#define LOGS(a...)
#define LOGST(a...)
#else
#define LOGS(...)
#define LOGST(...)
#endif	//SYMBIAN
#define SDEBUG_ARG(arg)
#endif  //SOCKET_DEBUGGING_MODE

#ifdef GRAPHICS_DEBUGGING_MODE
#define LOGG LOG
#else
#ifdef SYMBIAN
#define LOGG(a...)
#else
#define LOGG(...)
#endif	//SYMBIAN
#endif  //GRAPHICS_DEBUGGING_MODE

#ifdef AUDIO_DEBUGGING_MODE
#define LOGA LOG
#else
#ifdef SYMBIAN
#define LOGA(a...)
#else
#define LOGA(...)
#endif	//SYMBIAN
#endif  //AUDIO_DEBUGGING_MODE

#ifdef BLUETOOTH_DEBUGGING_MODE
#define LOGBT LOG
#define LOGBTBIN LOGBIN
#define LOGBT_ARG(arg) arg
#else
#ifdef SYMBIAN
#define LOGBT(a...)
#else
#define LOGBT(...)
#endif	//SYMBIAN
#define LOGBTBIN(v, i)
#define LOGBT_ARG(arg)
#endif  //BLUETOOTH_DEBUGGING_MODE

#ifdef WLAN_DEBUGGING_MODE
#define LOGW LOG
#define LOGWBIN LOGBIN
#else
#ifdef SYMBIAN
#define LOGW(a...)
#else
#define LOGW(...)
#endif	//SYMBIAN
#define LOGWBIN(v, i)
#endif  //WLAN_DEBUGGING_MODE

#ifdef FILE_DEBUGGING_MODE
#define LOGF LOG
#define LOGFBIN LOGBIN
#else
#ifdef SYMBIAN
#define LOGF(a...)
#else
#define LOGF(...)
#endif	//SYMBIAN
#define LOGFBIN(v, i)
#endif  //FILE_DEBUGGING_MODE

#ifdef PIM_DEBUGGING_MODE
#define LOGP LOG
#define LOGPBIN LOGBIN
#else
#ifdef SYMBIAN
#define LOGP(a...)
#else
#define LOGP(...)
#endif	//SYMBIAN
#define LOGPBIN(v, i)
#endif  //PIM_DEBUGGING_MODE

#define LOG_RES LOG

#endif	//LOG_H
