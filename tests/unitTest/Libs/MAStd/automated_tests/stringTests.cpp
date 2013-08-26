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

#include "common.h"
#include <ma.h>
#include <mastdlib.h>
#include <mastring.h>
#include <conprint.h>
#include <madmath.h>

class StringTestCase : public TestCase {

public:
	StringTestCase() : TestCase("String Test") {

	}

	void strtodTest() {

/** Macro magic to make sure that **/

#define TEST_DOUBLES(m)\
	m(1.0),\
	m(.0),\
	m(1e+5),\
	m(6.e10),\
	m(0.5e-6),\
	m(3.2E+4),\
	m(7.321E-3),\
	m(5.3876e4),\
	m(10000000)\

#define STRINGIZE(s) #s
#define COPY(s) s
#if 0	//unfinished
		const char* testStrings[] = {
				TEST_DOUBLES(STRINGIZE)
		};

		const double testDoubles[] = {
				TEST_DOUBLES(COPY)
		};

		for(size_t i = 0; i < sizeof(testStrings)/sizeof(testStrings[0]); i++) {

		}
#endif
	}

	void start() {
		printf("Automated string tests\n");
		strtodTest();
/*
 		printf("running math tests\n");
		doubleTest();
		trigTest();
		sqrtTest();
		suite->runNextCase();
*/
	}

};

void addStringTests(TestSuite* suite);
void addStringTests(TestSuite* suite) {
	suite->addTestCase(new StringTestCase());
}
