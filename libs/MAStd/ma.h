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

/** \file ma.h
* \brief Defines MoSync program entry point, size_t, NULL and BOOL
*/


#ifndef MA_H
#define MA_H

#include "maapi.h"

#ifndef MOSYNC_NATIVE

#ifndef SIZE_T_DEFINED
#ifdef _WIN32
typedef unsigned int size_t;
#elif defined(MAPIP)
typedef unsigned long size_t;
#endif
#define SIZE_T_DEFINED
#endif

#endif /* MOSYNC_NATIVE */

#ifdef MAPIP
#define BOOL	int
#else //NOT MAPIP
typedef int BOOL;
#endif //MAPIP

#define TRUE	1
#define FALSE	0

#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
extern "C"
#endif
int MAMain(void);

#ifdef MAPIP
#define _ma_inline(string) __asm__(string "\n")	
#else
#define _ma_inline(string)	
#endif

void ATTRIBUTE(noreturn, ErrorExit(const char * str, int v));

#endif /* MA_H */
