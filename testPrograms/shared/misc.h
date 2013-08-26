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

#ifndef MISC_H
#define MISC_H

#ifndef __cplusplus
typedef enum _bool { false=0, true=1 } bool;
#endif

//typedef long long s64;
//typedef unsigned long long u64;
static const int INT_MAX = 2147483647;

/// Number of base-FLT_RADIX digits in the significand of a double.
#define DBL_MANT_DIG 53
/// Number of decimal digits of precision in a double.
#define DBL_DIG 15
/// Difference between 1.0 and the minimum double greater than 1.0.
#define DBL_EPSILON 2.2204460492503131e-16
/// Minimum int x such that FLT_RADIX**(x-1) is a normalised double.
#define DBL_MIN_EXP (-1021)
/// Minimum normalised double.
#define DBL_MIN 2.2250738585072014e-308
/// Minimum int x such that 10**x is a normalised double.
#define DBL_MIN_10_EXP (-307)
/// Maximum int x such that FLT_RADIX**(x-1) is a representable double.
#define DBL_MAX_EXP 1024
/// Maximum double.
#define DBL_MAX 1.7976931348623157e+308
/// Maximum int x such that 10**x is a representable double.
#define DBL_MAX_10_EXP 308 

#define iswhite(c)	((c) == ' ' || (c) == '\t' || (c) == 0x0d || (c) == 0x0a)
#define isdigit(c)	((c) >= '0' && (c) <= '9')
#define isxdigit(c)	(((c) >= 'A' && (c) <= 'F') || isdigit(c))
#define toupper(c)	((c) & 0xDF)

static const unsigned int gPowersOfTen[] = { 10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };

#define streq(a,b) (strcmp(a,b) == 0)
#define deq(a,b) (dcmp(a,b) == 0)
#define dgt(a,b) (dcmp(a,b) > 0)  //a > b
#define dlt(a,b) (dcmp(a,b) < 0)  //a < b

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))

#define DMIN(a,b) (dlt(a,b) ? (a) : (b))

#if 0
/**
* Stores the integer part of \<x\> into *\<intptr\> and returns the fractal part.
*/
double modf(double x, double* intptr);

/**
* Returns the length of the null-terminated string \<str\>, excluding the null.
*/
#ifdef MAVM
int strlen(const char* str);
#else
unsigned int strlen(const char* str);
#endif
#endif	//0

/**
* Lexiographically compares the first \<len\> characters of the null-terminated strings \<a\> and \<b\>.
* Returns >0 if a>b, 0 if a==b and <0 if a<b.
*/
int strncmp(const char* a, const char* b, size_t len);
/**
* Converts a string to a floating-point value.
* Stores the address of the first unconverted char in endptr.
*/
double strtod(const char* string, const char** endptr);
/**
* Converts a string to an integer.
*/
int atoi(const char* string);

/**
* Returns the index of the first occurrence of \<b\> in the null-terminated string \<a\>,
* or -1 if there is none.
*/
int FindFirst(const char* a, char b);

/**
* Returns the index of the last occurrence of \<b\> in the null-terminated string \<a\>,
* or -1 if there is none.
*/
int FindLast(const char* a, char b);

/**
* Waits for the user to press and release any key.
*/
void PAKAndRelease();
/**
* Waits for the user to press and release the specified key.
*/
void PressAndRelease(int key);

//INEFFICIENT!
#define SCREEN_HEIGHT EXTENT_Y(maGetScrSize())
#define SCREEN_WIDTH EXTENT_X(maGetScrSize())

#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define COLOR_WHITE (-1)
#define COLOR_BLACK 0

#endif	//MISC_H
