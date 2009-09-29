/** \file maxtoa.h
* \brief Convert integers to ASCII strings.
*/

//
// xtoa.c
//
// Convert integers to ASCII strings
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

#ifndef MAXTOA_H
#define MAXTOA_H

#ifdef _MSC_VER
#include <stdlib.h>
#else

#ifdef __cplusplus
extern "C" {
#endif


/**
* Convert integer to ascii string.
* \param val The integer value to convert.
* \param buf The output buffer for the ascii representation.
* \param radix The output radix.
* \param negative The output sign.
*/

void xtoa(unsigned long val, char *buf, unsigned radix, int negative);

/**
* Convert integer to ascii string.
* \param val The integer value to convert.
* \param buf The output buffer for the ascii representation.
* \param radix The output radix.
*/

char *itoa(int val, char *buf, int radix);

/**
* Convert long to ascii string.
* \param val The integer value to convert.
* \param buf The output buffer for the ascii representation.
* \param radix The output radix.
*/

char *ltoa(long val, char *buf, int radix);

/**
* Convert unsigned long to ascii string.
* \param val The integer value to convert.
* \param buf The output buffer for the ascii representation.
* \param radix The output radix.
*/

char *ultoa(unsigned long val, char *buf, int radix);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//MAPIP

#endif	//MAXTOA_H
