/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file madmath.h
* \brief Intrinsic double-precision math
* \author Antony Hartley
*/

//********************************************************************************
// Mobile Author
// Intrinsic Double Precision Math Support Lib
//
// Author: Antony Hartley <antony.hartley@mobilesorcery.com>
//
// History:
// 06/07/2006	first beta version
// 
//********************************************************************************

#ifndef MADMATH_H
#define MADMATH_H

#define DBL_MANT_DIG 	53
#define DBL_DIG 		15
#define DBL_EPSILON 	2.2204460492503131e-16
#define DBL_MIN_EXP 	(-1021)
#define DBL_MIN 		2.2250738585072014e-308
#define DBL_MIN_10_EXP 	(-307)
#define DBL_MAX_EXP 	1024
#define DBL_MAX 		1.7976931348623157e+308
#define DBL_MAX_10_EXP 	308

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

#define M_PI_D180 ((double)((double) 3.14159265358979323846 / (double) 180))

#include "ma.h"
#include "mastring.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MAPIP
/**
* Breaks \a x into two parts: the integer part (stored in the object pointed by \a intpart)
* and the fractional part (returned by the function).
* Each part has the same sign as \a x.
*/
double modf(double x, double* intptr);

/**
* Returns the floating point remainder of the division (numerator / denominator).
*/
double fmod(double numerator, double denominator);

/**
* Returns the absolute value of \a x.
*/
double fabs(double d);

/**
* Parses the null-terminated \a string, interpreting its content as a floating point number
* and returns its value as a double.
*/
double atof(const char* string);


/**
* Returns the arc cosine of \a x.
*/
double acos(double x);

/**
* Returns the arc sine of \a x.
*/
double asin(double x);

/**
* Returns the angle of bidimensional vector (x, y),
* expressed in radians, in the range -PI to PI.
*/
double atan2(double y, double x);

/**
* Returns the natural logarithm of \a x.
*/
double log(double x);

/**
* Returns \a x to the power of \a y.
*/
double pow(double x, double y);

/**
* Returns the arctangent of \a x.
*/
double atan(double x);

/**
* Returns \a x rounded toward -inf to integral value.
*/ 
double ceil(double x);

/**
* Returns \a x rounded toward -inf to integral value.
*/
double floor(double x);

/**
* for non-zero x 
*	x = frexp(arg,&exp);
* return a double fp quantity x such that 0.5 <= |x| <1.0
* and the corresponding binary exponent "exp". That is
*	arg = x*2^exp.
* If arg is inf, 0.0, or NaN, then frexp(arg,&exp) returns arg 
* with *exp=0. 
*/
double frexp(double x, int *eptr);

/**
* Returns 1 if \a x is Not a Number. 0 otherwise.
*/
int isnan(double x);

/**
* Returns 1 if \a x is Infinite. 0 otherwise.
*/
int isinf(double x);

#else

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <math.h>

#endif //MAPIP

#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* MADMATH_H */
