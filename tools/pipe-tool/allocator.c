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
//			 Memory Allocation and File Allocation functions
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//****************************************
//
//****************************************

char * Open_File(char *FileName)
{
	char *ThisMem;
	int Length;
	int res;

        if (!FileName || (FileName && !strcmp (FileName, "-")))
          InFile = stdin;
        else
          {
            InFile = fopen(FileName,"rb");
            if (!InFile)
              {
                printf("Error : Problem opening '%s' test file\n",FileName);
                ExitApp(1);
              }
          }

	if (fseek(InFile,0,SEEK_END) != -1)
          {
            Length = ftell(InFile);
            fseek(InFile,0,SEEK_SET);
            if (Length == 0)
            {
		Error(Error_Skip, "File '%s' Has 0 length",FileName);
		ExitApp(1);
              }

            ThisMem = (char *) NewPtrClear((int) Length + 256);

            res = fread(ThisMem, 1, Length, InFile);
	      if(res != Length)
		{
		Error(Error_Skip, "File '%s' could not be read",FileName);
		ExitApp(1);
              }
          }
        else
          {
#define CHUNKSIZE 256            
            int n;
            int pos;
            
            Length = 0;
            pos = 0;
            ThisMem = NULL;
            do
              {
                if ((Length - pos) < CHUNKSIZE)
                  {
                    Length += CHUNKSIZE;
                    ThisMem = ReallocPtr (ThisMem, Length + 1);
                  }
                n = fread (ThisMem + pos, 1, CHUNKSIZE, InFile);
                pos += n;
              }
            while (n > 0 && !feof (InFile));
            if (pos > 0)
              ThisMem[pos] = '\0';
            else
              {
                fprintf (stderr, "Could not read input file\n");
                ExitApp(1);
              }
          }

	fclose(InFile);
	return ThisMem;
}


//****************************************
//
//****************************************

int FileAllocLen = 0;

char * Open_FileAlloc(char *FileName)
{
	char *ThisMem;
	int len,l;

	// Load

	FileAllocLen = 0;

    InFile = fopen(FileName,"rb");

	if (!InFile)
		return 0;
		
	fseek(InFile,0,SEEK_END);
	len = ftell(InFile);
	fseek(InFile,0,SEEK_SET);

	if (len == 0)
		return 0;

	ThisMem = (char *) NewPtrClear((int) len + 256);

	if (!ThisMem)
		return 0;

	l = fread(ThisMem, 1, len, InFile);;

	if (l != len)
	{
		DisposePtr(ThisMem);
		return 0;
	}

	FileAllocLen = len;

	fclose(InFile);
	return ThisMem;
}

//****************************************
//
//****************************************

int FileAlloc_Len()
{
	return FileAllocLen;
}

//****************************************
//			
//****************************************

void Free_File(char *fptr)
{
  DisposePtr(fptr);
}

//******************************************************************************
//				       Basic Memory Management
//******************************************************************************

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


