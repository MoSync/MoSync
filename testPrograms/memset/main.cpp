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

#include <ma.h>
#include <mavsprintf.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SIZE 32
#define BASE_OFFSET 4

void test();
void testSingle();
void checkBounds();
void checkContents();
void error(const char* msg, int extra);

void myMemSet(void* dst, int val, int count);
void dumpMemoryBytes(byte* mem, int count, int bytesPerLine);

extern "C" int MAMain() {
	test();

	static char endMsg[] = "Test successful.\n";
	maWriteLog(endMsg, sizeof(endMsg) - 1);

	return 0;
}

int gOffset;
int gCount;
byte gBuffer[SIZE * 64];

void test() {
	for(gOffset = 0; gOffset <= 5; gOffset++) {
		for(gCount = 0; gCount <= SIZE / 2; gCount++) {
			testSingle();
		}
	}

	//test big
	gOffset = 3;
	gCount = 345;
	testSingle();
}

void myMemSet(void* dst, int val, int count) {
	byte* ptr = (byte*)dst;
	byte* endptr = ptr + count;
	while(ptr < endptr)
		*ptr++ = (byte)val;
}

void testSingle() {
	myMemSet(gBuffer, 0xFF, sizeof(gBuffer));
	memset(gBuffer + BASE_OFFSET + gOffset, 0x80, gCount);
	checkBounds();
	checkContents();
	//dumpMemoryBytes(gBuffer, SIZE, 16);
}

void checkBounds() {
	//check left bound
	for(int i=0; i<BASE_OFFSET + gOffset; i++) {
		if(gBuffer[i] != 0xFF)
			error("Left bound overwritten!", i);
	}
	//check right bound
	for(int i=BASE_OFFSET + gOffset + gCount; i<BASE_OFFSET + gOffset + gCount + BASE_OFFSET; i++) {
		if(gBuffer[i] != 0xFF)
			error("Right bound overwritten!", i);
	}
}

void checkContents() {
	for(int i=BASE_OFFSET + gOffset; i<BASE_OFFSET + gOffset+gCount; i++) {
		if(gBuffer[i] != 0x80)
			error("Bad contents!", i);
	}
}

void dumpMemoryBytes(byte* mem, int count, int bytesPerLine) {
	lprintfln("0x%08X", mem);
	for(; count > 0; count -= bytesPerLine) {
		const int lineCount = MIN(count, bytesPerLine);
		char buffer[128];
		char* ptr = buffer;
		for(int j=0; j<lineCount; j++) {
			ptr += sprintf(ptr, "%02X ", *mem++);
			if((j & 3) == 3)
				*ptr++ = ' ';
		}
		ptr--;	//remove last space
		*ptr = 0;
		maWriteLog(buffer, strlen(buffer));
	}
}

void error(const char* msg, int extra) {
	lprintfln("%i %i  %i", gOffset, gCount, extra);
	dumpMemoryBytes(gBuffer, SIZE, 16);
	maPanic(1, msg);
}
