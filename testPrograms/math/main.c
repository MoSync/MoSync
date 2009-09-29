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

#include <madmath.h>
#include <conprint.h>
#include <mastdlib.h>


#define GUIDO
#include <IX_GUIDO.H>


double drand() {
	double d = rand();
	d *= (1.0 / INT_MAX);
	return d;
}

void p1(const char* name, double in, double out) {
	printf("%s(%f) = %f", name, in, out);
}

#define TEST(func) {\
	double in = drand();\
	double out = func(in);\
	p1(#func, in, out);\
}

#define TEST2(func) {\
	/*double i1 = drand(), i2 = drand();\*/double i1 = 0.5, i2 = 5;\
	double out = func(i1, i2);\
	printf("%s(%f, %f) = %f", #func, i1, i2, out);\
}

int MAMain() {
	InitConsole();

	srand(0);

	printf("rand() = %i", rand());
	printf("drand() = %f", drand());

	TEST(sin);
	TEST(cos);
	TEST(tan);
	TEST(sqrt);

#ifdef FULLMATH
#define GUIDO
	TEST2(pow);
	TEST(log);
	TEST(tanh);

	TEST(asin);
	TEST(acos);
	TEST(atan);
#endif	//FULLMATH

#ifdef GUIDO
	TEST(sinh);
	TEST(cosh);

	TEST(atanh);

	TEST2(atan2);
#endif	//GUIDO

	while(maGetEvent() != EVENT_CLOSE) {
		maWait(WAIT_EVENT, 0);
	}
	return 0;
}


void printf_crash_test() {
	__int64 ll_qnan = 0x7FFFFFFFFFFFFFFF;
	__int64 llind = 0xfff8000000000000;
	double dind = sqrt(-10);//*(double*)&llind;
	printf("%f", dind);
}
