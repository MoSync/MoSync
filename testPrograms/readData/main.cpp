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
