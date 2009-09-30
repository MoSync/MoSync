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

//typedef int (*printfPtr)(const char* fmt, ...);

class PrintFunctor {
public:
	virtual ~PrintFunctor() {}
	virtual int operator()(const char* fmt, ...) = 0;
};

class StringPrintFunctor : public PrintFunctor {
public:
	StringPrintFunctor();
	~StringPrintFunctor();

	const char* getString();
	void reset();
	void resizeString(int size);
	int operator()(const char* fmt, ...);

private:
	int mPos;
	char* mPtr;
	char *mString;
	int mStringSize;
};

typedef PrintFunctor& printfPtr;

class TypeBase {
public:
	enum Type {
		eUnknown, eBuiltin, eStruct, eArray, eEnum, ePointer, eFunction
	};

	enum PrintFormat {
		eBinary, eDecimal, eHexadecimal, eOctal, eNatural
	};

	TypeBase(int s, bool simple, Type tt) : size(s), isSimpleValue(simple), type(tt) {}
	virtual ~TypeBase() {}

	const int size;
	const bool isSimpleValue;
	const Type type;

	virtual void printMI(printfPtr, const void* data, PrintFormat pf) const = 0;

	//if complex && !isSimpleValue, then members will be printed.
	//multiple lines will be used to for structs.
	//enums will be printed on single lines.
	virtual void printTypeMI(printfPtr, bool complex) const = 0;

	//returns the type a pointer points to.
	//will return NULL if this type is not a pointer.
	virtual const TypeBase* deref() const;

	/// Returns a fixed type. Useful for types that are declared too early.
	virtual const TypeBase* resolve() const;
};

struct Tuple {
	Tuple(int x=-1, int y=-1) : a(x), b(y) {}
	int a, b;
	bool operator==(const Tuple& other) const { return a == other.a && b == other.b; }
	bool operator!=(const Tuple& other) const { return a != other.a || b != other.b; }
};

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

#endif	//STABS_TYPE_H
