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
