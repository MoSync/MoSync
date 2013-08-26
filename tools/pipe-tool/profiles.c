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
//				       	  Profile Reader Routines
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#define profprint		if (0) printf

//****************************************
//			 Assemble code
//****************************************

extern short EndComp;

void ProfileReader(char *filemem, char *config_path)
{
	char *FPtr = FilePtr;				// Preserve file pointer
	char *FTop = FileTop;				// Preserve file pointer

	SetError_File(config_path);
	SetError_Mode(ErrorMode_Basic);
	
	FileTop = FilePtr = filemem;

	EndComp = 0;

	ResetErrorCount();
	
	SET_ERROR_RETURN();

	while(1)
	{	
		if (EndComp)
			break;

		if (*FilePtr == 0)
			break;
	
		if (*FilePtr == 0)
			break;
	
		if (ProfileCommands())
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

char NameCopy[256];
char DefineNameCopy[256];

short ProfileCommands()
{	
	int v;

	SkipWhiteSpace();

//---------------------------------------------------------

//------------------------------------
//
//------------------------------------

	if (QToken("#define"))
	{
		SkipWhiteSpace();

		PushTokenPtr(FilePtr, 1);		// Save Token Scanner Context
		GetAsmName();
		strcpy(DefineNameCopy, Name);
		PopTokenPtr();

		if (QToken("MA_PROF_"))
		{

			if (QToken("CONST_"))
			{
				GetAsmName();
				strcpy(NameCopy, Name);
				SkipWhiteSpace();

				if (QToken("DYNAMIC"))
				{
					v = 1024 * 1024 * 16;
				}
				else
				{
					v = GetExpression();
				}

				RedefENum(DefineNameCopy, v);	
	
				profprint("Profile: const found '%s' = %d\n", DefineNameCopy, v);
				return 1;
			}
			
			GetAsmName();

			RedefENum(DefineNameCopy, 1);	
	
			profprint("Profile: found '%s'\n", DefineNameCopy);
			return 1;
		}
		
		profprint("Profile: unknown define '%s'\n", DefineNameCopy);
		return 1;
	}

//------------------------------------
// Line not understood, skip line
//------------------------------------

	SkipLine();

	return 0;
}
	


