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
#include <mastdlib.h>

class MemTestCase : public TestCase {

public:
	MemTestCase() : TestCase("Mem test") {
	}
	
	void stringAndMemTest() {
		char temp[256];
		const char *a, *b;

		memset(temp, 3, 256);		
		int numFailed = 255;
		while(numFailed) {
			if(temp[numFailed]!=3)
				break;
			numFailed--;
		}
		assert("testing memset()", numFailed==0);

		unsigned char *tempMemory = new unsigned char[256];
		int numTests = 10;
		while(numTests--) {
			int length = rand()&0xf;
			int src_addr_start = rand()%(256-length);
			int src_addr_end = src_addr_start + length;
			int dst_addr_start = rand()%(256-length);
			int dst_addr_end = dst_addr_start + length;		
			while(dst_addr_start<=src_addr_end && dst_addr_end >= src_addr_start) {
				dst_addr_start = rand()%(256-length);
				dst_addr_end = dst_addr_start + length;	
			}
			unsigned char *src = &tempMemory[src_addr_start];
			unsigned char *dst = &tempMemory[dst_addr_start];

			memcpy(dst, src, length);
			
			numFailed = 0;
			while(length) {
				if(*dst++!=*src++) numFailed++;
				length--;
			}
			assert("testing memcpy()", numFailed==0);
		}

		a = "foo";
		b = "foo";
		assert("testing foo==foo", strcmp(a, b)==0);

		a = "foo";
		b = "bar";
		assert("testing foo!=bar", strcmp(a, b)!=0);
		
		memset(temp, 0, 256);
		strcpy(temp, a);
		assert("testing strcpy()", strcmp(temp, "foo")==0);
		delete tempMemory;
	}

	void start() {
		stringAndMemTest();
		suite->runNextCase();
	}

};

void addMemTests(TestSuite* suite);
void addMemTests(TestSuite* suite) {
	suite->addTestCase(new MemTestCase());
}
