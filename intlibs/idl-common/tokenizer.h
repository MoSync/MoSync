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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>
#include "helpers/attribute.h"

using namespace std;

void setTokenStream(istream* is);
void readToken(string& token);
void readLine(string& line);
void readTextToken(string& token);
void readQuotedString(string& token);
void doExact(const string& aToken);
string getComment();
void ATTRIBUTE(noreturn, tokenError(const string& token));
void ATTRIBUTE(noreturn, Error(const string& type, const string& str));
int getCurrentLine();
void doEOF();

#endif	//TOKENIZER_H
