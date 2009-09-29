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

/* ProtoBuilder(tm) AutoPrototype file 'Token.h' */
/* Copyright A.R.Hartley 1996 v2.1W*/
/*void SetTokenPtr(char *ptr,char *top);
char * GetTokenPtr();
char * GetFileTop();
long GetDecNum();
long GetHexNum();
short GetHexDigit();
long GetNum();
short isEOF();
void GetName();
float GetFloat();
char GetChar();
void GetStrToWS(short maxlen);
int Token(char *token);
void SkipToken(char *token);
int NextToken(char *token);
void NeedToken(char *token);
void SkipLine();
void SkipComment();
void SkipWhiteSpace();
char *NewPtr(long size);
char *NewPtrClear(long size);
void DisposePtr(char *thisPtr);
char * Load_File(char *filename);
void Free_File(char *ptr);
long GetLineNumber(char *TopFile,char *theFilePtr);
void Error(char *Template, ...);
void BasicError(char *Template, ...);*/

int Token(char *token);
void SkipWhiteSpace();
void NeedToken(char *token);
double GetFloat();
char GetChar();
long GetDecNum();
long GetHexNum();
short GetHexDigit();
long GetNum();

//void GetStr(char* buf, int maxlen);
