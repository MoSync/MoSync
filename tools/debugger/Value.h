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

#ifndef _VALUE_H_
#define _VALUE_H_

#include "stabs/stabs.h"
#include "stabs/stabs_builtins.h"
#include "old_expr.h"
#include "cmd_stack.h"
#include "helpers.h"

#include <map>
#include <string>
#include "Token.h"
#include "ParseException.h"

typedef Builtin::SubType PrimitiveType;

// did this in project settings instead
// disables warning about using unary '-' on unsigned type.
//#pragma warning(disable:4146)

// also disabled warning 4800
// (that says that you can't typecast for instance doubles to bool)

#include "stabs/stabs_builtins.h"

#if defined(_MSC_VER) && defined(NDEBUG)
// fix warning C4748
#pragma optimize("g", on)
#endif

#define TYPES(m) \
	BUILTINS(m, m) \
	m(bool, Bool)

TypeBase* getTypeBaseFromType(Builtin::SubType type);

#define V_CONSTRUCTOR_ELEM_HEADER(theType, id)\
	Value(theType t);

#define V_OPERATOR_CAST_ELEM_HEADER(toType, id)\
	operator toType () const;

const TypeBase* convertConstType(const TypeBase* tb);

class Value {
public:
#define CONVERT_ELEM(toType, id) case Builtin::e##id: return (T)id;
	template<typename T>
	T convertToType() const {
		if(isType()) throw ParseException("Cannot convert type to value.");
		switch(mPrimitiveType) {
			TYPES(CONVERT_ELEM)
			default: throw ParseException("Conversion error");
		}
	}

	Value();
	Value(const std::string& value);
	Value(const SYM& sym);

	// type is value
	Value(const TypeBase* typeBase);

	TYPES(V_CONSTRUCTOR_ELEM_HEADER)
	TYPES(V_OPERATOR_CAST_ELEM_HEADER)

	bool isDereferencable() const;
	bool isPointer() const;
	Builtin::SubType getPrimitiveType() const;
	TypeBase::Type getType() const;
	const SYM& getSymbol() const;
	void setSymbol(const SYM& sym);
	void setTypeBase(const TypeBase* type);
	const TypeBase* getTypeBase() const;
	const void* getDataAddress() const;

	const TypeBase *deref() const;

	// if value is an actual type
	bool isType() const;
	int sizeOf() const;


#include "operations_generated.h"

private:
#define UNION_ELEM(type, id) type id;
	union {
		size_t mV;
		TYPES(UNION_ELEM)
	};

	SYM mSym;
	Builtin::SubType mPrimitiveType;
};

#endif
