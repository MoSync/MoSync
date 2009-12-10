// STRINGAR.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __STRINGAR_H__
#define __STRINGAR_H__

#include "array.h"
#include "astring.h"

class StringArray : public Array
{
	friend std::ostream& operator<<(std::ostream & os, StringArray & s);
	friend RCBinaryStream& operator<<(RCBinaryStream & os, StringArray & s);
public:
	StringArray();
	StringArray& operator=(const StringArray& Source);
	void Add( String * pNewItem);
};

class StringArrayIterator : public ArrayIterator
{
public:
	StringArrayIterator(const StringArray & c);
	String * operator()();
};


#endif
