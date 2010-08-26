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

#ifndef STABS_TYPE_H
#define STABS_TYPE_H

#include <string>
#include "helpers/attribute.h"
#include "helpers/types.h"
#include "helpers/RefCounted.h"

//typedef int (*printfPtr)(const char* fmt, ...);

class PrintFunctor {
public:
	virtual ~PrintFunctor() {}
	virtual int operator()(const char* fmt, ...) PRINTF_ATTRIB(2,3) = 0;
};

class StringPrintFunctor : public PrintFunctor {
public:
	StringPrintFunctor();
	~StringPrintFunctor();

	const char* getString();
	void reset();
	void resizeString(int size);
	int operator()(const char* fmt, ...) PRINTF_ATTRIB(2,3);

private:
	int mPos;
	char* mPtr;
	char *mString;
	int mStringSize;
};

typedef PrintFunctor& printfPtr;

class TypeBase : public RefCounted {
public:
	enum Type {
		eUnknown, eBuiltin, eStruct, eArray, eEnum, ePointer, eFunction, ePointerToMember, eConst
	};

	enum PrintFormat {
		eBinary, eDecimal, eHexadecimal, eOctal, eNatural
	};

	virtual ~TypeBase() {}

	virtual int size() const = 0;
	virtual bool isSimpleValue() const = 0;
	virtual Type type() const = 0;

	virtual void printMI(printfPtr, const void* data, PrintFormat pf) const = 0;

	//if complex && !isSimpleValue, then members will be printed.
	//multiple lines will be used for structs.
	//enums will be printed on single lines.
	virtual void printTypeMI(printfPtr, bool complex) const = 0;

	//returns the type a pointer points to.
	//will return NULL if this type is not a pointer.
	virtual const TypeBase* deref() const;

	//returns a pointer suitable for casting.
	virtual const TypeBase* resolve() const;
};

struct Tuple {
	Tuple(int x=-1, int y=-1) : a(x), b(y) {}
	int a, b;
	bool operator==(const Tuple& other) const { return a == other.a && b == other.b; }
	bool operator!=(const Tuple& other) const { return a != other.a || b != other.b; }
};

//todo: add reference counting for the type?
struct Type {
	Tuple id;
	const TypeBase* type;
	std::string name;

	bool operator==(const Type& other) const {
		return id.a == other.id.a && id.b == other.id.b;
	}
	bool operator<(const Type& other) const {
		if(id.a == other.id.a)
			return id.b < other.id.b;
		else
			return id.a < other.id.a;
	}
};


template<typename T>
void printPrimitiveByFormat(printfPtr pf, const void* data, const char* decimalFmt, TypeBase::PrintFormat fmt, TypeBase::PrintFormat natural) {
	if(natural == TypeBase::eNatural) return; // sanity check
	T t = *((T*)data);

	if(fmt == TypeBase::eNatural) {
		printPrimitiveByFormat<T>(pf, data, decimalFmt, natural, natural);
		return;
	} else if(fmt == TypeBase::eDecimal) {
		pf(decimalFmt, t);
	} else if(fmt == TypeBase::eOctal) {
		pf("%o", (unsigned)t);
	} else if(fmt == TypeBase::eHexadecimal) {
		pf("0x%x", (unsigned)t);
	} else if(fmt == TypeBase::eBinary) {
		u64 tt = (u64)(size_t)t;
		int numBits = (int)(sizeof(T)<<3)-1;
		while(numBits>0 && !(tt&((u64)1<<numBits))) numBits--;
		while(numBits>=0) {
				pf((tt&((u64)1<<numBits))?"1":"0");
				numBits--;
		}
	}
}

#endif	//STABS_TYPE_H
