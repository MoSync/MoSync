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

#ifndef JSEXT_H_
#define JSEXT_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>

#include <idl-common/idl-common.h>
#include <idl-common/types.h>

using namespace std;

void writeJSBridge(string&, Interface&);
string getJSTypeDesc(Interface& ext, vector<string>& names, vector<string>& types, vector<bool>* dirs, map<string, int>& structIdMap, vector<string>& typedescs, bool argList);
string getJSDefaultValue(string scalarType);
void generateArrayMarshalling(ostream& extensionFile, Interface& ext, string& arrayName, string& arrayType, int ptrDepth);
void generateStructMarshalling(ostream& extensionFile, Interface& ext, string& varName, string& varType);
void generateMarshalling(ostream& extensionFile, Interface& ext, string& varName, string& varType);

#endif /* JSEXT_H_ */
