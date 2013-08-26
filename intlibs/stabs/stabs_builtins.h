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

#ifndef STABS_BUILTINS_H
#define STABS_BUILTINS_H

#include "stabs_types.h"

struct BUILTINdecl {
	const char* name;
	TypeBase* type;
};

extern BUILTINdecl sBuiltins[];
extern const int snBuiltins;

#include "stabs_builtins_generated.h"

#define ENUM_BUILTIN_FIRST(name, id) e##id
#define ENUM_BUILTIN(name, id) ,e##id

class Builtin : public TypeBase {
public:
	enum SubType {
		BUILTINS(ENUM_BUILTIN_FIRST, ENUM_BUILTIN)
		BROKEN_BUILTINS(ENUM_BUILTIN),
		eBool, 
		eVTablePtr,
		NUM_SUB_TYPES
	};

	Builtin(const char* name, int s, SubType st) :
		mSize(s), mName(name), mSubType(st) {}
	int size() const { return mSize; }
	bool isSimpleValue() const { return true; }
	Type type() const { return eBuiltin; }
	const TypeBase* resolve() const { return this; }
	void printTypeMI(printfPtr p, bool complex) const { p("%s", mName); }
	SubType subType() const { return mSubType; }
public:
	const int mSize;
	const char* const mName;
	const SubType mSubType;
};

#endif	//STABS_BUILTINS_H
