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

#include <madmath.h>
#include <maassert.h>
#include <conprint.h>
#include <mastdlib.h>
#include <limits.h>
#include <maapi_defs.h>
typedef union MA_DV MA_DV;

//#define GUIDO
//#include <IX_GUIDO.H>
#define FULLMATH


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
//#define GUIDO
	TEST2(pow);
	TEST(log);

	TEST(asin);
	TEST(acos);
	TEST(atan);
#endif	//FULLMATH

#ifdef GUIDO
	TEST(sinh);
	TEST(cosh);
	TEST(tanh);

	TEST(atanh);
#endif	//GUIDO

	TEST2(atan2);

	FREEZE;
	return 0;
}

#if 0
void printf_crash_test() {
	__int64 ll_qnan = 0x7FFFFFFFFFFFFFFF;
	__int64 llind = 0xfff8000000000000;
	double dind = sqrt(-10);//*(double*)&llind;
	printf("%f", dind);
}
#endif
