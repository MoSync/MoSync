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

//****************************************************************************
//							Token Reader
//****************************************************************************

#include "Token.h"

const char *FilePtr;
const char *FileTop;

//char Name[256];

//****************************************
//		   Set the File Pointer
//****************************************

void SetTokenPtr(char *ptr,char *top)
{
	FilePtr = ptr;
	FileTop = top;
}

//****************************************
//		   Set the File Pointer
//****************************************

const char * GetTokenPtr()
{
	return FilePtr;
}

//		   Set the File Pointer

const char * GetFileTop()
{
	return FileTop;
}

//		    Get Decimal Number

long GetDecNum()
{
	register unsigned long v = 0;

	while (isdigit(*FilePtr))
		v = (v * 10) + (*FilePtr++ - 0x30);
	
	return (long) v;
}

//		      Get Hex Number

long GetHexNum()
{
	register unsigned long v = 0;
	register unsigned long d;

	while (isxdigit(*FilePtr))
	{		
		d = toupper(*FilePtr++);
		
		v *= 16;
		
		if (isdigit(d))
			v += d - '0';
		else
			v += (d - 'A') + 10;
	}
	
	return (long) v;
}

//			Get a hex digit

short GetHexDigit()
{
	short d;
	
	d = toupper(*FilePtr++);
				
	if (isdigit(d))
		return d - '0';
	else
		return (d - 'A') + 10;
}

//		   Test for end of file

long GetNum()
{
	if (Token("0x"))
		return GetHexNum();

	return GetDecNum();
}

//		   Test for end of file

const char *oldptr = 0;
long stickcount = 0;

short isEOF()
{
	if (FilePtr == oldptr)
	{
		stickcount++;
	}
	else
	{
		stickcount = 0;
		oldptr = FilePtr;
	}

	if (stickcount > 1000)
	{
		return -1;
	}

	return (*FilePtr == 0);
}

//			 Get a identifier

/*short maxlen = 100;

void GetName()
{
	unsigned long v = 0;
	char *NamePtr = Name;
	short c=0;
		
	while (*FilePtr != 0)
	{
		v = *FilePtr++;

		if (!iscsymf(v))
			break;

//		if ((v == 10) || (v == 13))
//			break;

		*NamePtr++ = v;

		if (c++ > maxlen)
			break;
	}
	
	*NamePtr++ = 0;
	return;
}*/

//			 Get a identifier

void GetStr(char* buf, int maxlen)
{
	//unsigned long v = 0;
	//char *NamePtr = Name;
	short c=0;
	
	if (*FilePtr != '"')
		return;

	FilePtr++;
	
	while (*FilePtr != '\"')
	{
		if (*FilePtr == 0 || *FilePtr == 13)
			break;
	
		*buf++ = *FilePtr++;

		if (c++ > maxlen)
			break;
	}

	*buf++ = 0;

	if (Token("\""))
		return;

	return;
}

//			 Get a number string

double GetFloat()
{
	const char* endptr;
	double v;

	v = strtod(FilePtr, &endptr);
	FilePtr = endptr;

	return v;
}

/*
float GetFloat()
{
	char *NamePtr = Name;
	char c;
	int n;
	float val;
	
	for (n=0;n<32;n++)
	{
		c = *FilePtr++;
		
		if (iswhite(c))
			break;

		if (c == ';' || c == ','  || c == ';' || c == ':' || c == 0 || c == 13)
			break;
	
		*NamePtr++ = c;
	}

	*NamePtr++ = 0;


	val = strtod(Name, 0);
	return val;
}
*/
//			 Get a number string

char GetChar()
{		
	return *FilePtr++;
}

//			 Get a number string

/*void GetStrToWS(short maxlen)
{
	//unsigned long v = 0;
	char *NamePtr = Name;
	short c=0;
		
	while(1)
	{
		if (iswhite(*FilePtr))
			break;
	
		*NamePtr++ = *FilePtr++;

		if (c++ > maxlen)
			break;
	}

	*NamePtr++ = 0;

	return;
}*/

//			 Test a Token

int	Token(char *token)
{
	int len;
	
	SkipWhiteSpace();

	if (*FilePtr != *token)
		return 0;

	len = strlen(token);
	
	if (strncmp(token,FilePtr,len) == 0)
	{
		FilePtr += len;
		SkipWhiteSpace();
		return 1;
	}
	
	return 0;
}

//			Skip a Token

void SkipToken(char *token)
{	
	FilePtr += strlen(token);
	SkipWhiteSpace();
	return;
	
}

//			Next Token

int NextToken(char *token)
{
	//SkipWhiteSpace();
	return strncmp(token,FilePtr,strlen(token)) == 0;	
}

//

void NeedToken(char *token)
{
	if (!Token(token))
	{
		//println("Expected token");
		//println(token);
	}
}

//			Skip white space

void SkipLine()
{
	unsigned char c;

	while (1)
	{
		c = *FilePtr++;

		if (c == 0x0d || c == 0x0a || c == 0x00)
			break;
	}

	FilePtr--;
}

//			Skip white space

void SkipComment()
{
	unsigned char c;

	while (1)
	{
		c = *FilePtr++;
			
		if (c == '*' && *FilePtr == '/')
			break;

		if (c == 0x00)
			break;
	}

	FilePtr++;
}

//			Skip white space

void SkipWhiteSpace()
{
	while(1)
	{
		while (iswhite(*FilePtr))
			FilePtr++;
		
		if (*FilePtr == '/' && *(FilePtr+1) == '/')
		{
			SkipLine();
			continue;
		}
			

		if (*FilePtr == '/' && *(FilePtr+1) == '*')
		{
			SkipComment();
			continue;
		}

		break;
	}
}


//	   Basic Memory Management for PC

/*char *NewPtr(long size)
{
	return (char *) malloc(size);
}

char *NewPtrClear(long size)
{
	char *mem;
	
	mem = (char *) malloc(size);

	if (mem == NULL)
		return mem;

	memset((void *) mem,0,size);

	return mem;
}

void DisposePtr(char *thisPtr)
{
	 free(thisPtr);
}

//			  Load Binary

char * Load_File(char *filename)
{
	FILE *InFile;
	long len;
	char *theMem;
	
	if ((InFile = fopen(filename,"rb")) == NULL)
		return 0;

	fseek(InFile, 0L ,SEEK_END);	// Point to end of file

	len = ftell(InFile);
	fseek(InFile, 0L ,SEEK_SET);	// Repoint a pos 0

	theMem = NewPtrClear(len+32);

	if (!theMem)
	{
		fclose(InFile);
		return 0;
	}
	
	fread(theMem, 1,(long) len, InFile);
	fclose(InFile);

	return theMem;
}

//		Free a File Binary

void Free_File(char *ptr)
{
	if (ptr)
		DisposePtr(ptr);
}*/

//		Get line number

/*long GetLineNumber(char *TopFile,char *theFilePtr)
{
	long line = 1;
	char *ptr;
	char v;
	
	ptr = TopFile;
	
	while(1)
	{
		if (ptr >= theFilePtr)
			break;
			
		v = *ptr++;
		
		if (v == 0)
			break;
		
		if (v == 10)
			line++;
	}
	
	return line;
}

//

void Error(char *Template, ...)
{
	char 			tbuf[128];
	va_list 		args;
	long			line;
	
	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	line = GetLineNumber(FileTop,FilePtr);

	printf("Error: %s : Line %ld",tbuf,line);
	printf("\n");
	
	{
		int y;
		unsigned char c;
		char *tptr = FilePtr;
		
		for(y=0;y<2;)
		{
			if (FilePtr == FileTop)
				break;
	
			c = *--FilePtr;
	
			if (c == 0) break;
			if (c == 13) y++;
		}

		printf("%03ld: ",GetLineNumber(FileTop,FilePtr));

		for(y=0;y<4;)
		{
		
			if (FilePtr == tptr)
				printf("<ERROR>");

			c = *FilePtr++;
	
			if (c == 0) break;
			if (c == 13) y++;
			//if (c == 10) break;
	
			printf("%c",c);
			
			if (c == 10)
				printf("%03ld: ",GetLineNumber(FileTop,FilePtr));

		}
	}

	printf("\n");
	
	exit(0);
}

//

void BasicError(char *Template, ...)
{
	char 			tbuf[128];
	va_list 		args;
	long			line;
	
	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	printf("Error: %s\n",tbuf);
	exit(-1);
}*/
