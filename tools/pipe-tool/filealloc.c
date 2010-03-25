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
          g_InFile = stdin;
        else
          {
            g_InFile = fopen(FileName,"rb");
            if (!g_InFile)
              {
                printf("Error : Problem opening '%s' test file\n",FileName);
                ExitApp(1);
              }
          }

	if (fseek(g_InFile,0,SEEK_END) != -1)
          {
            Length = ftell(g_InFile);
            fseek(g_InFile,0,SEEK_SET);
            if (Length == 0)
            {
		Error(Error_Skip, "File '%s' Has 0 length",FileName);
		ExitApp(1);
              }

            ThisMem = (char *) NewPtrClear((int) Length + 256);

            res = fread(ThisMem, 1, Length, g_InFile);
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
                n = fread (ThisMem + pos, 1, CHUNKSIZE, g_InFile);
                pos += n;
              }
            while (n > 0 && !feof (g_InFile));
            if (pos > 0)
              ThisMem[pos] = '\0';
            else
              {
                fprintf (stderr, "Could not read input file\n");
                ExitApp(1);
              }
          }

	fclose(g_InFile);
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

    g_InFile = fopen(FileName,"rb");

	if (!g_InFile)
		return 0;
		
	fseek(g_InFile,0,SEEK_END);
	len = ftell(g_InFile);
	fseek(g_InFile,0,SEEK_SET);

	if (len == 0)
		return 0;

	ThisMem = (char *) NewPtrClear((int) len + 256);

	if (!ThisMem)
		return 0;

	l = fread(ThisMem, 1, len, g_InFile);;

	if (l != len)
	{
		DisposePtr(ThisMem);
		return 0;
	}

	FileAllocLen = len;

	fclose(g_InFile);
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
