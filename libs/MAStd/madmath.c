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

#include "ma.h"
#include "madmath.h"

#ifdef MAPIP

// Functions are intrinsic no prototypes needed
// See http://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html
// for documentation

// ** Convert float to unsigned int
unsigned int __fixunssfsi(float a)			/// ??? Might need fixing
{
	if (a < 0)
		return 0;

	if (a > 0xffffffff)
		return 0xffffffff;

	if (a < 0x80000000)
		return (unsigned) __fixsfsi(a);
	
	return ((unsigned int) __fixsfsi(a - 0x7fffffff)) + 0x7fffffff;
}

// ** if a greater than b

int __gtsf2(float a, float b)
{
	return fcmp(a,b);
}

// ** if a less than or equal to b

int __lesf2(float a, float b)
{
	return fcmp(a,b);
}

// ** if a less than  b

int __ltsf2(float a, float b)
{
	return fcmp(a,b);
}

// ** if a greater than or equal to b

int __gesf2(float a, float b)
{
	return fcmp(a,b);
}

// ** if a is equal to b

int __eqsf2(float a, float b)
{
	return fcmp(a,b);
}

// ** if a is not equal to b

int __nesf2(float a, float b)
{
	return fcmp(a,b);
}


// ** Convert double to unsigned int

unsigned int __fixunsdfsi(double a)			/// ??? Might need fixing
{
	if (a < 0)
		return 0;

	if (a > 0xffffffff)
		return 0xffffffff;

	if (a < 0x80000000)
		return (unsigned) __fixdfsi(a);
	
	return ((unsigned int) __fixdfsi(a - 0x7fffffff)) + 0x7fffffff;
}

// ** if a greater than b

int __gtdf2(double a, double b)
{
	return dcmp(a,b);
}

// ** if a less than or equal to b

int __ledf2(double a, double b)
{
	return dcmp(a,b);
}

// ** if a less than  b

int __ltdf2(double a, double b)
{
	return dcmp(a,b);
}

// ** if a greater than or equal to b

int __gedf2(double a, double b)
{
	return dcmp(a,b);
}

// ** if a is equal to b

int __eqdf2(double a, double b)
{
	return dcmp(a,b);
}

// ** if a is not equal to b

int __nedf2(double a, double b)
{
	return dcmp(a,b);
}

float __truncdfsf2 (double a)
{
	return d2f(a);
}

/* the following deal with IEEE single-precision numbers */

#define EXCESS		126
#define SIGNBIT		0x80000000
#define HIDDEN		(1 << 23)
#define FSIGN(fp)	((fp) & SIGNBIT)
#define EXP(fp)		(((fp) >> 23) & 0xFF)
#define MANT(fp)	(((fp) & 0x7FFFFF) | HIDDEN)
#define PACK(s,e,m)	((s) | ((e) << 23) | (m))

/* the following deal with IEEE double-precision numbers */

#define EXCESSD		1022
#define HIDDEND		(1 << 20)
#define EXPD(fp)	(((fp.l.upper) >> 20) & 0x7FF)
#define SIGND(fp)	((fp.l.upper) & SIGNBIT)
#define MANTD(fp)	(((((fp.l.upper) & 0xFFFFF) | HIDDEND) << 10) | \
				(fp.l.lower >> 22))
#define HIDDEND_LL	((long long)1 << 52)
#define MANTD_LL(fp)	((fp.ll & (HIDDEND_LL-1)) | HIDDEND_LL)
#define PACKD_LL(s,e,m)	(((long long)((s)+((e)<<20))<<32)|(m))

union double_long
{
    double d;
#ifdef SWAP
    struct
    {
      unsigned long lower;
      long upper;
    } l;
#else
    struct
    {
      long upper;
      unsigned long lower;
    } l;
#endif
    long long ll;
};

union float_long
{
	float f;
	long l;
};

// ** convert float to double

double __extendsfdf2 (float a1)
{
  return f2d(a1);
  /*
  register union float_long fl1;
  register union double_long dl;
  register int exp;

  fl1.f = a1;

  if (!fl1.l)
    {
      dl.l.upper = dl.l.lower = 0;
      return (dl.d);
    }

  dl.l.upper = FSIGN (fl1.l);
  exp = EXP (fl1.l) - EXCESS + EXCESSD;
  dl.l.upper |= exp << 20;
  dl.l.upper |= (MANT (fl1.l) & ~HIDDEN) >> 3;
  dl.l.lower = MANT (fl1.l) << 29;

  return (dl.d);
  */
}

double fabs(double d) {
	return d > 0 ? d : -d;
}

double atof(const char* string) {
	return strtod(string, NULL);
}

double fmod(double numerator, double denominator) {
	double res = numerator - floor(numerator/denominator) * denominator;
	if((numerator < 0.0) != (denominator < 0.0))
		res = res - denominator;
	return res;
}

#endif	//MAPIP

#if 0
//This function is not intrinsic
double modf(double x, double* intptr)
{
	//WILL BREAK if x > MAX_INT
  int i = (int) x;
  *intptr = i;
  return (x - *intptr);
}
#endif
