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
