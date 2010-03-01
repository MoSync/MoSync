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

#include "ma.h"
#include "maheap.h"
#include "stdio.h"

malloc_handler gMallocHandler = default_malloc_handler;
malloc_hook gMallocHook = NULL;
free_hook gFreeHook = NULL;
realloc_hook gReallocHook = NULL;
block_size_hook gBlockSizeHook = NULL;

#ifdef MOSYNCDEBUG
//#define MEMORY_PROTECTION
int gUsedMem = 0;
int gNumMallocs = 0, gNumFrees = 0;
#endif

void default_malloc_handler(int size) {
#ifdef MOSYNCDEBUG
	lprintfln("um %i", gUsedMem);
	lprintfln("nm %i, nf %i", gNumMallocs, gNumFrees);
#endif
	maPanic(size, "malloc failed");
}

malloc_handler set_malloc_handler(malloc_handler new) {
	malloc_handler temp = gMallocHandler;
	gMallocHandler = new;
	return temp;
}

malloc_hook set_malloc_hook(malloc_hook new) {
	malloc_hook temp = gMallocHook;
	gMallocHook = new;
	return temp;
}

realloc_hook set_realloc_hook(realloc_hook new) {
	realloc_hook temp = gReallocHook;
	gReallocHook = new;
	return temp;
}

free_hook set_free_hook(free_hook new) {
	free_hook temp = gFreeHook;
	gFreeHook = new;
	return temp;
}

block_size_hook set_block_size_hook(block_size_hook new) {
	block_size_hook temp = gBlockSizeHook;
	gBlockSizeHook = new;
	return temp;
}

#ifdef MAPIP

#include "tlsf.h"

//#define MASTD_HEAP_LOGGING

#ifdef MASTD_HEAP_LOGGING
#include "conprint.h"
#define MASTD_HEAP_LOG lprintfln
#else
#define MASTD_HEAP_LOG(...)
#endif

void ansi_heap_init_crt0(char *start, int length);

//****************************************
//				NewPtr
//****************************************

void ansi_heap_init_crt0(char *start, int length)
{
	int res;
	if(maCheckInterfaceVersion(MAIDL_HASH) != (int)MAIDL_HASH) {
		maPanic(1, "Interface version mismatch!");
	}

	MASTD_HEAP_LOG("heap: start 0x%08x len 0x%x", start, length);

	if(length <= 0)
		return;
	res = init_memory_pool(length, start);
	if(res < 0) {
		maPanic(1, "init_memory_pool failed!");
	}
	set_malloc_hook((malloc_hook)tlsf_malloc);
	set_free_hook(tlsf_free);
	set_realloc_hook((realloc_hook)tlsf_realloc);
	set_block_size_hook((block_size_hook)tlsf_block_size);
		
	MASTD_HEAP_LOG("TLSF initialized!");
}

//****************************************
//				malloc
//****************************************

void * malloc(int size)
{
	void *result;

#ifdef MEMORY_PROTECTION
	int wasMemoryProtected = maGetMemoryProtection();
	maSetMemoryProtection(FALSE);
#endif

	result = gMallocHook(size);

	if(result == 0)
	{
		if (!gMallocHandler)
			return 0;
		gMallocHandler(size);
		result = gMallocHook(size);
	}
#ifdef MOSYNCDEBUG
	gNumMallocs++;
	gUsedMem += size;
#endif

#ifdef MEMORY_PROTECTION
	maUnprotectMemory((byte*)result, size);
	maSetMemoryProtection(wasMemoryProtected);
#endif
	MASTD_HEAP_LOG("malloc(%i) pointer (0x%08X)\n", size,result);

	return result;
}

//****************************************
//				calloc
//****************************************

void * calloc(int num_elem, int size_elem)
{
	int size = num_elem * size_elem;
	void *mem;

	MASTD_HEAP_LOG("calloc(%i, %i)\n", num_elem, size_elem);
	mem = (void *) malloc(size);

	if (mem == 0)
		return mem;

	memset(mem, 0, size);			// Do initial clear
	return mem;
}

//****************************************
//				free
//****************************************

void free(void *mem)
{
	MASTD_HEAP_LOG("free(0x%08X)\n", (int)mem);

	if (!mem)
		return;

#ifdef MEMORY_PROTECTION
	int wasMemoryProtected = maGetMemoryProtection();
	maSetMemoryProtection(FALSE);
	if(gBlockSizeHook)
		maProtectMemory(mem, gBlockSizeHook(mem));	
#endif

	gFreeHook(mem);
	
#ifdef MEMORY_PROTECTION
	maSetMemoryProtection(wasMemoryProtected);
#endif
	
}

//****************************************
//				fugly realloc
//****************************************
void* realloc(void* old, int size) {
	MASTD_HEAP_LOG("realloc(0x%08X, %i)\n", (int)old, size);

	return gReallocHook(old, size);
}

#endif /* MAPIP */
