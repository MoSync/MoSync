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

/** \file maheap.h
* \brief Dynamic memory allocation.
*/

//************************************************************************************************
//							   Memory Allocation Library
//************************************************************************************************

#ifndef MAHEAP_H
#define MAHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ma.h"

#ifndef MAPIP
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <malloc.h>
#else

//************************************************************************************************
//									ANSI memory api
//************************************************************************************************

/** \brief Allocate uninitialized memory.
* Allocates memory from the mosync heap
* \param size The number of bytes to allocate.
* \return Returns a pointer to the allocated memory.
*
* \see calloc
* \see free
* \note The maximum heap size depends on the heapsize setting in the build
*/

void * malloc(int size);

/** \brief Allocate initialized memory.
* Allocates memory from the mosync heap and sets the contents to null.
* \param num_elem The number of elements of memory to allocate.
* \param size_elem The size of each elements of memory.
* \return Returns a pointer to the allocated memory.
*
* \see malloc
* \see free
* \note The maximum heap size depends of the datasize setting in the build.
*/

void * calloc(int num_elem, int size_elem);

/** \brief Deallocate memory.
* Deallocates memory from the mosync heap.
* \param mem A pointer to the memory to deallocate.
*
* \see malloc
* \see calloc
*/

void free(void *mem);

/** \brief Reallocate memory.
* Attempts to resize a previously allocated block of memory.
* If successful, returns a pointer to the resized memory block.
* The pointer may be identical to the \a old, or different.
* Returns NULL on failure, leaving the old memory block untouched.
*
* \param old The old block of memory.
* \param size The requested size.
*/
void* realloc(void* old, int size);


/**
* Returns the total size of the heap, in bytes.
*/
size_t heapTotalMemory(void);

/**
* Returns the amount of available memory on the heap, in bytes.
* \note Due to overhead, you will not be able to allocate exactly this amount.
*/
size_t heapFreeMemory(void);

#endif	//MAPIP

typedef void (*malloc_handler)(int size);
typedef void* (*malloc_hook)(int size);
typedef void (*free_hook)(void* ptr);
typedef void* (*realloc_hook)(void* ptr, int size);
typedef int (*block_size_hook)(void* ptr);

/**
* Calls maPanic().
*/
void ATTRIBUTE(noreturn, default_malloc_handler(int size));

/**
* Sets the function to be called in case malloc() fails.
* Setting this to NULL disables the handler system.
* It is initially set to default_malloc_handler().
* \returns The previous handler.
* \see malloc
*/
malloc_handler set_malloc_handler(malloc_handler);

/**
* Sets a function to be called instead of the standard malloc().
* The malloc_handler system will still work after a hook has been set.
* \param hook A function to allocate memory or return NULL.
* \returns The old hook.
*/
malloc_hook set_malloc_hook(malloc_hook hook);

/**
* Sets a function to be called instead of the standard free().
* \param hook A function to free memory.
* \returns The old hook.
*/
free_hook set_free_hook(free_hook hook);

/**
* Sets a function to be called instead of the standard realloc().
* \param hook A function to increase the size of an existing memory cell.
* \returns The old hook.
*/
realloc_hook set_realloc_hook(realloc_hook hook);

/**
* Sets a function to be called from free to get the size of the block.
* \param hook A function to retrieve the size of a user allocated block.
* \returns The old hook.
*/
block_size_hook set_block_size_hook(block_size_hook hook);

/**
* This function is not implemented. You may implement it. If you do,
* it will be called at the beginning of execution, instead of the standard
* heap initialization function, before C++ static constructors and MAMain.
*
* You must call set_malloc_hook() and set_free_hook() from this function,
* or the heap will be broken and malloc() will not work.
* You must also call set_realloc_hook(), if your program uses realloc().
*
* This function will only be called on the MAPIP GCC target, because the
* optional call to this function relies on a special feature of pipe-tool.
*
* \param start The address of the start of the heap.
* \param length The length of the heap, in bytes.
*/
void override_heap_init_crt0(char* start, int length);

/**
* The default heap initialization function.
* \see override_heap_init_crt0()
*/
void ansi_heap_init_crt0(char *start, int length);

/**
* Starts a system for dumping information about every call to malloc() or free().
* Only available in debug builds.
* It is recommended to call this from override_heap_init_crt0(), in order to
* catch malloc() calls from static constructors.
*/
void initStackDump(void);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* MAHEAP_H */
