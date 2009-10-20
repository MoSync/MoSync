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
//				  		PIP-e II Assembler Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

//#define VERBOSE_ERRORS
#define ERRDEBUG

//****************************************
//
//****************************************

void SetError_Mode(int mode)
{
	ErrorMode = mode;
}

//****************************************
//
//****************************************

char error_src_file_name[1024] = "";

void SetError_File(char *file)
{
	strcpy(error_src_file_name, file);
}

//****************************************
//
//****************************************

void ResetErrorCount()
{
	ErrorCount = 0;
}

//****************************************
//
//****************************************

void ExitErrorCount()
{
	if (ErrorCount)
		ExitApp(1);
}


//****************************************
//
//****************************************

void VerboseError(char *tokenptr)
{
	int y;
	unsigned char c;
	
	for(y=0;y<5;)
	{
		if (FilePtr == FileTop)
			break;

		c = *--FilePtr;

		if (c == 0) break;
		if (c == 10) y++;
	}

	printf("\nDebugging Error System\n\n");
	
	printf("%03d: ",GetLineNumber(FileTop,FilePtr));

	for(y=0;y<10;)
	{
	
		if (FilePtr == tokenptr)
			printf("<ERROR>");

		c = *FilePtr++;

		if (c == 0) break;
		if (c == 10)
		{
			y++;
			printf("\n");

		}
		else
			if (c != 13)
				printf("%c",c);
		
		if (c == 10)
			printf("%03d: ",GetLineNumber(FileTop,FilePtr));

	}

	printf("\n");
}


//****************************************
//
//****************************************

void PrintErrorMessage(int flag, char *errorString)
{
//	.sourcedir '/cygdrive/c/mosync/Ye Olde/execute/'
//	.sourcefile 'source\980223.c'

	char *tokenptr = GetPrevFilePtr();

	char *sourcedir = FindDirectiveAbove(FilePtr, ".sourcedir");
	char *sourcefile = FindDirectiveAbove(FilePtr, ".sourcefile");
	char *sourceline = FindDirectiveAbove(FilePtr, ".line");
	char *asmfile = FindDirectiveAbove(FilePtr, ".lfile");

	char SourceFileName[512] = "";
	char SourceDirName[512] = "";
	char AsmFileName[512] = "";
	int	SourceLine = 0;
	int	AsmLine = 0;
	int	DumpLine = 0;

	if (flag == Error_System)
	{
		printf("System Error: %s", errorString);
		return;
	}

	DumpLine = GetLineNumber(FileTop, FilePtr);
	
	if (asmfile)
	{
		PushTokenPtr(asmfile, 1);		// Save Token Scanner Context
		
		NeedToken(".lfile");
		SkipWhiteSpace();
		GetLFileName();	
		strcpy(AsmFileName, Name);
		
		PopTokenPtr();

		AsmLine = GetLineNumber(asmfile, FilePtr);
	}


	if (sourcefile)
	{
		PushTokenPtr(sourcefile, 1);		// Save Token Scanner Context
		
		NeedToken(".sourcefile");
		SkipWhiteSpace();
		GetLFileName();	
		strcpy(SourceFileName, Name);
		
		PopTokenPtr();
	}

	if (sourcedir)
	{
		PushTokenPtr(sourcedir, 1);		// Save Token Scanner Context
		
		NeedToken(".sourcedir");
		SkipWhiteSpace();
		GetLFileName();	
		strcpy(SourceDirName, Name);
		
		PopTokenPtr();
	}

	if (sourceline)
	{
		PushTokenPtr(sourceline, 1);
		
		NeedToken(".line");
		SkipWhiteSpace();
		SourceLine = GetNum();	
		
		PopTokenPtr();	
	}

	if (xerr)
	{
		printf("ErrorDebugInfo\n\n");
		printf("AsmFileName    = %-50s (%x)\n", AsmFileName, (int)AsmFileName);
		printf("SourceFileName = %-50s (%x)\n", SourceFileName, (int)SourceFileName);
		printf("SourceDirName  = %-50s (%x)\n", SourceDirName, (int)SourceDirName);
		printf("SourceLine     = %d\n", SourceLine);
		printf("AsmLine        = %d\n", AsmLine);
		printf("DumpLine       = %d\n\n", DumpLine);
	}

	// Make sure we have both file/line info

//	if ((sourcefile && sourceline) && (sourcefile < asmfile) && (sourceline < sourcefile))

	if (sourcefile && sourceline)
	{
		printf("%s:%d: ", SourceFileName, SourceLine);
	}
	else
	{
//		printf("%s:%d: ", AsmFileName, AsmLine-1);
		printf("In File '%s': ", AsmFileName);
	}

	if (flag == Error_Warning)
		printf("Warning: %s, ", errorString);
	else
		printf("Error: %s, ", errorString);

	printf("\n");

	if (xerr)
	{
		printf("_masterdump.s:%d: Extended error report", DumpLine);
		printf("\n");
		
		VerboseError(tokenptr);
	}
}

//****************************************
//
//****************************************

void BasicErrorMessage(int flag, char *errorString)
{
	char *tokenptr = GetPrevFilePtr();
	int	DumpLine = 0;

	if (flag == Error_System)
	{
		printf("System Error: %s", errorString);
		return;
	}

	DumpLine = GetLineNumber(FileTop, FilePtr);
	
	printf("%s:%d: ", error_src_file_name, DumpLine);

	if (flag == Error_Warning)
		printf("Warning: %s ", errorString);
	else
		printf("Error: %s ", errorString);

	printf("\n");

	if (xerr)
	{
		printf("_masterdump.s:%d: Extended error report", DumpLine);
		printf("\n");
		
		VerboseError(tokenptr);
	}
}

//****************************************
//
//****************************************

void Error(int flag, char *Template, ...)
{
	char 	tbuf[1280];
	va_list	args;
	char *DebegFilePtr = FilePtr;
	
	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	if (ErrorMode == ErrorMode_Basic)
		BasicErrorMessage(flag, tbuf);
	else
		PrintErrorMessage(flag, tbuf);

	if (flag == Error_Warning)
		return;

	if (ExpIsPure)
		ExitApp(1);

	if (ErrorCount++ > 20)
		ExitApp(1);

	if (flag == Error_Exit || flag == Error_Fatal)
		ExitApp(1);

	if (flag == Error_Skip)
		SkipLine();
	
	// Return to error mark point
	
	ErrorReturn(1);
}

//****************************************
//
//****************************************
/*
	char FileName[256];
	int Line;
	int MasterLine;
	char *MasterPtr;
	int IP;
*/

void ErrorOnIP(int flag, int ip, char *Template, ...)
{
	char 	errorString[1280];
	va_list	args;
	int res;
	LineInfo info;
	
	va_start(args,Template);
	vsprintf(errorString,Template,args);
	va_end(args);
	
	res = FileInfo_From_IP(&info, ip);

	if (!res)
	{
		printf("%s:ip=0x%d : Unable to locate source position\n", errorString, ip);
		return;
	}
	
	printf("%s:%d: ", info.FileName, info.Line);
	
	if (flag == Error_Warning)
		printf("Warning: %s, ", errorString);
	else
		printf("Error: %s, ", errorString);

	printf("(rebuild)\n");

	if (xerr)
	{
		printf("_masterdump.s:%d: Extended error report", info.MasterLine);
		printf("\n");
		
		if (info.MasterPtr)
			VerboseError(info.MasterPtr);
	}

	if (flag == Error_Skip)
		return;

	if (flag == Error_Warning)
		return;

	if (ErrorCount++ > 20)
		ExitApp(1);

	if (flag == Error_Exit || flag == Error_Fatal)
		ExitApp(1);
}


//****************************************
//
//****************************************

void Warning(char *Template, ...)
{
	char 			tbuf[1280];
	va_list 		args;

	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	printf("%s",tbuf);
}

//****************************************
//
//****************************************

void Report(char *Template, ...)
{
	char 			tbuf[1280];
	va_list 		args;

	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	printf("%s",tbuf);
}

//****************************************
//		Set Error Return Position
//****************************************

void SetErrorReset()
{
	ErrRetSet = 0;
}

//****************************************
//		Set Error Return Position
//****************************************

int SetErrorReturn()
{
	int exitcode;

	ErrRetSet = 1;

	exitcode = setjmp(ErrorRet);
	return exitcode;
}

//****************************************
//		Set Error Return Position
//****************************************

void ErrorReturn(int code)
{
	if (!ErrRetSet)
	{
		printf("Exiting...\n");
		ExitApp(1);
	}	

	longjmp(ErrorRet, code);
}

//****************************************
//   Finds file info from an IP address
//		  Fills in a LineInfo
//   FileName, Line, MasterDump Line
//****************************************

int FileInfo_From_IP(LineInfo *info, int ip)
{
	SYMBOL	*Sym;
	int line;
	int file;
	int n;
	char *ptr;
	
	// Copy ip for completeness, save for later use

	info->IP = ip;

	// Clear out other values
		
	info->MasterPtr = 0;
	info->MasterLine = 0;

	info->FileName[0] = 0;
	info->Line = 0;

	// Find the master dump line

	while(1)
	{
		if (ip == 0)
			return 0;
			
		ptr = (char *) ArrayGet(&AsmCharArray, ip);

		if (ptr)
			break;

		ip--;
	}

	// Save the master dump line

	line = GetLineNumber(FileTop, ptr);

	info->MasterPtr = ptr;
	info->MasterLine = line;

	// Find the line above or on the ip

	ip = info->IP;

	while(1)
	{
		if (ip == 0)
			return 0;
			
		line = ArrayGet(&SLD_Line_Array, ip);

		if (line)
			break;

		ip--;
	}			

	// We have an IP and a line

	info->Line = line;

	// Find the file name from number

	file = ArrayGet(&SLD_File_Array, ip);

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (Sym->Section == section_SLD_File)
		{
			if (Sym->Value == file)
			{
				strcpy(info->FileName, Sym->Name);
				break;
			}
		}
	
		Sym++;
	}
	while(--n);

	// We should now have the name of the file too

	return 1;
}


