/*
* Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#ifndef __STRUCTST_H__
#define __STRUCTST_H__

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
using std::ostream;
#else //!__MSVCDOTNET__
#include <iostream.h>
#endif //__MSVCDOTNET__

#include "stringar.h"
#include "datatype.h"

class ResourceItem;

// StructItem

class StructItem : public ArrayItem
	{
public:
	virtual ResourceItem * NewResourceItem() = 0;
	virtual ostream& StreamOut ( ostream & os) = 0;
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
	friend ostream& operator<< ( ostream & os, SimpleStructItem & o);
public:
	SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet);
	SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet,String aMaxLength);
	ResourceItem * NewResourceItem();
	ostream& StreamOut ( ostream & os);
	virtual ~SimpleStructItem();
	SimpleStructItem( SimpleStructItem & Source);
	SimpleStructItem& operator=( SimpleStructItem & Source);
public:
	DataType iItemType;
	String iDefault;
	String iLengthLimit;
	};

// ArrayStructItem

class ArrayStructItem : public StructItem	// e.g. WORD a[] = {1,2,3}
	{
	friend ostream& operator<< ( ostream & os, ArrayStructItem & o);
public:
	ArrayStructItem( DataType ItemTypeToSet, String LabelToSet);
	virtual ~ArrayStructItem();
	ResourceItem * NewResourceItem();
	ostream& StreamOut ( ostream & os);
public:
	DataType iItemType;
	StringArray iDefaults;
	DataType iLenType;
	String iSize;
	};

// StructTypeStructItem

class StructTypeStructItem : public StructItem	// e.g. STRUCT a
	{
	friend ostream& operator<< ( ostream & os, StructTypeStructItem & o);
public:
	StructTypeStructItem( String LabelToSet);
	ResourceItem * NewResourceItem();
	ostream& StreamOut ( ostream & os);
	};

// StructArrayStructItem

class StructArrayStructItem : public StructItem // e.g. STRUCT a[]
	{
	friend ostream& operator<< ( ostream & os, StructArrayStructItem & o);
public:
	StructArrayStructItem( String LabelToSet);
	StructArrayStructItem( String LabelToSet, String SizeToSet);
	ResourceItem * NewResourceItem();
	ostream& StreamOut ( ostream & os);
	virtual ~StructArrayStructItem();
public:
	DataType iLenType;
	String iSize;
	};

// StructItemArray

class StructItemArray : public Array
	{
	friend ostream& operator<< ( ostream & os, StructItemArray & o);
public:
	StructItemArray();
	~StructItemArray();
	void Add( StructItem * pNewItem);
	};

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
	friend ostream& operator<< ( ostream & os, StructHeader & o);
public:
	StructHeader( String LabelToSet);
	StructHeader( String LabelToSet, DataType LenTypeToSet);
public:
	String iLabel;
	DataType iLenType;
	StructItemArray iSIA;
	};

// StructHeaderArray

class StructHeaderArray : public Array
	{
	friend ostream& operator<< ( ostream & os, StructHeaderArray & o);
public:
	StructHeaderArray();
	~StructHeaderArray();
	void Add( StructHeader * pNewItem);
	StructHeader * Find( const String & LabelSought);
private:
	static int iInUse;	// Only one instance of this class may exist at a time.
	};

// StructHeaderArrayIterator

class StructHeaderArrayIterator : public ArrayIterator
	{
public:
	StructHeaderArrayIterator( const StructHeaderArray& c);
	StructHeader * operator() ();
	};

#endif

