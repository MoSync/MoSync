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

#include <maheap.h>
#include <ma.h>
#include "tlsf.h"

#include <mavsprintf.h>
#include <maassert.h>
#include <mastdlib.h>

#if 0
void override_heap_init_crt0(char* start, int length) {
	int res = init_memory_pool(length, start);
	if(res < 0) {
		maPanic(1, "init_memory_pool failed!");
	}
	set_malloc_hook((malloc_hook)tlsf_malloc);
	set_free_hook(tlsf_free);
	set_realloc_hook((realloc_hook)tlsf_realloc);
	
	lprintfln("TLSF initialized!");
}
#endif

static int bytes=0, allocs=0;
static int startTime;

static void fail() {
	lprintfln("fail after %i allocs", allocs);
	lprintfln("%i bytes, %i ms", bytes, maGetMilliSecondCount() - startTime);
}

static void* ptr[6];

static void allocHalf(int index) {
	int i;
	for(i=index; i<index+3; i++) {
		int size = urand() & ((2*1024)-1);
		//lprintfln("size %i", size);
		ptr[i] = malloc(size);
		if(ptr[i] == NULL)
			fail();
		bytes += size;
		allocs++;
	}
}

static void freeHalf(int index) {
	int i;
	for(i=index; i<index+3; i++) {
		free(ptr[i]);
	}
}

static void nil() {}

int MAMain() {
	set_malloc_handler(nil);
	startTime = maGetMilliSecondCount();
	//srand(maTime());
	//urand();
	//urand();
	allocHalf(0);
	while(1) {
		allocHalf(3);
		freeHalf(0);
		allocHalf(0);
		freeHalf(3);
	}
	FREEZE;
}
