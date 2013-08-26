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
