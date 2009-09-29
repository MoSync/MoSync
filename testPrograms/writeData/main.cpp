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

//assumes that readData and memset works

#include <ma.h>
#include <mavsprintf.h>

#include "maheaders.h"


#define BASE_OFFSET 4
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SIZE 32

int gOffset;
int gCount;
byte gBuffer[SIZE * 2];

void test();
void testSingle();
void checkBounds();
void checkContents(byte* ptr, int offset);
void error(const char* msg, int extra);

extern "C" int MAMain() {

	test();
	/*
	gOffset = 0;
	gCount = 8;
	testSingle();*/

	lprintfln("Test successful.");
	return 0;
}


void test() {
	for(gOffset = 0; gOffset <= 5; gOffset++) {
		for(gCount = 0; gCount <= SIZE / 2; gCount++) {
			testSingle();
		}
	}
}

void testSingle() {
	//the actual test (assumes that readData works)
	{
		//clear data object
		memset(gBuffer, 0xff, SIZE);
		maWriteData(1, gBuffer, 0, SIZE);

		//setup data to be written
		//maReadData(1, gBuffer + gAddress, gOffset + BASE_OFFSET, gCount);	
		for(int i=0; i<SIZE; i++) {
			gBuffer[i] = (byte)i;
		}

		//write data. this is the function being tested.
		maWriteData(1, gBuffer + BASE_OFFSET + gOffset, gOffset + BASE_OFFSET, gCount);

		//read entire object to see all errors
		maReadData(1, gBuffer, 0, SIZE);
	}
	
	checkBounds();
	checkContents(gBuffer + BASE_OFFSET + gOffset, gOffset + BASE_OFFSET);
}

void checkBounds() {
	//check left bound
	for(int i=0; i<BASE_OFFSET + gOffset; i++) {
		if(gBuffer[i] != 0xFF)
			error("Left bound overwritten!", i);
	}
	//check right bound
	for(int i=BASE_OFFSET + gOffset + gCount; i<SIZE; i++) {
		if(gBuffer[i] != 0xFF)
			error("Right bound overwritten!", i);
	}
}

void checkContents(byte* ptr, int offset) {
	for(int i=offset; i<offset+gCount; i++) {
		if(*(ptr++) != i)
			error("Bad contents!", i);
	}
}

void dumpMemoryBytes(byte* mem, int count, int bytesPerLine) {
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
