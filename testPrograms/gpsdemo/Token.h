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
