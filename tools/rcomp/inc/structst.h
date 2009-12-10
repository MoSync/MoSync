// STRUCTST.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __STRUCTST_H__
#define __STRUCTST_H__

#include <iostream>
#include "stringar.h"
#include "datatype.h"

class ResourceItem;

// StructItem

class StructItem : public ArrayItem
{
public:
	virtual ResourceItem * NewResourceItem() = 0;
	virtual std::ostream& StreamOut ( std::ostream & os) = 0;
protected:
	StructItem( String LabelToSet);
	virtual ~StructItem();
	StructItem( StructItem & Source);
public:
	String	iLabel;
};

// SimpleStructItem

class SimpleStructItem : public StructItem	// e.g. WORD a = 5
{
	friend std::ostream& operator<< ( std::ostream & os, SimpleStructItem & o);
public:
	SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet);
	SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet,String aMaxLength);
	ResourceItem * NewResourceItem();
	std::ostream& StreamOut ( std::ostream & os);
	virtual ~SimpleStructItem();
	SimpleStructItem( SimpleStructItem & Source);
	SimpleStructItem& operator=( SimpleStructItem & Source);
public:
	DataType iItemType;
	String iDefault;
	String iLengthLimit;
};

std::ostream& operator<< ( std::ostream & os, SimpleStructItem & o);

// ArrayStructItem

class ArrayStructItem : public StructItem	// e.g. WORD a[] = {1,2,3}
{
	friend std::ostream& operator<< ( std::ostream & os, ArrayStructItem & o);
public:
	ArrayStructItem( DataType ItemTypeToSet, String LabelToSet);
	virtual ~ArrayStructItem();
	ResourceItem * NewResourceItem();
	std::ostream& StreamOut ( std::ostream & os);
public:
	DataType iItemType;
	StringArray iDefaults;
	DataType iLenType;
	String iSize;
};

std::ostream& operator<< ( std::ostream & os, ArrayStructItem & o);

// StructTypeStructItem

class StructTypeStructItem : public StructItem	// e.g. STRUCT a
{
	friend std::ostream& operator<< ( std::ostream & os, StructTypeStructItem & o);
public:
	StructTypeStructItem( String LabelToSet);
	ResourceItem * NewResourceItem();
	std::ostream& StreamOut ( std::ostream & os);
};

std::ostream& operator<< ( std::ostream & os, StructTypeStructItem & o);

// StructArrayStructItem

class StructArrayStructItem : public StructItem // e.g. STRUCT a[]
{
	friend std::ostream& operator<< ( std::ostream & os, StructArrayStructItem & o);
public:
	StructArrayStructItem( String LabelToSet);
	StructArrayStructItem( String LabelToSet, String SizeToSet);
	ResourceItem * NewResourceItem();
	std::ostream& StreamOut ( std::ostream & os);
	virtual ~StructArrayStructItem();
public:
	DataType iLenType;
	String iSize;
};

std::ostream& operator<< ( std::ostream & os, StructArrayStructItem & o);

// StructItemArray

class StructItemArray : public Array
{
	friend std::ostream& operator<< ( std::ostream & os, StructItemArray & o);
public:
	StructItemArray();
	~StructItemArray();
	void Add( StructItem * pNewItem);
};

std::ostream& operator<< ( std::ostream & os, StructItemArray & o);

// StructItemArrayIterator

class StructItemArrayIterator : public ArrayIterator
{
public:
	StructItemArrayIterator( const StructItemArray & c);
	StructItem * operator() ();
};

// StructHeader

class StructHeader : public ArrayItem
{
	friend std::ostream& operator<< ( std::ostream & os, StructHeader & o);
public:
	StructHeader( String LabelToSet);
	StructHeader( String LabelToSet, DataType LenTypeToSet);
public:
	String iLabel;
	DataType iLenType;
	StructItemArray iSIA;
};

std::ostream& operator<< ( std::ostream & os, StructHeader & o);

// StructHeaderArray

class StructHeaderArray : public Array
{
	friend std::ostream& operator<< ( std::ostream & os, StructHeaderArray & o);
public:
	StructHeaderArray();
	~StructHeaderArray();
	void Add( StructHeader * pNewItem);
	StructHeader * Find( const String & LabelSought);
private:
	static int iInUse;	// Only one instance of this class may exist at a time.
};

std::ostream& operator<< ( std::ostream & os, StructHeaderArray & o);

// StructHeaderArrayIterator

class StructHeaderArrayIterator : public ArrayIterator
{
public:
	StructHeaderArrayIterator( const StructHeaderArray& c);
	StructHeader * operator() ();
};

#endif

