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

/** \file maassert.h
* \brief Fatal error handling.
*/

#ifndef _SE_MSAB_MASTD_MAASSERT_H_
#define _SE_MSAB_MASTD_MAASSERT_H_

#include "ma.h"

#if defined(__GNUC__)
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#define ALIGN(ament, func) ATTRIBUTE(aligned(ament), func)
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ALIGN(ament, func) ATTRIBUTE(align(ament), func)
#else
#error Unsupported compiler!
#endif	//__GNUC__

#ifdef  __cplusplus
extern "C" {
#endif

/**
* Returns a pointer to the filename in a full path.
*/
const char* FileNameFromPath(const char* path);

/**
* Freezes the program. Calls maExit() with \a val on Close Event or keypress #MAK_0.
*/
void ATTRIBUTE(noreturn, Freeze(int val));

#define FREEZE Freeze(0)

#define MAASSERT(a) do{ if(!(a)) BIG_PHAT_ERROR; }while(0)
#define BIG_PHAT_ERROR BIG_PHAT_ERROR_VAL(6)

#define BIG_PHAT_ERROR_VAL(value) bfeHandler(value, FileNameFromPath(__FILE__), __LINE__)

/**
* Handles a fatal error by printing a message to the console, then freezing.
* The message consists of filename and line number where the error occurred.
* \see Freeze
*/
void ATTRIBUTE(noreturn, bfeHandler(int val, const char* filename, int line));

void ATTRIBUTE(noreturn, panicFunction(const char* msg, const char* function));

/**
* \def PANIC_MESSAGE(msg)
* Handles a fatal error by calling maPanic().
* The message includes the name of the function where the error occurred.
* \see Freeze
*/
#define PANIC_MESSAGE(msg) panicFunction((msg), __FUNCTION__)

#define ASSERT_MSG(cond, msg) do{ if(!(cond)) PANIC_MESSAGE(msg); }while(0)

#ifdef  __cplusplus
}
#endif

#endif	//_SE_MSAB_MASTD_MAASSERT_H_
