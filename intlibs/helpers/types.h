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

#ifndef _INTLIBS_TYPES_H_
#define _INTLIBS_TYPES_H_

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

#ifdef _MSC_VER
# define PFZT_PREFIX "I"
typedef long int ssize_t;
#elif defined(SYMBIAN)
# define PFZT_PREFIX ""
#elif defined(__GNUC__)
# if (__GNUC__ == 4) && defined(WIN32)
#  define PFZT_PREFIX "I"
# else
#  define PFZT_PREFIX "z"
# endif	//WIN32
#else
# error Unsupported compiler!
#endif

#define PFZT PFZT_PREFIX "u"
#define PFSZT PFZT_PREFIX "i"
#define PFXZT PFZT_PREFIX "x"

#ifdef SYMBIAN
# define PFP "08x"
#else
# define PFP "p"
#endif

#endif // _INTLIBS_TYPES_H_
