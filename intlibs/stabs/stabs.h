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

#ifndef STABS_H
#define STABS_H

#include "stabs_function.h"
#include "stabs_static.h"

bool loadStabs(const char* sld, const char* stabs);
bool stabsIsLoaded();

void stabsEnumerateSymbols(void (*callback)(const Symbol*));
const Type* stabsFindTypeByName(const std::string& name, int scope);

//note: separate address spaces.
const Function* stabsGetFunctionByAddress(int address);
const StaticVariable* stabsGetVariableByAddress(int address);

const Symbol* stabsGetSymbolByScopeAndName(int scope, const std::string& name);
const Symbol* stabsGetSymbolGlobal(const std::string& name);

#ifndef WIN32
inline int iscsym(int c) { return (isalnum(c) || ((c) == '_')); }
#endif

#endif	//STABS_H
