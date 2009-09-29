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

/** \file main.c
* A console-based program showing off heap management, printf and
* double-precision floating point mathematics.
*/

#include <conprint.h>
#include <madmath.h>
#include <maheap.h>
#include <matime.h>
#include <ma.h>
#include "../common/helpers.h"

int MAMain() {
	InitConsole();

	//These are declared extern in conprint.h.
	gConsoleBackgroundColor = 0xFF800080;
	gConsoleTextColor = -1;
	gConsoleLogging = 1;

	printf("Hello PI!\n");
	printf("Current local time:\n");
	printf("%s\n", sprint_time(maLocalTime()));
	printf("M_PI = %g\n", M_PI);
	printf("sin(pi/2) = %g\n", sin(M_PI/2));
	printf("Let's do some math\n");

	printf("acos(.5): %g\n", acos(.5));
	printf("asin(1): %g\n", asin(1));
	printf("atan2(1, 4): %f\n", atan2(1, 4));
	printf("log(M_PI): %f\n", log(M_PI));
	printf("pow(2, 4.3): %f\n", pow(2, 4.3));
	printf("atan(1): %f\n", atan(1));
	printf("ceil(1.56): %g\n", ceil(1.56));
	printf("floor(2.3): %g\n", floor(2.3));
	{
		int e;
		double ex = frexp(M_PI, &e);
		printf("frexp(M_PI): %g, %i\n", ex, e);
	}

	printf("Epsilon: %g\n", DBL_EPSILON);
	printf("Min: %g\n", DBL_MIN);
	printf("Max: %g\n", DBL_MAX);	//vsprintf has a bug when printing exceptionally large numbers
	printf("BigGood: %g\n", 4.5470*1000000000.0);	//but moderately large numbers work fine
	printf("BigGood2: %g\n", 1.543e+75);
	printf("BigBad: %g\n", 1.345e+80);	//seems to be capped at e+79
	printf("IntMax: %i\n", 0x7FFFFFFF);
	{
		double mod, intPart, x = 4.25;
		printf("Modf %g:", x);
		mod = modf(x, &intPart);
		printf("%g %g\n", mod, intPart);
		mod *= 10000000000.0;
		printf("BigMod: %g\n", mod);
		mod /= (intPart * 10);
		printf("SmallMod: %g\n", mod);
	}

	printf("Let's allocate some memory\n");
	{
		int size = 0x800;
		void* ptr = malloc(size);
		printf("%i %p\n", size, ptr);
		free(ptr);

		printf("Attempting to allocate a gigabyte...\n");
		printf("Expect a panic.\n");

		size = 1 << 30;	//do we have a gigabyte of memory? :)
		ptr = malloc(size);	//probably not. thus a Panic occurs.
		printf("%i %p\n", size, ptr);
		free(ptr);
	}
	printf("We're done.\n");
	FREEZE;
	return 0;
}
