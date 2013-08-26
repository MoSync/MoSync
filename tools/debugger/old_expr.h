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

#ifndef OLD_EXPR_H
#define OLD_EXPR_H

#include <string>
#include "stabs/stabs_symbol.h"
#include "stabs/stabs_type.h"
#include "stabs/stabs_function.h"

struct SYM {
	SymbolType symType;
	const TypeBase* type;

	const void* address;
	StorageClass storageClass; // only valid if adress exist.

	// scope
	struct Scope {
		enum ScopeType {
			eLocal,
			eStatic,
			eGlobal
		};
		ScopeType type;
		union {
			struct { 
				unsigned int start, end;
			};
			int fileScope;
		};
	};
	Scope scope;
};

typedef void (*EECallback)(const char* type, const char* value, bool simpleType);
typedef void (*SeeCallback)(const SYM& sym);

//Attempts to evaluate \a expr in a given frame.
//\a frameAddr may be <0, in which case the Current Frame is used.
//Fails if there is no current frame.
//Calls the callback on success.
//Calls error() on failure.
/*
void stackEvaluateExpression(const std::string& expr, int frameAddr,
	EECallback, bool complex);
*/

void locate_symbol(const std::string& name, SeeCallback cb);

bool isLocalGlobalOrStatic(const std::string& name);


#endif
