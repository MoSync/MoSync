/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
		int mV;
		TYPES(UNION_ELEM)
	};

	SYM mSym;
	Builtin::SubType mPrimitiveType;
};

#endif
