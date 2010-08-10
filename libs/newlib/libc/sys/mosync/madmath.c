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

double __extendsfdf2(float a)
{
	return f2d(a);
}

// these should not be needed, as long == int.
long __fixsfdi(float a)
{
	return __fixdfsi(a);
}

long __fixdfdi(double a)
{
	return __fixdfsi(a);
}

#endif	//MAPIP
