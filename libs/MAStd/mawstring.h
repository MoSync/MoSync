/** \file mawstring.h
* \brief Unicode string handling.
*/

//
// string.c
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
// Heavy modifications by A.R.Hartley Mobile Sorcery AB 2006
//

#ifndef MAWSTRING_H
#define MAWSTRING_H

#include "ma.h"

#ifndef MAPIP
#include <string.h>
#else

#include "ma.h"
#include "mactype.h"

#ifdef __cplusplus
extern "C" {
#endif

//#	define NO_BUILTINS

// ** Protos
#ifndef NO_BUILTINS
wchar *wstrncpy(wchar *dest, const wchar *source, size_t count);		// Built-in
int wstrncmp(const wchar *s1, const wchar *s2, size_t count);			// Built-in
wchar *wstrncat(wchar *s1, const wchar *s2, size_t count);				// Built-in
#endif

/**
* Compares strings \a s1 with \a s2 without case sensitivity.
* \returns
*          < 0 if s1 is less than s2\n
*          = 0 if s1 is equal to s2\n
*          > 0 if s1 is greater than s2\n
*/

int wstricmp(const wchar *s1, const wchar *s2);

/**
* Compares strings \a s1 with \a s2 for a max length \a count without case sensitivity.
* \returns
*          < 0 if s1 is less than s2\n
*          = 0 if s1 is equal to s2\n
*          > 0 if s1 is greater than s2\n
*/

int wstrnicmp(const wchar *s1, const wchar *s2, size_t count);

/**
* Scans a string for the first occurance of a character.
* \returns A pointer to the character, NULL if not found.
*/

wchar *wstrchr(const wchar *s, int ch);

/**
* Scans a string for the last occurance of a character.
* \returns A pointer to the character, NULL if not found.
*/

wchar *wstrrchr(const wchar *s, int ch);

/**
* Scans the substring \a str2 in string \a str1.
* \returns A pointer to the substring, NULL if not found.
*/


wchar *wstrstr(const wchar *str1, const wchar *str2);

/**
* Find length of initial segment matching mask.
* \returns Returns the length of the initial portion of \a string
* which consists only of characters that are part of \a control.
*/

size_t wstrspn(const wchar *string, const wchar *control);

/**
* Gets the length of a complementary substring.
* \returns The length of the initial part of \a string not containing any of the characters
* that are part of \a control.
* This is the length of \a string if none of the characters in \a control are found in \a string.
*/

size_t wstrcspn(const wchar *string, const wchar *control);

/**
* A pointer to the first occurrence in \a string of any of the characters
* that are part of \a control, or a null pointer if none of the characters
* of \a control is found in \a string before the terminating null-character.
*/

wchar *wstrpbrk(const wchar *string, const wchar *control);

/**
* Searches \a buf for count bytes for the first occurance of \a ch.
* \returns Pointer to the \a ch found, #NULL if not found.
*/

void *wmemchr(const void *buf, int ch, size_t count);

/**
* Searches \a buf for count bytes for the last occurance of \a ch.
* \returns Pointer to the \a ch found, #NULL if not found.
*/

void *wmemrchr(const void *buf, int ch, size_t count);

/**
* Makes a duplicate of a string, by allocation memory from the heap.
* \returns Pointer to the new string.
*/

wchar *wstrdup(const wchar *s);

/**
* Makes a string lower case.
* \returns Pointer to the end of the string.
*/


wchar *wstrlwr(wchar *s);

/**
* Makes a string upper case.
* \returns Pointer to the end of the string.
*/

wchar *wstrupr(wchar *s);

/**
* Sets a number of characters in the string \a s to \a c.
* \returns Pointer to the last character changed.
*/

wchar *wstrnset(wchar *s, int c, size_t count);

/**
* Reverses the order of the characters in a string.
* \returns Pointer to the altered string.
*/

wchar *wstrrev(wchar *s);

/**
* Get the length of a string.
* \returns The length of the string.
*/

size_t wstrlen(const wchar *s);

/**
* Appends the string \a dst with the string \a src,
* the dst string is appended at its end.
* \returns Destination string.
*/

wchar *wstrcat(wchar *dst, const wchar *src);

/**
* Sets all the characters in string \a s to the character \a c,
* \returns Destination string \a s.
*/

wchar *wstrset(wchar *s, int c);


int wstrcmp(const wchar *s1, const wchar *s2);

/**
* Converts a string to a double.
*
* Stores the pointer to the character immediately
* following the converted string in the pointer pointed to by \a endptr,
* if \a endptr is not NULL.
*/
double wstrtod(const wchar* string, wchar** endptr);

size_t wstrnlen(const wchar *s, size_t count);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//MAPIP

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//MAWSTRING_H
