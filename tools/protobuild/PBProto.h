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

/* ProtoBuilder(tm) AutoPrototype Private file */

/* Prototypes for 'getproto.c' */

void Show();
void ExitApp(short n);
char *NewPtr(long size);
char *NewPtrClear(long size);
void DisposePtr(char *thisPtr);
char * OpenFile(const char *FileName);
void FreeFile(char *fptr);
int	 Token(const char *token);
int	 String(const char *token);
void SkipToken(char *token);
void NeedToken(char *token);
int NextToken(const char *token);
void SkipL();
void GenDocument(char *fptr,int rem);
void SkipDefines();
void SkipC();
//********************************************************************************
// This is the Skip whitespace program
//********************************************************************************
void SkipWhiteSpace();
void SkipQuote(char QToken);
void SkipPair(char LToken,char RToken);
void SkipName();
int main(void);
int ReadFileName(void);
long GetType();
long GetIndLevel();
void SkipLine();
void LogPrototype(char *ThisLine,char *EndPtr);
void WarnPrototype(char *ThisLine,char *EndPtr);
void MakeProtos(void);
