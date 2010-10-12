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
//				  			  	Array Class Routines
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#ifdef INCLUDE_ARRAY_CLASS

//****************************************
//				Defines
//****************************************

#define ARRAY_BLOCK_CHUNK	1024*1024				// Set lower for thriftyness with memory
#define ARRAY_LOW_VAL		0x7fffffff

ArrayStore *CurrentArray = 0;

//****************************************
//		  Initialize an array
//****************************************

void ArrayInit(ArrayStore *theArray, uint type, uint size)
{
	theArray->array = 0;
	
	if (type == 0)
		return;

	if (size < 1)
		size = ARRAY_BLOCK_CHUNK;

	theArray->array = (void *) NewPtrClear(type * size);
	theArray->size = size;
	theArray->type = type;
	
	theArray->lo = ARRAY_LOW_VAL;
	theArray->hi = 0;
	theArray->ps = 0;

	if (!theArray->array)
		Error(Error_Fatal, "ArrayClass failed to allocate 1");

}

//****************************************
//		  Initialize an array
//****************************************

void ArrayDispose(ArrayStore *theArray)
{
	if (!theArray->array)
		return;

	DisposePtr((char *) theArray->array);

	theArray->size = 0;
	theArray->type = 0;
	theArray->array = 0;
}

//****************************************
//		  Initialize an array
//****************************************

void ArrayClear(ArrayStore *theArray)
{
	if (!theArray->array)
		return;

	if (theArray->size < 1)
		return;

	memset(theArray->array, 0, theArray->type * theArray->size);

	theArray->ps = 0;
}

//****************************************
//		 Set value in array
//****************************************

void * ArrayPtr(ArrayStore *theArray, uint index)
{
	int bytesToClear;
	char *clearMem;
	char *arrayPtr;
	
	if (!theArray->array)
		return 0;

	if (index >= theArray->size)
	{
		int newsize = index + ARRAY_BLOCK_CHUNK;
	
		theArray->array = (void *) ReallocPtr((char *) theArray->array,  theArray->type * newsize);

		if (!theArray->array)
			Error(Error_Fatal, "ArrayClass failed to reallocate 2");

		// Clear out new memory

		bytesToClear = (newsize - theArray->size) * theArray->type;
		clearMem = (char *) theArray->array + (theArray->size * theArray->type);
		memset(clearMem, 0, bytesToClear);

		// Set the new size

		theArray->size = newsize;	
	}

	if (index > theArray->hi)
		theArray->hi = index;

	if (index < theArray->lo)
		theArray->lo = index;

	arrayPtr = (char *) theArray->array + (theArray->type * index);
	return (void *) arrayPtr;
}

//****************************************
//		 Get a pointer to an array
//    make sure the array is valid for
//	 indices between start & end index
//****************************************

void * ArrayPtrBound(ArrayStore *theArray, int startIndex, uint endIndex)
{
	int bytesToClear;
	char *clearMem;
	char *arrayPtr;
	
	if (!theArray->array)
		return 0;

	if (endIndex >= theArray->size)
	{
		int newsize = endIndex + ARRAY_BLOCK_CHUNK;
	
		theArray->array = (void *) ReallocPtr((char *) theArray->array,  theArray->type * newsize);

		if (!theArray->array)
			Error(Error_Fatal, "ArrayClass failed to reallocate 3");

		// Clear out new memory

		bytesToClear = (newsize - theArray->size) * theArray->type;
		clearMem = (char *) theArray->array + (theArray->size * theArray->type);
		memset(clearMem, 0, bytesToClear);

		// Set the new size

		theArray->size = newsize;	
	}

	if (endIndex > theArray->hi)
		theArray->hi = endIndex;

	if (endIndex < theArray->lo)
		theArray->lo = endIndex;

	arrayPtr = (char *) theArray->array + (theArray->type * startIndex);
	return (void *) arrayPtr;
}


//****************************************
//		 Set value in array
//****************************************

void ArraySet(ArrayStore *theArray, uint index, uint value)
{
	int bytesToClear;
	char *clearMem;

	if (!theArray->array)
		return;

	if (index >= theArray->size)
	{
		int newsize = index + ARRAY_BLOCK_CHUNK;
	
		theArray->array = (void *) ReallocPtr((char *) theArray->array,  theArray->type * newsize);

		if (!theArray->array)
			Error(Error_Fatal, "ArrayClass failed to reallocate 4");

		// Clear out new memory

		bytesToClear = (newsize - theArray->size) * theArray->type;
		clearMem = (char *) theArray->array + (theArray->size * theArray->type);
		memset(clearMem, 0, bytesToClear);

		// Set the new size

		theArray->size = newsize;	
	}

	switch(theArray->type)
	{
		case 1:
			((uchar *)theArray->array)[index] = value;
		break;

		case 2:
			((ushort *)theArray->array)[index] = value;
		break;

		case 4:
		{
			//int *tab = theArray->array;
			((uint *)theArray->array)[index] = value;
		}
		break;
	}
	
	if (index > theArray->hi)
		theArray->hi = index;

	if (index < theArray->lo)
		theArray->lo = index;
}

//****************************************
//		Set value in array auto-inc
//****************************************

void ArrayAppend(ArrayStore *theArray, uint value)
{
	ArraySet(theArray, theArray->ps, value);
	theArray->ps++;
}

//****************************************
//		Set value in array auto-inc
//****************************************

void ArraySetPosition(ArrayStore *theArray, uint pos)
{
	theArray->ps = pos;
}

//****************************************
//		Set value in array auto-inc
//****************************************

int ArrayGetPosition(ArrayStore *theArray)
{
	return theArray->ps;
}

//****************************************
//		 Get value from array
//****************************************

uint ArrayGet(ArrayStore *theArray, uint index)
{
	if (!theArray->array)
		return 0;
		
	if (index >= theArray->size)
		return 0;
	
	switch(theArray->type)
	{
		case 1:
			return ((uchar *)theArray->array)[index];

		case 2:
			return ((ushort *)theArray->array)[index];

		case 4:
		{
			//int *tab = theArray->array;
			return ((uint *)theArray->array)[index];
		}
	}

	return 0;
}

//****************************************
//	  Search for a value in an array
//  return an index or -1 if not found
//****************************************

void ArrayCopy(ArrayStore *dstArray, ArrayStore *srcArray)
{
	uint n;

	if (dstArray->array == srcArray->array)
		return;

	if (!dstArray->array)
		return;

	if (!srcArray->array)
		return;

// !! Fix ARH 21/09/10 - srcArray->lo is incorrect should be 0 !!

	for (n=0;n<srcArray->hi+1;n++)

	{
		ArraySet(dstArray, n, ArrayGet(srcArray, n));
	}

	dstArray->ps = srcArray->ps;
	return;
}

//****************************************
//	  Search for a value in an array
//  return an index or -1 if not found
//****************************************

int ArraySearch(ArrayStore *theArray, uint value)
{
	uint n;
	
	if (!theArray->array)
		return 0;

// !! Fix ARH 21/09/10 - theArray->lo is incorrect should be 0 !!

	for (n=0;n<theArray->hi+1;n++)
	{
		if (value == ArrayGet(theArray, n))
			return n;
	}

	return -1;	
}

//****************************************
//	 Write an array to a file
//****************************************

char * ArrayFileInfo(ArrayStore *theArray, int *len)
{	
	if (!theArray->array)
		return 0;

	*len = theArray->type * theArray->hi;
	return (char *) theArray->array;
}

//****************************************
//	 Write an array to a file
//****************************************

int ArrayWrite(ArrayStore *theArray, char *filename)
{
	FILE *theFile;
	size_t len;
	
	if (!theArray->array)
		return 0;

	theFile = fopen(filename, "wb");
	
	if (!theFile)
		return 0;

	len = theArray->type * theArray->hi;

	if (fwrite(theArray->array, 1, len, theFile) != len)
	{
		fclose(theFile);
		return 0;	
	}

	fclose(theFile);
	return len;	
}

//****************************************
//	 Write an array to a file
//****************************************

int ArrayWriteFP(ArrayStore *theArray, FILE *theFile, size_t len)
{
	if (!theArray->array)
		return 0;

	if (!theFile)
		return 0;

	if (!len)
		len = theArray->type * theArray->hi;

	if (fwrite(theArray->array, 1, len, theFile) != len)
		return 0;	

	return len;	
}

//****************************************
//
//****************************************

void ArrayPrint(ArrayStore *theArray, char *Template, ...)
{
	char 			tbuf[1024];
	va_list 		args;
	int 			len;
	int				n;
		
	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);
		
	len = strlen(tbuf);

	for (n=0;n<len;n++)
		ArrayAppend(theArray, tbuf[n]);

	return;
}

//****************************************
// 	 state based array functions
//****************************************

void arraySelect(ArrayStore *theArray)
{
	CurrentArray = theArray->array;
}

uint arrayGet(int index)
{
	return ArrayGet(CurrentArray, index);
}

void arraySet(int index, uint val)
{
	ArraySet(CurrentArray, index, val);
}

void arrayAppend(uint val)
{
	ArrayAppend(CurrentArray, val);
}


//****************************************
//			Test Code
//****************************************
/*
void ArrayPrint(ArrayStore *theArray, char *heading)
{
	int n;

	printf("\n%s\n\n", heading);

	if (!theArray->array)
	{
		printf("array is null\n");
		return;
	}
	
	for (n=0;n<theArray->hi+1;n++)
	{
		SYMBOL *sym = (SYMBOL *) ArrayGet(theArray, n);

		if (sym)
		{
			printf("%d: %d 0x%x\n", n, sym, sym);
			DumpEnumEntry(sym);

		}
	}
}
*/
//****************************************
//			Test Code
//****************************************

/*

void ArrayTest()
{
	ArrayStore	test;
	int n;
		
	ArrayInit(&test, 0);

	//ArraySet(&test, 0, 99);

	for (n=10;n<40;n++)
	{
		if (n < 30)
			ArraySet(&test, n, (n+1));

		printf("%d: %d --> Size %d\n", n, ArrayGet(&test, n), test.size );
	}
	
	printf("low %d, hi %d\n", test.lo, test.hi );
	
}

*/

//****************************************

#endif

