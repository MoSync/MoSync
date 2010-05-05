/** \file mastring.h
* \brief String handling.
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

#ifndef MASTRING_H
#define MASTRING_H

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
char *strncpy(char *dest, const char *source, size_t count);		// Built-in
int strncmp(const char *s1, const char *s2, size_t count);			// Built-in
char *strncat(char *s1, const char *s2, size_t count);				// Built-in
int memcmp(const void *dst, const void *src, size_t n);				// Built-in
#endif

/**
* Compares strings \a s1 with \a s2 without case sensitivity.
* \returns
*          < 0 if s1 is less than s2\n
*          = 0 if s1 is equal to s2\n
*          > 0 if s1 is greater than s2\n
*/

int stricmp(const char *s1, const char *s2);

/**
* Compares strings \a s1 with \a s2 for a max length \a count without case sensitivity.
* \returns
*          < 0 if s1 is less than s2\n
*          = 0 if s1 is equal to s2\n
*          > 0 if s1 is greater than s2\n
*/

int strnicmp(const char *s1, const char *s2, size_t count);

/**
* Scans a string for the first occurance of a character.
* \returns A pointer to the character, NULL if not found.
*/

char *strchr(const char *s, int ch);

/**
* Scans a string for the last occurance of a character.
* \returns A pointer to the character, NULL if not found.
*/

char *strrchr(const char *s, int ch);

/**
* Scans the substring \a str2 in string \a str1.
* \returns A pointer to the substring, NULL if not found.
*/


char *strstr(const char *str1, const char *str2);

/**
* Find length of initial segment matching mask.
* \returns Returns the length of the initial portion of \a string
* which consists only of characters that are part of \a control.
*/

size_t strspn(const char *string, const char *control);

/**
* Gets the length of a complementary substring.
* \returns The length of the initial part of \a string not containing any of the characters
* that are part of \a control.
* This is the length of \a string if none of the characters in \a control are found in \a string.
*/

size_t strcspn(const char *string, const char *control);

/**
* A pointer to the first occurrence in \a string of any of the characters
* that are part of \a control, or a null pointer if none of the characters
* of \a control is found in \a string before the terminating null-character.
*/

char *strpbrk(const char *string, const char *control);

/**
* Moves count bytes from \a src to \a dst.
* \returns Pointer to the last destination memory address.
*/

void *memmove(void *dst, const void *src, size_t count);

/**
* Searches \a buf for count bytes for the first occurance of \a ch.
* \returns Pointer to the \a ch found, #NULL if not found.
*/

void *memchr(const void *buf, int ch, size_t count);

/**
* Searches \a buf for count bytes for the last occurance of \a ch.
* \returns Pointer to the \a ch found, #NULL if not found.
*/

void *memrchr(const void *buf, int ch, size_t count);

/**
* Makes a duplicate of a string, by allocation memory from the heap.
* \returns Pointer to the new string.
*/

char *strdup(const char *s);

/**
* Makes a string lower case.
* \returns Pointer to the end of the string.
*/


char *strlwr(char *s);

/**
* Makes a string upper case.
* \returns Pointer to the end of the string.
*/

char *strupr(char *s);

/**
* Sets a number of characters in the string \a s to \a c.
* \returns Pointer to the last character changed.
*/

char *strnset(char *s, int c, size_t count);

/**
* Reverses the order of the characters in a string.
* \returns Pointer to the altered string.
*/

char *strrev(char *s);

/**
* Copies bytes from memory area src into dst, stops if byte c is encounted
* \returns A Pointer to the byte after the copy of c in dst, if c not found a null pointer.
*/

void *memccpy(void *dst, const void *src, int c, size_t count);

/**
* Compares first n (count) bytes of buf1 & buf2, is case insensitive.
* \returns
*          < 0 if buf1 is less than buf2\n
*          = 0 if buf1 is equal to buf2\n
*          > 0 if buf1 is greater than buf2\n
*/

int memicmp(const void *buf1, const void *buf2, size_t count);

/**
* Get the length of a string.
* \returns The length of the string.
*/

size_t strlen(const char *s);

/**
* Appends the string \a dst with the string \a src,
* the dst string is appended at its end.
* \returns Destination string.
*/

char *strcat(char *dst, const char *src);

/**
* Sets all the characters in string \a s to the character \a c,
* \returns Destination string \a s.
*/

char *strset(char *s, int c);


char *strtok_r(char *string, const char *control, char **lasts);
char *strtok(char *string, const char *control);

/**
* Converts a string to a double.
*
* Stores the pointer to the character immediately
* following the converted string in the pointer pointed to by \a endptr,
* if \a endptr is not NULL.
*/
double strtod(const char* string, char** endptr);

size_t strnlen(const char *s, size_t count);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//MAPIP

#ifdef __cplusplus
extern "C" {
#endif

/**
* Compares two null-terminated strings.
* Returns #TRUE if the characters of the shorter string equals the first characters
* of the longer string.
*/
BOOL StringMatch(const char* a, const char* b);
/**
* Compares two null-terminated strings.
* Returns #TRUE if the characters of \a a equals the first characters of \a b.
*/
BOOL StringMatch1(const char* a, const char* b);
/**
* Compares memory areas. Returns #TRUE if identical, #FALSE otherwise.
*/
BOOL memeq(const void* a, const void* b, int size);
/**
* Converts a single Latin-1 character to UTF-8.
* Returns the number of bytes written to \a dst (1 or 2).
*/
int ConvertLatin1ToUtf8(char* dst, int src) GCCATTRIB(deprecated);
/**
* Stores a string representation of \a a into the buffer pointed to by \a dst.
* Returns strlen(dst).
*/
int i2a(int a, char* dst);
/**
* Stores a string representation of \a d into the buffer pointed to by \a dst.
* Returns strlen(dst).
*/
int d2a(double d, char* dst);


#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//MASTRING_H
