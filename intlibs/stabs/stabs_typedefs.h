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

#ifndef STABS_TYPEDEFS_H
#define STABS_TYPEDEFS_H

#include <vector>

#include "stabs_type.h"

class TypeReference : public TypeBase {
public:
	void ATTRIB(noreturn) printMI(printfPtr, const void*, PrintFormat) const;
	void ATTRIB(noreturn) printTypeMI(printfPtr, bool complex) const;
	int size() const { return 0; }
	bool isSimpleValue() const { return true; }
	Type type() const { return eUnknown; }

	virtual const TypeBase* resolve() const = 0;
};

class DelayedType : public TypeBase {
public:
	/// Records the object for later deallocation.
	DelayedType(TypeReference*);
	virtual ~DelayedType();

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	const TypeBase* deref() const;
	const TypeBase* resolve() const;
	int size() const { return 0; }
	bool isSimpleValue() const { return true; }
	Type type() const { return eUnknown; }

	static bool resolveAll();
protected:
	const TypeBase* mType;
};

class TupleReference : public TypeReference {
public:
	TupleReference(Tuple);

	virtual const TypeBase* resolve() const;
protected:
	const Tuple mId;
	const int mFile;
};

class CrossReferenceType : public TupleReference {
public:
	CrossReferenceType(Tuple, const char* name);
	const TypeBase* resolve() const;

	const std::string mName;
};

class PointerType : public TypeBase {
public:
	PointerType(const TypeBase* target);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	const TypeBase* deref() const;
	int size() const { return 4; }
	bool isSimpleValue() const { return true; }
	Type type() const { return ePointer; }

	const TypeBase* const mTarget;
protected:
};

typedef PointerType ReferenceType;

class FunctionType : public TypeBase {
public:
	FunctionType(const TypeBase* returnType);
	void addParam(const TypeBase*);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const { return 0; }
	bool isSimpleValue() const { return false; }
	Type type() const { return eFunction; }

	const TypeBase* const mReturnType;
protected:
	std::vector<const TypeBase*> mParams;
};

//see GDB STABS spec
enum Visibility {
	ePrivate = 0,
	eProtected = 1,
	ePublic = 2
};

struct DataMember {
	std::string name;
	const TypeBase* type;
	int offsetBits, sizeBits;
	Visibility visibility;
};

struct Method {

	enum Linkage {
		eStatic, eNormal, eVirtual
	};

	std::string name;
	std::string globalName;
	Visibility visibility;
	Linkage linkage;
	bool isConst;
	bool isVolatile;
	int virtualOffset;	//only valid if linkage is virtual

	//the class type that this function is overriding.
	//may be the class that this function is a member of, if it's the first.
	const TypeBase* virtualBaseType;
};

struct BaseClass {
	const TypeBase* type;
	Visibility visibility;
	int offset;	//in bytes, from the subclass's this pointer.
	bool isVirtual;
};

struct StaticDataMember {
	const TypeBase* type;
	Visibility visibility;
	std::string name, globalName;
};

class StructType : public TypeBase {
public:
	StructType(int size, const std::string& name);
	void addDataMember(const DataMember&);
	void addMethod(const Method&);
	void addBaseClass(const BaseClass&);
	void addStaticDataMember(const StaticDataMember&);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const { return mSize; }
	bool isSimpleValue() const { return false; }
	Type type() const { return eStruct; }

	const std::string mName;
	const int mSize;

	const std::vector<DataMember>& getDataMembers() const;
	const std::vector<Method>& getMethods() const;
	const std::vector<BaseClass>& getBases() const;
	const std::vector<StaticDataMember>& getStaticDataMembers() const;

protected:
	std::vector<DataMember> mDataMembers;
	std::vector<Method> mMethods;
	std::vector<BaseClass> mBases;
	std::vector<StaticDataMember> mStaticDataMembers;
};

class PointerToMemberType : public TypeBase {
public:
	PointerToMemberType(const StructType* c, const TypeBase* m);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const { return 4; }
	bool isSimpleValue() const { return true; }
	Type type() const { return ePointerToMember; }

	const StructType* const mClass;
	const TypeBase* const mMember;
};

class RangeType : public TypeBase {
public:
	RangeType(int min, int max);

	void ATTRIB(noreturn) printMI(printfPtr, const void*, PrintFormat) const;
	void ATTRIB(noreturn) printTypeMI(printfPtr, bool complex) const;
	int size() const { return 0; }
	bool isSimpleValue() const { return true; }
	Type type() const { return eUnknown; }
protected:
	const int mMin, mMax;
};

class ArrayType : public TypeBase {
public:
	ArrayType(int len, const TypeBase* elemType, const TypeBase* rangeType);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const;
	bool isSimpleValue() const { return false; }
	Type type() const { return eArray; }

	const int mLength;
	const TypeBase* const mElemType;
	const TypeBase* const mRangeType;
protected:
};

class ConstType : public TypeBase {
public:
	ConstType(const TypeBase* target);

	void printMI(printfPtr, const void*, PrintFormat) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const;
	bool isSimpleValue() const;
	Type type() const;

	const TypeBase* deref() const;

	const TypeBase* const mTarget;
protected:
};

struct EnumMember {
	std::string name;
	int value;
};

class EnumType : public TypeBase {
public:
	EnumType(const std::string& name);
	void addMember(const EnumMember&);

	void printMI(printfPtr, const void*, PrintFormat pf) const;
	void printTypeMI(printfPtr, bool complex) const;
	int size() const { return 4; }
	bool isSimpleValue() const { return true; }
	Type type() const { return eEnum; }

	const std::string mName;
protected:
	std::vector<EnumMember> mMembers;
};

#if 0
class StructCrossReferenceType : public CrossReferenceType {
public:
	StructCrossReferenceType(const char* name) : CrossReferenceType(name) {}

	void printMI(printfPtr, const void*, PrintFormat pf) const;
};

class EnumCrossReferenceType : public CrossReferenceType {
public:
	EnumCrossReferenceType(const char* name) : CrossReferenceType(name) {}

	void printMI(printfPtr, const void*, PrintFormat pf) const;
};
#endif

#endif	//STABS_TYPEDEFS_H
