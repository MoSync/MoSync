// INDEXTAB.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <iostream>
#include "rcbinstr.h"
#include "array.h"
#include "resource.h"


class IndexTableItem : public ArrayItem
{
	friend std::ostream& operator<<(std::ostream & os,const IndexTableItem& o);
	friend RCBinaryStream& operator<<(RCBinaryStream & os,const IndexTableItem& o);
public:
	IndexTableItem(ResourceHeader* aResource);
	~IndexTableItem();
	void SetOffset(unsigned long aOffsetToSet);
	ResourceHeader& Resource() {return *iResource;}
public:
	ResourceHeader* iResource;
	unsigned char iOffset[2];
};

class IndexTable : public Array
{
	friend std::ostream& operator<<(std::ostream& os,const IndexTable& o);
	friend RCBinaryStream& operator<<(RCBinaryStream& os,const IndexTable& o);
public:
	IndexTable();
	~IndexTable();
	void Add(ResourceHeader* aResource);
	void SetIndexOffset(unsigned long aOffset);
private:
	static int iInUse;
	unsigned char iOffset[2];
};

class IndexTableIterator : public ArrayIterator
{
public:
	IndexTableIterator(const IndexTable & aTable);
	IndexTableItem* operator()();
};
