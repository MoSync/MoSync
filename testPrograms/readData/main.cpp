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

#include "maheaders.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SIZE 32

void testRes(Handle resource);
void testSingle(Handle res, int addr, int offset, int count);
void checkBounds(byte* buffer, int addr, int count);
void checkContents(byte* ptr, int offset, int count);
void error(const char* msg, int extra);

extern "C" int MAMain() {
	//have maybe 32 bytes of resource, bin and ubin
	//have the resource be incremented bytes, 0 to 31
	//have oversized buffer to read to
	//test reading both types of resource, to addresses and from offsets of all alignments, in blocks of maybe 0 to 16 bytes
	//check (A) that memory outside the designated read area is not changed and (B) that the data is what it should be.
	
	testRes(1);
	testRes(2);
	
	static char endMsg[] = "Test successful.\n";
	maWriteLog(endMsg, sizeof(endMsg) - 1);

	return 0;
}

int gResource;
int gAddressAlignment;
int gOffsetAlignment;
int gCount;
byte gBuffer[SIZE * 2];

void testRes(Handle resource) {
	gResource = resource;
	for(gAddressAlignment = 0; gAddressAlignment <= 5; gAddressAlignment++) {
		for(gOffsetAlignment = 0; gOffsetAlignment <= 5; gOffsetAlignment++) {
			for(gCount = 0; gCount <= SIZE / 2; gCount++) {
				testSingle(gResource, gOffsetAlignment, gOffsetAlignment, gCount);
			}
		}
	}
}

void testSingle(Handle res, int addr, int offset, int count) {
	memset(gBuffer, 0xFF, sizeof(gBuffer));	//assumes memset works
#define BASE_OFFSET 4
	maReadData(res, gBuffer + BASE_OFFSET + addr, offset, count);
	checkBounds(gBuffer, addr, count);
	checkContents(gBuffer + BASE_OFFSET + addr, offset, count);
}

void checkBounds(byte* buffer, int addr, int count) {
	//check left bound
	for(int i=0; i<BASE_OFFSET + addr; i++) {
		if(buffer[i] != 0xFF)
			error("Left bound overwritten!", i);
	}
	//check right bound
	for(int i=BASE_OFFSET + addr + count; i<BASE_OFFSET + addr + count + 4; i++) {
		if(buffer[i] != 0xFF)
			error("Right bound overwritten!", i);
	}
}

void checkContents(byte* ptr, int offset, int count) {
	for(int i=offset; i<offset+count; i++) {
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
	lprintfln("%i %i %i %i  %i", gResource, gAddressAlignment, gOffsetAlignment, gCount, extra);
	dumpMemoryBytes(gBuffer, SIZE, 16);
	maPanic(1, msg);
}
