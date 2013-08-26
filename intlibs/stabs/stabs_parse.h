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

#ifndef STABS_PARSE_H
#define STABS_PARSE_H

#include <string>

#include "stabs_type.h"
#include "stabs_function.h"

char* scanTuple(char* str, Tuple* t);
const TypeBase* subParseType(char** pText, const Tuple& id = Tuple(),
	const std::string& name = std::string());

bool parseLSym(Tuple t, char* text);

extern Function* gLastFunction;

#endif	//STABS_PARSE_H
