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
//				 PIP-e II Assembler Token Handling Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

char WS[256];

int TokenStack[32];
int TokenSkip[32];
int TokenSP;
int SkipCommentOn = 1;

//****************************************
// 		 Build Whitespace table
//****************************************

void InitTokenSystem()
{
	int n;
	
	for (n=0;n<256;n++)
		WS[n] = (isspace(n) != 0);

	TokenSP = 0;
	SkipCommentOn = 1;
}

//****************************************
//	 Change comment skipping rules
//****************************************
/*
void SetCommentSkip(int flag)
{
	SkipCommentOn = 0;
}
*/
//****************************************
//		  Push Token Pointer
//****************************************

void PushTokenPtr(char *new_file_ptr, int com_skip)
{
	TokenStack[TokenSP] = (int) FilePtr;
	TokenSkip[TokenSP] = SkipCommentOn;
	TokenSP++;
	
	FilePtr = new_file_ptr;
	SkipCommentOn = com_skip;
}

//****************************************
//		  Pop Token Pointer
//****************************************

void PopTokenPtr()
{
	TokenSP--;
	FilePtr = (char *) TokenStack[TokenSP];
	SkipCommentOn = TokenSkip[TokenSP];

}

//****************************************
//		   Test for end of file
//****************************************

char *oldptr = 0;
int stickcount = 0;

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

//****************************************
//
//****************************************

int	 Token(char *token)
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

//****************************************
//
//****************************************

char *pLastToken = 0;

char * LastTokenPtr()
{
	return pLastToken;
}

//****************************************
//		  Quicker token scanner
//****************************************

int	 QToken(char *token)
{
	int len;

	SkipWhiteSpace();
	
	if (*FilePtr != *token)
		return 0;

	len = strlen(token);
	
	if (strncmp(token,FilePtr,len) == 0)
	{
		pLastToken = FilePtr;
		FilePtr += len;
		
		SkipWhiteSpace();	

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
		Error(Error_Skip, "Expected '%s'",token);
		ExitApp(1);
	}
}

//****************************************
//
//****************************************

int NextToken(char *token)
{
	//SkipWhiteSpace();
	return strncmp(token,FilePtr,strlen(token)) == 0;	
}


//****************************************
//			Skip white space
//****************************************

void SkipLine()
{
	unsigned char c;

	while (1)
	{
		c = *FilePtr;

		if (c == 0x0d || c == 0x0a || c == 0x00)
			break;
			
		FilePtr++;
	}

	if (c != 0)
		FilePtr++;

}

//****************************************
//			Skip white space
//****************************************

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

//****************************************
//  	 Test for end of input
//****************************************

int	EndLine()
{	
	SkipWhiteSpace();

	if (*FilePtr == 0)
		return 1;
	
	return 0;
}


//****************************************
//		  Get Prev FilePtr
//****************************************

char * PrevFilePtr = 0;

char * GetPrevFilePtr()
{
	if (PrevFilePtr == 0)
		return FilePtr;

	return PrevFilePtr;
}

//****************************************
//			Skip white space
//****************************************

void SkipWhiteSpace()
{
	register char c;

	PrevFilePtr = FilePtr;

	while (WS[(unsigned char)*FilePtr])
			FilePtr++;

	while(1)
	{
		while (WS[(unsigned char)*FilePtr])
			FilePtr++;

		if (SkipCommentOn == 1)
		{
			if (*FilePtr == ';')
			{
				SkipLine();
				continue;
			}

			if (*FilePtr == '/')
			{
				c = *(FilePtr+1);

				if ( c == '/')
				{
					SkipLine();
					continue;
				}
					
				if (c == '*')
				{
					SkipComment();
					continue;
				}
			}
		}
			
		break;
	}

}


//****************************************
//			 Get a identifier
//****************************************

void GetToken()
{
	char *NamePtr = Name;
	char c;
	int v = 0;

	while (1)
	{
		c = *FilePtr++;

		if (WS[(unsigned char)c])
			break;

		if (c == 0)
			break;

		*NamePtr++ = c;

		if (v++ >= NAME_MAX)
			Error(Error_Fatal, "Token too long");
	}
	
	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

char GetTokenChar()
{
	return *FilePtr++;
}

//****************************************
//			 Get a identifier
//****************************************

void GetName()
{
	unsigned int v = 0;
	char *NamePtr = Name;
	
	while (iscsym(*FilePtr))
	{
		*NamePtr++ = *FilePtr++;

		if (v++ >= NAME_MAX)
			Error(Error_Fatal, "Symbol too int");
	}
	
	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

void GetStringName(int maxlen)
{
	unsigned int v = 0;
	int c = 0;
	char *NamePtr = Name;
	
	if (*FilePtr++ != '"')
		Error(Error_Skip, "String has missing '\"'");
	
	if (maxlen >= NAME_MAX)
		Error(Error_Fatal, "Demands of string size too high");
		
	while (1)
	{
		v = *FilePtr++;

		if (v == 0 || v == 13)
			Error(Error_Skip, "string not terminated correctly");
			
		if (v == '\"')
			break;

		if (v == '\\')
			v = GetEscCode();

		*NamePtr++ = (char)v;

		if (c++ >= NAME_MAX)
			Error(Error_Skip, "string exceeded maximum length");
	}
	
	*NamePtr++ = 0;
	return;
}

//****************************************
//			 Get a identifier
//****************************************

void GetLFileName()
{
	unsigned int v = 0;
	int c = 0;
	char *NamePtr = Name;
	
	if (*FilePtr++ != '\'')
		Error(Error_Skip, "File name has missing quote");
		
	while (1)
	{
		v = *FilePtr++;

		if (v == 0 || v == 13)
			Error(Error_Skip, "string not terminated correctly");
			
		if (v == '\'')
			break;

		*NamePtr++ = (char) v;

		if (c++ >= NAME_MAX)
			Error(Error_Fatal, "file name exceeded maximum length");
	}
	
	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

void GetCmdString()
{
	unsigned int v = 0;
	int c = 0;
	char *NamePtr = Name;
			
	while (1)
	{
		v = *FilePtr++;

		if (v == 0 || v == 13)
			break;
			
		*NamePtr++ = (char) v;

		if (c++ >= NAME_MAX)
			break;
	}
	
	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Skip Bracket Pairs
//****************************************

void SkipQuote(char qToken)
{
	char token;

	token = *FilePtr++;

	if (token != qToken)
	{
		printf("Expected '%c'",qToken);
		ExitApp(1);
	}

	while (1)
	{
		token = *FilePtr;

		if (token == 0)
		{
			printf("Expected '%c'",qToken);
			ExitApp(1);
		}
		
		if (token == qToken)
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
	//char *StPos = FilePtr;
	
	token = *FilePtr++;

	if (token != LToken)
	{
		printf("Expected '%c'",LToken);
		ExitApp(1);
	}

	Count = 1;
	
	while (1)
	{
		token = *FilePtr;

		if (token == 0)
		{
			printf("Expected '%c'",RToken);
			ExitApp(1);
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
//		Get line number
//****************************************

int GetLineNumber(char *TopFile,char *theFilePtr)
{
	int line = 1;
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
//		if (v == 13)
			line++;
	}
	
	return line;
}

//****************************************
//		Get line number
//****************************************

char * TokenSearch(char *token, char *theFilePtr, char **eof)
{
	char *ptr;
	
	PushTokenPtr(theFilePtr, 1);
	
	while(1)
	{			
		if (*FilePtr == 0)
		{
			break;
		}
		
		SkipWhiteSpace();
	
		if (*FilePtr == '.')
		{
			if (NextToken(token))
			{
				ptr = FilePtr;

				// Restore file ptr
	
				PopTokenPtr();
				return ptr;					// Found something
			}
		}

		SkipLine();
	}

	if (eof)
		*eof = FilePtr;

	PopTokenPtr();

	// Say nothing found
	return 0;
}

//****************************************
//
//****************************************

char * FindDirectiveAbove(char *theFilePtr, char *directive)
{	
	char *prevFilePtr = 0;
	char *FilePos = FileTop;
	char *eof;
	
	int	len = strlen(directive);

	do
	{
		FilePos = TokenSearch(directive, FilePos, &eof);

		// Have we passed out file pointer

		if (FilePos > theFilePtr)
			return PrevFilePtr;

		// Have we reached eof
		
		if (!FilePos)
		{
			if (eof > theFilePtr)
				return prevFilePtr;
			
			break;
		}
		
		prevFilePtr = FilePos;
		
		FilePos += len;
	}
	while(1);

	return 0;

}

//****************************************
//
//****************************************

char hextab2[] = "0123456789abcdef";
char hexstr[16];

char * Hex32(int v)
{
	int n;
	for (n=0;n<8;n++)
	{
		hexstr[n] = hextab2[(v >> 28) & 0xf];
		v <<= 4;
	}

	hexstr[8] = 0;
	return hexstr;
}

//****************************************
//
//****************************************

char * Hex8(int v)
{
	hexstr[0] = hextab2[(v >> 4) & 0xf];
	hexstr[1] = hextab2[v & 0xf];
	hexstr[2] = 0;
	return hexstr;
}

