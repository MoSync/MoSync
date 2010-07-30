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

//*********************************************************************************************
//			 				Memory Allocation functions
// 						  	  Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//***************************************
// 		Configuration settings
//***************************************

//#define OLD_MEM_MANAGEMENT
#define NEW_MEM_MANAGEMENT

//******************************************************************************
//				        Old Basic Memory Management
//******************************************************************************

#ifdef OLD_MEM_MANAGEMENT

#define MAXALLOCS 16384 * 16

char *allocList[MAXALLOCS];
int allocStack[MAXALLOCS];
int allocSP = 0;
int allocCount = 0;

//***************************************
//
//***************************************

void InitMasterAlloc()
{
	int n;
	
	allocSP = 0;
	allocCount = 0;

	memset(allocList, 0, sizeof(allocList));

	for (n=0;n<MAXALLOCS;n++)
		allocStack[n] = n;
}

//***************************************
//
//***************************************

void DisposeMasterAlloc()
{
	char *ptr;
	int n;
	
	for (n=0;n<MAXALLOCS;n++)
	{
		ptr = allocList[n];

		if (ptr)
		{
			free(ptr);
			allocList[n] = 0;
		}
	}
}

//***************************************
//		Find Free Master Entry
//***************************************

int FindFreeMaster()
{
	char *ptr;
	int n;
	
	allocCount++;
	
	if (allocCount >= MAXALLOCS)
		Error(Error_Fatal, "MasterAlloc table is full");

	n = allocStack[allocSP++];

	ptr = allocList[n];

	if (ptr)
		Error(Error_Fatal, "(FindFreeMaster) alloc stack entry was not null");

	return n;	
}


//***************************************
//		Find Free Master Entry
//***************************************

void ReleaseFreeMaster(int i)
{
 	// make sure the index is within bounds
 	
 	if ((i < 0) || (i >= MAXALLOCS) )
 	{ 
 		Error(Error_Fatal, "Fatal memory error in DisposePtr : master entry is out of range (%d)\n", i);
 		return;
 	}

	// Clear the master alloc entry

	allocList[i] = 0;

	allocStack[--allocSP] = i;
	allocCount--;

	if (allocCount < 0)
		Error(Error_Fatal, "MasterAlloc table underflow");
	
	return;
}

//***************************************
//
//***************************************

char *NewPtr(int size)
{
	char *ptr;
	int	 *rptr;
	int	 entry;
	
	// Look for a master entry in the alloc table
	
	entry = FindFreeMaster();

	// If no entry availble then return
	
	if (entry == -1)
		return 0;

	// allocate the required memory, add an int so that we can store
	// a reference to the master entry

	ptr = (char *) malloc(size + 4);
	
	// if no mem, return
	
	if (!ptr)
		return 0;

	// put the master alloc index into the memory block,
	// then we get fast deallocation

	rptr = (int *) ptr;
	*rptr = entry;
	
	// put the memory into the master alloc table
	
	allocList[entry] = ptr;

	// finally return the allocated memory to the user,
	// advanced by 4 bytes

	return ptr + 4;
}

//***************************************
//
//***************************************

char *NewPtrClear(int size)
{
	char *ptr = NewPtr(size);

	if (!ptr)
		return 0;

	// Clear the memory
	
	memset(ptr, 0, size);
	return ptr;
}

//***************************************
//
//***************************************

void DisposePtr(char *thisPtr)
{
	int *master;
 	int i;
 
 	// Make sure memory is not null
 	
 	if (!thisPtr)
 		return;
 
 	// get a ptr to the master index for this memory
 	master = (int *)(thisPtr - 4);
 	i = *master;
 
	ReleaseFreeMaster(i);
	
	// free up the memory
	
	free((char *) master);
}

//***************************************
//
//***************************************

char *ReallocPtr(char *thisPtr, int size)
{
	char *newptr;
	int *master;
	int i;

 	// Make sure memory is not null
 	
 	if (!thisPtr)
 		return 0;
 
 	// get a ptr to the master index for this memory

 	master = (int *)(thisPtr - 4);
 	i = *master;
 
 	// make sure the index is within bounds
 	
 	if ((i < 0) || (i >= MAXALLOCS) )
 	{ 
 		Error(Error_Fatal, "Fatal memory error in ReallocPtr : master entry is out of range (%d)\n", i);
 		return 0;
 	}

	newptr = (char *) realloc((void *) master, size + 4);

  	if (!newptr)
		return 0;
	
	// Return the ptr

	allocList[i] = newptr;
	return newptr + 4;
}

//***************************************
//		End of Old Memory Manager
//***************************************

#endif

//******************************************************************************
//				        New Basic Memory Management
//******************************************************************************

#ifdef NEW_MEM_MANAGEMENT

ArrayStore allocList;
ArrayStore allocStack;
int allocSP = 0;
int allocCount = 0;

//***************************************
//
//***************************************

void InitMasterAlloc()
{
	allocSP = 0;
	allocCount = 0;

	ArrayInit(&allocList, sizeof(int), 0);
	ArrayInit(&allocStack, sizeof(int), 0);

	allocSP = 0;
	allocCount = 0;
}

//***************************************
//
//***************************************

void DisposeMasterAlloc()
{
	char *ptr;
	int n;
	
	for (n=0;n<allocCount;n++)
	{
		ptr = (char *) ArrayGet(&allocList, n);

		if (ptr)
		{
			free(ptr);
			ArraySet(&allocList, n, 0);
		}
	}

//	printf("allocCount = %d : allocSP = %d\n", allocCount ,allocSP);
}

//***************************************
//		Find Free Master Entry
//***************************************

int FindFreeMaster()
{
	int n;

	// Check if there is a free index on the stack
	
	if (allocSP > 0)
	{
		n = ArrayGet(&allocStack, --allocSP);
		return n;
	}

	// Just mask a new alloc index then

	n = allocCount++;
	ArraySet(&allocList, n, 0);		// Set the new entry to 0
	return n;	
}


//***************************************
//		Release Master Entry
//***************************************

void ReleaseFreeMaster(int i)
{
	// Clear the master alloc entry

	ArraySet(&allocList, i, 0);
	
	// Save this index to the alloc stack
	
	ArraySet(&allocStack, allocSP++, i);
	return;
}

//***************************************
//
//***************************************

char *NewPtr(int size)
{
	char *ptr;
	int	 *rptr;
	int	 entry;
	
	// Look for a master entry in the alloc table
	
	entry = FindFreeMaster();

	// If no entry availble then return
	
	if (entry == -1)
		return 0;

	// allocate the required memory, add an int so that we can store
	// a reference to the master entry

	ptr = (char *) malloc(size + 4);
	
	// if no mem, return
	
	if (!ptr)
		return 0;

	// put the master alloc index into the memory block,
	// then we get fast deallocation

	rptr = (int *) ptr;
	*rptr = entry;
	
	// put the memory into the master alloc table
	
	ArraySet(&allocList, entry, (int) ptr);

	// finally return the allocated memory to the user,
	// advanced by 4 bytes

	return ptr + 4;
}

//***************************************
//
//***************************************

char *NewPtrClear(int size)
{
	char *ptr = NewPtr(size);

	if (!ptr)
		return 0;

	// Clear the memory
	
	memset(ptr, 0, size);
	return ptr;
}

//***************************************
//
//***************************************

void DisposePtr(char *thisPtr)
{
	int *master;
 	int i;
 
 	// Make sure memory is not null
 	
 	if (!thisPtr)
 		return;
 
 	// get a ptr to the master index for this memory
 	master = (int *)(thisPtr - 4);
 	i = *master;
 
	ReleaseFreeMaster(i);
	
	// free up the memory
	
	free((char *) master);
}

//***************************************
//
//***************************************

char *ReallocPtr(char *thisPtr, int size)
{
	char *newptr;
	int *master;
	int i;

 	// Make sure memory is not null
 	
 	if (!thisPtr)
 		return 0;
 
 	// get a ptr to the master index for this memory

 	master = (int *)(thisPtr - 4);
 	i = *master;
  	
	newptr = (char *) realloc((void *) master, size + 4);

  	if (!newptr)
		return 0;
	
	// Return the ptr
	
	ArraySet(&allocList, i, (int) newptr);
	return newptr + 4;
}

//***************************************
//		End of New Memory Manager
//***************************************

#endif

