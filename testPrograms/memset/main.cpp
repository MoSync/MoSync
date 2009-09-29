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
