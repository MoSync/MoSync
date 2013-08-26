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

//*********************************************************************************************
//			 			 File Allocation functions
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
