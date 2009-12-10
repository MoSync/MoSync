// DATATYPE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__DATATYPE_H__)
#define __DATATYPE_H__
#if !defined(__ARRAY_H__)
#include "array.h"
#endif
#if !defined(__ASTRING_H__)
#include "astring.h"
#endif

typedef unsigned long DataType;

// RCType

class RCType : public ArrayItem
{
public:
	RCType(DataType ElementType,const String& Name,unsigned short Size);
	virtual ~RCType();
	const String& GetName() const;
	DataType GetElementType() const;
	unsigned short GetSize() const;
	friend std::ostream& operator<<(std::ostream& os,RCType& r);
private:
	DataType iElementType;
	String iName;
	unsigned short iSize;
};

// RCTypeArray

/*
* alfredh: changed from 'protected' to 'public' due to compiler error:
*
* g++-2.95 -Wall -gstabs+ -Wno-deprecated -I../inc -Iderived -c datatype.cpp -o datatype.o
* datatype.cpp: In method `RCTypeArrayIterator::RCTypeArrayIterator(const RCTypeArray &)':
* datatype.cpp:100: fields of `const Array' are inaccessible in `const RCTypeArray' due to private inheritance
* make: *** [datatype.o] Error 1
*/
class RCTypeArray : public Array
{
public:
	RCTypeArray();
	~RCTypeArray();
	String const& GetName(DataType ElementTypeSought) const;
	unsigned short GetSize(DataType ElementTypeSought) const;
};

// RCTypeArrayIterator

class RCTypeArrayIterator : public ArrayIterator
{
public:
	RCTypeArrayIterator(const RCTypeArray& c);
	RCType * operator()();
};

#endif
