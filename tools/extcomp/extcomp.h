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

#ifndef EXTCOMP_H_
#define EXTCOMP_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <map>

void streamFunctionCSignature(ostream&, Interface&, Function&);
void streamFunctionWrapper(ostream&, Interface&, Function&, bool);
string resolveTypedef(Interface&, string&);
void streamExtensionManifest(map<string,string>&);
void streamExtHashValue(ostream&, Interface&);
void streamXMLComment(ostream&, string& comment);
void writeHeaders(string& outputFile, Interface& ext, bool includeFunctions);
string getModHandle(Interface& ext);
string getFnIxDefine(Function& f);
string getFnIxHandle(Interface& f);
string getModHashDefine(Interface& ext);
bool isReturnType(Interface&, string&);
string extractPointerType(string type, int& ptrDepth);
Struct* getStruct(Interface& ext, string& structType);
int getPadding(Interface& ext, string& type, size_t offset);
size_t cTypeAlignment(Interface& ext, string& type);
size_t cTypeAlignedSize(Interface& ext, string& type);
string safeArg(string& arg);

#endif /* EXTCOMP_H_ */
