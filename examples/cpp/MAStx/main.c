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

/** \file main.c
* A console-based program showing off heap management, printf and
* double-precision floating point mathematics.
*/

#include <conprint.h>
#include <matime.h>
#include <ma.h>
#include <maassert.h>

#ifdef USE_NEWLIB
#include <math.h>
#include <malloc.h>
#include <float.h>
#else
#include <madmath.h>
#include <maheap.h>
#endif	//USE_NEWLIB

int MAMain(void) GCCATTRIB(noreturn);
int MAMain(void) {
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
}
