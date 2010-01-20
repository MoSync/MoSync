/** \file mavsprintf.h
* \brief vsprintf and support functions.
*/

//
// vsprintf.c
//
// Print formatting routines
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

#ifndef MAVSPRINTF_H
#define MAVSPRINTF_H

#ifndef MAPIP
#include <stdio.h>
#endif

//#include <sys/types.h>
#include "maarg.h"
#include "mastring.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _STDIO_H
/** \brief vsprintf - print formatted output
* \param buf output string
* \param fmt input format string
* \param args format variable arg pointer list
* \return Returns string length of output
*/


int vsprintf(char *buf, const char *fmt, va_list args);


/** \brief sprintf - print formatted output
* \param buf output string
* \param fmt input format string
* \return Returns string length of output
*/
int sprintf(char *buf, const char *fmt, ...);
#endif	//_STDIO_H

/**
* Writes a line of formatted output to the system debug log.
* Will do nothing on release versions.
*
* \warning This function uses a fixed buffer size.
* Outputting too much data at once will cause a buffer overrun.
*
* \return Length of output, or \<0 if the debug log is unavailable.
*/
int lprintfln(const char* fmt, ...) GCCATTRIB(format(printf, 1, 2));

#ifdef __cplusplus
}	//extern "C"
#endif

#endif
