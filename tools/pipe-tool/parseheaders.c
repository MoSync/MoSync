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
//				       	  Header Reader Routines
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#define rhprint		if (0) printf


//****************************************
//	    Read and Parse H Files
//****************************************

void ReadAndParseHeaders(char *filename)
{
	char *file = Open_FileAlloc(filename);

	if (!file)
		return;

	HeaderReader(file);
	Free_File(file);
}

//****************************************
//			 Parse H Files
//****************************************

short hEndComp;

void HeaderReader(char *filemem)
{
	char *FPtr = FilePtr;				// Preserve file pointer
	char *FTop = FileTop;				// Preserve file pointer

	SetError_Mode(ErrorMode_Basic);
	
	FileTop = FilePtr = filemem;

	hEndComp = 0;

	ResetErrorCount();
	SetErrorReturn();

	while(1)
	{	
		if (hEndComp)
			break;

		if (*FilePtr == 0)
			break;
	
		if (*FilePtr == 0)
			break;
	
		if (HeaderCommands())
			continue;
	}

	FilePtr = FPtr;
	FileTop = FTop;

	ExitErrorCount();
	SetErrorReset();

	SetError_Mode(ErrorMode_Full);
}

//****************************************
//
//****************************************

extern char NameCopy[256];
extern char DefineNameCopy[256];

short HeaderCommands()
{	
	int v;

	SkipWhiteSpace();

//------------------------------------
//
//------------------------------------

	if (QToken("#define"))
	{
		SkipWhiteSpace();

		GetAsmName();
		strcpy(DefineNameCopy, Name);

		if (QToken("#"))
		{
			// Just define a script
			
			RedefENum(DefineNameCopy, 1);	
			rhprint("HeaderRead: .set %s = 1\n", DefineNameCopy);

			SkipWhiteSpace();
			return 1;
		}

		// Test for strings
		
		if (QToken("\""))
		{
			GetStringName(128);
			RedefENumString(DefineNameCopy, Name);
			printf("HeaderRead: .set %s = '%s'\n", DefineNameCopy, Name);

			SkipWhiteSpace();
			return 1;
		}
		
		v = GetExpression();
		RedefENum(DefineNameCopy, v);	

		SkipWhiteSpace();
		
		rhprint("Profile: unknown define '%s'\n", DefineNameCopy);
		return 1;
	}

//------------------------------------
// Line not understood, skip line
//------------------------------------

	SkipLine();

	return 0;
}
	


