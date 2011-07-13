/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
