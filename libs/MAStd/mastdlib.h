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

/** \file mastdlib.h
* \brief String conversion and pseudo-random number generation.
*/

#ifndef MASTDLIB_H
#define MASTDLIB_H

#include "ma.h"
#include "maxtoa.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAPIP
#include <stdlib.h>
//#include <stdio.h>
#ifdef _MSC_VER
#pragma warning(disable:4273 4005)
#else
#ifndef MOSYNC_NATIVE
#define stricmp strcasecmp
#endif
#endif
#else

/** \brief Converts ASCII number to integer.
* \param string A string containing a number.
* \return An integer
*
*/

int atoi(const char* string);

/**
* \see atoi
*/
long atol(const char* string);

/**
 * \brief Type that contains the quotient and reminder of a divison.
 */
typedef struct div_t {
	int quot, rem;
} div_t;

div_t div(int numer, int denom);

#endif	//MAPIP

/** \brief Hexadecimal ASCII number to integer.
* \param hexStg A string containing a hexadecimal number.
*/

int axtoi(const char *hexStg);

/*
 * Convert a string to a long integer.
 */
#ifndef __WINDOWS_PHONE_8__
long strtol(const char *nptr, char **endptr, int base);
#endif

/*
 * Convert a string to an unsigned long integer.
 */
unsigned long strtoul(const char *nptr, char **endptr, int base);

/*
 * Convert a string to an unsigned long long integer.
 */
unsigned long long strtoull(const char *nptr, char **endptr, int base);

/**
* Converts one UTF8 character to Latin-1.
* \param utf8 A pointer to a UTF8 character.
* \param pnBytes A pointer to an integer that gets the length in bytes of the UTF8 character.
* \return The converted character.
*/

char ConvertUtf8ToLatin1(const char* utf8, int* pnBytes) GCCATTRIB(deprecated);

/**
* Compares a UTF8 string with a Latin-1 string.
* \param utf8 A pointer to a UTF8 string.
* \param latin1 A pointer to a latin1 string.
* \return #TRUE if the strings matched, #FALSE otherwise
*/

BOOL StringEqual_utf8_latin1(const char* utf8, const char* latin1) GCCATTRIB(deprecated);

/**
* Finds the first occurance of a character in a string
* \param a The character to find.
* \param b The string in which to search.
* \return Returns an integer index to the first occurance of the character,
* or NULL if the character was not found.
*/

int find(const char* a, char b);

#ifdef RAND_MAX
#undef RAND_MAX
#endif
#define RAND_MAX 0x7fffffff

/** \brief Generate and return 31-bit random value.
*
* Returns a pseudo-random integral number in the range 0 to #RAND_MAX.
*/
int rand(void);

/** \brief Generate and return 32-bit random value.
*
* Returns a pseudo-random number in the range 0 to 0xFFFF FFFF.
*/
int urand(void);

/** \brief Initialize random number generator.
*
* The pseudo-random number generator is initialized using the argument passed as seed.
* For every different seed value used in a call to srand, the pseudo-random number generator can be expected to generate a different succession of results in the subsequent calls to rand.
* Two different initializations with the same seed, instructs the pseudo-random generator to generate the same succession of results for the subsequent calls to rand in both cases.
* If seed is set to 1, the generator is reinitialized to its initial value and produces the same values as before any call to rand or srand.
* In order to generate random-like numbers, srand is usually initialized to some distinctive value, like those related with the execution time.
* \param seed An integer value to be used as seed by the pseudo-random number generator algorithm.
*/
void srand(unsigned int seed);

/**
* Returns the absolute value of \a x.
*/
int abs(int x);

/**
* Places \a n zero-valued bytes in the area pointed to by \a s.
*/
void bzero(void* s, size_t n);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* MASTDLIB_H */
