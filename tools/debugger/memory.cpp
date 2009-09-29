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

#include "memory.h"
#include "helpers.h"

#include "config.h"
#include "helpers/helpers.h"

int gMemSize = 0;
char* gMemBuf = NULL;

#define CACHED_MEM_BITS_SIZE ((sizeof(int)*gMemSize*8+32)>>5)
static int *sCachedMemBits = NULL;

//******************************************************************************
// init
//******************************************************************************

void setMemSize(int size) {
	gMemSize = size;
	SAFE_DELETE(gMemBuf);
	gMemBuf = new char[size];
	sCachedMemBits = new int[CACHED_MEM_BITS_SIZE];
	clearMemoryCacheBits();
}

void clearMemoryCacheBits() {
	memset(sCachedMemBits, 0, CACHED_MEM_BITS_SIZE);
}

bool isMemoryCached(int src, int len) {
	int numSet = 0;
	for(int i = src; i < src+len; i++) {
		int j_hi = i>>5;
		int j_lo = 1<<(i&((1<<5)-1));
		if(!(sCachedMemBits[j_hi]&j_lo)) {
			sCachedMemBits[j_hi]|=j_lo;
		} else {
			numSet++;
		}
	}
	if(numSet==len) return true;
	else return false;
}
