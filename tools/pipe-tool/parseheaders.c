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
	
	SET_ERROR_RETURN();

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

		if (NextToken("#"))
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
	


