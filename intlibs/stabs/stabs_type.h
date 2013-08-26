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
	int length() const { return mPos; }
	int operator()(const char* fmt, ...) PRINTF_ATTRIB(2,3);

private:
	int mPos;
	char* mString;
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
void printPrimitiveByFormat(printfPtr pf, const void* data, const char* decimalFmt,
	TypeBase::PrintFormat fmt, TypeBase::PrintFormat natural)
{
	if(natural == TypeBase::eNatural) return; // sanity check
	T t = *((T*)data);

	if(fmt == TypeBase::eNatural) {
		printPrimitiveByFormat<T>(pf, data, decimalFmt, natural, natural);
		return;
	} else if(fmt == TypeBase::eDecimal) {
		pf(decimalFmt, t);
	} else if(fmt == TypeBase::eOctal) {
		pf("%" PFZT_PREFIX "o", (size_t)t);
	} else if(fmt == TypeBase::eHexadecimal) {
		pf("0x%" PFZT_PREFIX "x", (size_t)t);
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
