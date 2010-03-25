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
	char *FPtr = g_FilePtr;				// Preserve file pointer
	char *FTop = g_FileTop;				// Preserve file pointer

	SetError_File(config_path);
	SetError_Mode(ErrorMode_Basic);
	
	g_FileTop = g_FilePtr = filemem;

	EndComp = 0;

	ResetErrorCount();
	SetErrorReturn();

	while(1)
	{	
		if (EndComp)
			break;

		if (*g_FilePtr == 0)
			break;
	
		if (*g_FilePtr == 0)
			break;
	
		if (ProfileCommands())
			continue;
	}

	g_FilePtr = FPtr;
	g_FileTop = FTop;

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

		PushTokenPtr(g_FilePtr, 1);		// Save Token Scanner Context
		GetAsmName();
		strcpy(DefineNameCopy, g_Name);
		PopTokenPtr();

		if (QToken("MA_PROF_"))
		{

			if (QToken("CONST_"))
			{
				GetAsmName();
				strcpy(NameCopy, g_Name);
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
	


