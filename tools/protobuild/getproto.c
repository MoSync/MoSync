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

/****************************************************************************
					SoundEdit Label and time extractor
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sioux.h>
#include <setjmp.h>
#include <ctype.h>

#include "PBProto.h"

char filename[128];
char *FilePtr;
char *ProtoPtr;
long LineNum;
FILE *InFile,*OutFile,*PubOutFile;
int	Public = 0;


#define	INCLINE(ccc)	if (ccc == 0x0d) LineNum++

//****************************************
//		  Some useful defines
//****************************************

//#define bypass()			tptr += strlen((char *)tptr) + 1

#define iswhite(c)			(c == ' ' || c == '\t' || c == 0x0d || c == 0x0a )
#define iscsymf(c)			(isalpha(c) || c == '_')
#define iscsym(c)			(isalnum(c) || c == '_')

//#define TwoChar(c1,c2)		*ptr == c1 && *(ptr+1) == c2

char *pos;

void Show()
{
	char *ThisPtr = pos;

	printf("\nPos = ");
	{
		int y;
		unsigned char c;
			
		for(y=0;y<60;y++)
		{
			c = *ThisPtr++;
	
			if (c == '\t')
			{
				printf("\t");
				continue;	
			}
			
			if (c == '\r')	
			{
				printf("\n");
				continue;	
			}
	
			if (c == '\n')
			 	continue;	

			if (c == 0) break;	
			printf("%c",c);
		}
	}
}

//***************************************
//				Exit app
//***************************************

short EJmp = 0;
jmp_buf ErrorJump;

void ExitApp(short n)
{
	longjmp(ErrorJump,n);	// return to main
}

//***************************************
//	   Basic Memory Management for PC
//***************************************

char *NewPtr(long size)
{
	return (char *) malloc(size);
}

char *NewPtrClear(long size)
{
	return (char *) calloc(1, size);

}

void DisposePtr(char *thisPtr)
{
	free(thisPtr);
}

//****************************************
//
//****************************************

char * OpenFile(const char *FileName)
{
	char *ThisMem;
	long Length;
	int res;

	if ((InFile = fopen(FileName,"rb")) == NULL)
	{
			printf("Error : Problem opening '%s' test file\n",FileName);
			ExitApp(0);
	}
	
	fseek(InFile,0,SEEK_END);
	Length = ftell(InFile);

	fseek(InFile,0,SEEK_SET);
	
	if (Length == 0)
	{
		printf("File '%s' Has 0 length",FileName);
		ExitApp(0);
	}
		
	ThisMem = (char *) NewPtrClear((long) Length + 64);

	if (ThisMem == NULL)
	{
		printf("Could not allocate memory for reading file '%s'",FileName);
		ExitApp(0);
	}

	res = fread(ThisMem, 1,(long) Length, InFile);
	if(res != Length) {
		printf("Could not read file '%s'",FileName);
		ExitApp(0);
	}

	return ThisMem;
}

//****************************************
//			
//****************************************

void FreeFile(char *fptr)
{
	if (fptr)
		DisposePtr(fptr);
}

//************************************************************************************************
//								  Parser utility routines
//************************************************************************************************

//****************************************
//
//****************************************

int	 Token(const char *token)
{
	int len = strlen(token);

	SkipWhiteSpace();
	
	if (strncmp(token,FilePtr,len) == 0)
	{
		FilePtr += len;
		SkipWhiteSpace();
		return 1;
	}
	
	return 0;
}

//****************************************
//
//****************************************

int	 String(const char *token)
{
	int len = strlen(token);
	
	if (strncmp(token,FilePtr,len) == 0)
	{
		FilePtr += len;
		return 1;
	}
	
	return 0;
}
//****************************************
//
//****************************************

void SkipToken(char *token)
{	
	FilePtr += strlen(token);
	SkipWhiteSpace();
	return;
	
}
//****************************************
//
//****************************************

void NeedToken(char *token)
{
	if (!Token(token))
	{
		printf("Expected '%s'",token);
		ExitApp(0);
	}
}

//****************************************
//
//****************************************

int NextToken(const char *token)
{	
	return strncmp(token,FilePtr,strlen(token)) == 0;	
}

//****************************************
//			Skip white space
//****************************************

void SkipL()
{
	unsigned char c;

	while (1)
	{
		c = *FilePtr++;
		
		//INCLINE(c);

		if (c == 0x0d || c == 0x0a || c == 0x00)
			break;
	}


	FilePtr--;
}

//****************************************
//			Output documentation
//****************************************

char Split[] = "//********************************************************************************\n";

void GenDocument(char *fptr,int rem)
{
	unsigned char c;

	if (rem)
	{
		if (*fptr == '/')
		{
			fprintf(OutFile,"%s",Split);
			fprintf(PubOutFile,"%s",Split);
			return;
		}

		c = *fptr;

		if (c == 0x0d || c == 0x0a || c == 0x00)
			return;

		fprintf(OutFile,"//");
		fprintf(PubOutFile,"//");
	}

	while (1)
	{
		c = *fptr++;

		if (c == 0x0d || c == 0x0a || c == 0x00)
			break;

		fprintf(OutFile,"%c",c);
		fprintf(PubOutFile,"%c",c);
	}

	fprintf(OutFile,"\n");
	fprintf(PubOutFile,"\n");
}

//****************************************
//			Skip white space
//****************************************

void SkipDefines()
{
	unsigned char c,rep;

	do
	{
		SkipWhiteSpace();

		rep = 0;
				
		while (1)
		{
			c = *FilePtr++;
				
			if (c == '\\')
				rep = 1;
			
			if (c == 0x00)
				return;
				
			if (c == 0x0d || c == 0x0a)
				break;
		}

		INCLINE(c);
	}
	while(rep);

//	FilePtr--;
}

//****************************************
//			Skip white space
//****************************************

void SkipC()
{
	unsigned char c;

	while (1)
	{
		c = *FilePtr++;

		INCLINE(c);
			
		if (c == '*' && *FilePtr == '/')
			break;

		if (c == 0x00)
			break;
	}

	FilePtr++;
}

//****************************************
//			Skip white space
//****************************************

///
////
/// This is the Skip whitespace program
////
///

//#public

void SkipWhiteSpace()
{
	while(1)
	{
		while (iswhite(*FilePtr))
		{

			INCLINE(*FilePtr);
			FilePtr++;
		}
		
		if (*FilePtr == '/' && *(FilePtr+1) == '/')
		{
			if (String("//#public"))
				Public = 1;

			if (String("//#private"))
				Public = 0;

			if (String("//#noproto"))
				Public = -1;

			if (String("//#nowarn"))
				Public = -2;

			if (String("//#text"))
				GenDocument(FilePtr,0);

			if (String("///"))
				GenDocument(FilePtr,1);			// For rems
				
			SkipL();
			continue;
		}
			
		if (*FilePtr == '/' && *(FilePtr+1) == '*')
		{
			SkipC();
			continue;
		}

		break;
	}
}

//****************************************
//			 Skip Bracket Pairs
//****************************************

void SkipQuote(char QToken)
{
	char token;

	token = *FilePtr++;

	if (token != QToken)
	{
		printf("Expected '%c'",QToken);
		ExitApp(0);
	}

	while (1)
	{
		token = *FilePtr;

		if (token == 0)
		{
			printf("Expected '%c'",QToken);
			ExitApp(0);
		}
		
		if (token == QToken)
			break;

		if (token == '\\')
		{
			token = *FilePtr++;
		}

		FilePtr++;
	}


	FilePtr++;
	//SkipWhiteSpace();	

	return;
}

//****************************************
//			 Skip Bracket Pairs
//****************************************

void SkipPair(char LToken,char RToken)
{
	int Count;
	char token;
	char *StPos = FilePtr;
	
	pos = FilePtr;

	token = *FilePtr++;

	if (token != LToken)
	{
		printf("Expected '%c'",LToken);
		ExitApp(0);
	}

	Count = 1;
	
	while (1)
	{
		token = *FilePtr;

		INCLINE(token);

		if (token == 0)
		{
			printf("Expected '%c'",RToken);
			
			pos = StPos;
			Show();	
			ExitApp(0);
		}
		
		if (token == LToken)
			Count++;

		if (token == RToken)
		{
			--Count;
			if (Count == 0)
				break;
		}

		if (token == '(' && LToken != '(')
		{
			SkipPair('(',')');
			continue;
		}	
	
		if (token == '\'')
		{
			SkipQuote(token);
			continue;
		}	

		if (token == '"')
		{
			SkipQuote(token);
			continue;
		}	
	
		FilePtr++;
		SkipWhiteSpace();	
	}

	FilePtr++;
	return;
}

//****************************************
//				SkipName
//****************************************

void SkipName()
{
	while (iscsym(*FilePtr))
		FilePtr++;
}

//**********************************
//		Load sound resource
//**********************************

int main(void)
{
	char *ThisFile,*ProtoFile;
	
//	SIOUXSettings.asktosaveonclose = 0;
//	SIOUXSettings.columns = 60;
//	SIOUXSettings.rows = 20;
//	SIOUXSettings.autocloseonquit = 0;

	printf("ProtoBuilder Prototyper v2.1w\n");
	printf("© 1996-1999 A.R.Hartley\n");
	
//	SIOUXSetTitle("\pProtoBuilder");

	OutFile = fopen("PBProto.h","w");
	PubOutFile = fopen("PBProtoPub.h","w");

	if (OutFile == NULL || PubOutFile == NULL)
	{
		printf("Error : Could not open either 'PBProto.h' or 'PBProtoPub.h' for output\n");
		return 1;
	}

	fprintf(OutFile,"/* ProtoBuilder(tm) AutoPrototype Private file */\n");
	fprintf(OutFile,"/* Copyright A.R.Hartley 1996 v2.1W*/\n");

	fprintf(PubOutFile,"/* ProtoBuilderª AutoPrototype Public file */\n");
	fprintf(PubOutFile,"/* Copyright A.R.Hartley 1996 v2.1w*/\n");

	ProtoFile = ProtoPtr = OpenFile("proto.lst");

	setjmp(ErrorJump);
	EJmp++;
	
	while(EJmp == 1)
	{
		if (ReadFileName() == -1) break;
		
		if (filename[0] != ';')
		{
			printf("Processing '%s'\n",filename);

			fprintf(OutFile,"\n/* Prototypes for '%s' */\n\n",filename);
	//		fprintf(PubOutFile,"\n/* Prototypes for '%s' */\n\n",filename);

			ThisFile = FilePtr = OpenFile(filename);
			MakeProtos();
			FreeFile(ThisFile);
		}
	}

	FreeFile(ProtoFile);

	printf("Done.\n");
	fclose(OutFile);
	fclose(PubOutFile);
	return 0;
}

//**********************************
//		Read in file name
//**********************************

int ReadFileName(void)
{
	int n=0;
	char c=0;

	c = *ProtoPtr++;

	while (c=='\n' || c=='\r')
		c = *ProtoPtr++;

	if (c == 0) return -1;
	if (c == '#') return -1;

	filename[n++] = c;

	while(1)
	{
		c = *ProtoPtr++;

		if (c == '\n' || c=='\r')
			break;

		filename[n++] = c;
			
		if (n>64)
			break;
	}
	
	filename[n] = 0;
	return 0;
}

//****************************************
//			Get type info
//****************************************

long GetType()
{
	int Count = 10000;
	
	while(1)
	{
		SkipWhiteSpace();
		SkipName();
		SkipWhiteSpace();
	
		if (!iscsym(*FilePtr))
			break;
			
		if (*FilePtr == 0)
			break;
			
		if (--Count == 0)
		{
			pos = FilePtr;
			Show();
			ExitApp(0);
		}

	}

	return 0;
}

//****************************************
//       Get level of indirection
//****************************************

long GetIndLevel()
{
		long IndLevel = 0;				// No indirection
		
		while (Token("*"))				// Sigal a ptr
			IndLevel++;					// Increase level of indirection

		return IndLevel;
}

//****************************************
//				SkipName
//****************************************

void SkipLine()
{
	char c;

	SkipWhiteSpace();	
	
	while(1)
	{
		c = *FilePtr++;

		INCLINE(c);

		if (c == 0)	break;
		if (c == ';') break;
	}

	SkipWhiteSpace();	

	return;
}

//****************************************
//			Log a prototype
//****************************************

void LogPrototype(char *ThisLine,char *EndPtr)
{
	char c;

	if (Public == -1)
	{
		Public = 0;
		return;
	}

	while(1)
	{
		if (ThisLine == EndPtr)
			break;
	
		c = *ThisLine++;

		if (c == '\n')	
		{
			continue;	
		}
	
		if (c == '\r')
		{
			fprintf(OutFile,"\n");
			if (Public) fprintf(PubOutFile,"\n");
			continue;
		}
			
		if (c == 0)
			break;

		fprintf(OutFile,"%c",c);
		if (Public) fprintf(PubOutFile,"%c",c);
		
	}

	fprintf(OutFile,";\n");
	if (Public) fprintf(PubOutFile,";\n");
	
	Public = 0;
	return;
}

//****************************************
//			warn a prototype
//****************************************

void WarnPrototype(char *ThisLine,char *EndPtr)
{
	char c;
	
	printf("\nWarning : prototype defined, in '%s' Line %ld\n",filename,LineNum);
	
	while(1)
	{
		if (ThisLine == EndPtr)
			break;
	
		c = *ThisLine++;

		if (c == '\n')	
		{
			printf("\n");
			continue;	
		}
	
		if (c == '\r')	
				continue;	
			
		if (c == 0)
				break;

		printf("%c",c);
	}

	printf(";\n");

	return;
}

//**********************************
//		Load sound resource
//**********************************

void MakeProtos(void)
{	
	char *ThisLine,*EndLine;

	LineNum = 1;

	while(1)
	{
		SkipWhiteSpace();
		
		if (*FilePtr == 0)			// Done
			return;
	
		if (NextToken("#"))				// Test for Directive statements
		{
			SkipDefines();				// Skip them
			continue;
		}
	
		ThisLine = FilePtr;				// save start of line
	
		GetType();						// Grab all type data
		GetIndLevel();					// grab indirection data
		SkipName();

		if (NextToken("("))
		{
			SkipPair('(',')');			// Skip expression

			EndLine = FilePtr;				// save start of line

			SkipWhiteSpace();

//* new
//			if (NextToken("#"))				// Test for Directive statements
//				SkipLine();
//				SkipDefines();				// Skip them
//*

			if (NextToken("{"))
			{
				LogPrototype(ThisLine,EndLine);
				SkipPair('{','}');			// Skip function'
				continue;
			}
			else if (NextToken(";"))
			{
				if (Public != -2)
					WarnPrototype(ThisLine,EndLine);
				else
					Public = 0;
			}
		}
		
		SkipLine();
		
	}
}
