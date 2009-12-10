// STRUCTST.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <stdlib.h>
#include "structst.h"
#include "resource.h"
#include "tokens.h"
#include "errorhan.h"

// StructItem

StructItem::StructItem(String aLabelToSet):
iLabel(aLabelToSet)
{}

StructItem::~StructItem()
{}

StructItem::StructItem(StructItem& /*aSource*/)
{
	assert(0);
}

// SimpleStructItem

SimpleStructItem::SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet):
StructItem(aLabelToSet),
iItemType(aItemTypeToSet)
{
	switch(iItemType)
	{
	case L_BYTE:
	case L_WORD:
	case L_LONG:
		iDefault = "0";
		break;
	case L_DOUBLE:
		iDefault = "0.0";
		break;
	}
}

SimpleStructItem::SimpleStructItem(DataType aItemTypeToSet,String aLabelToSet,String aMaxLength):
StructItem(aLabelToSet),
iItemType(aItemTypeToSet),
iLengthLimit(aMaxLength)
{
	switch(iItemType)
	{
	case L_BYTE:
	case L_WORD:
	case L_LONG:
		iDefault = "0";
		break;
	case L_DOUBLE:
		iDefault = "0.0";
		break;
	}
}

ResourceItem* SimpleStructItem::NewResourceItem()
{
	SimpleResourceItem* item=new SimpleResourceItem(this);
	return(item);
}

SimpleStructItem::~SimpleStructItem()
{}

SimpleStructItem::SimpleStructItem(SimpleStructItem& aSource):
StructItem(aSource)
{
	assert(0);
}

SimpleStructItem& SimpleStructItem::operator=(SimpleStructItem& /*Source*/)
{
	assert(0);
	return *this;
}

// ArrayStructItem

ArrayStructItem::ArrayStructItem(DataType aItemTypeToSet,String aLabelToSet):
StructItem(aLabelToSet),
iItemType(aItemTypeToSet),
iLenType(0)
{}

ArrayStructItem::~ArrayStructItem()
{
	iDefaults.DeleteAll();
}

ResourceItem * ArrayStructItem::NewResourceItem()
{
	return new ArrayResourceItem( this);
}

// StructTypeStructItem

StructTypeStructItem::StructTypeStructItem(String aLabelToSet):
StructItem(aLabelToSet)
{}

ResourceItem * StructTypeStructItem::NewResourceItem()
{
	return new StructTypeResourceItem( this);
}

// StructArrayStructItem

StructArrayStructItem::StructArrayStructItem(String aLabelToSet):
StructItem(aLabelToSet),
iLenType(0)
{}

StructArrayStructItem::StructArrayStructItem(String aLabelToSet,String aSizeToSet):
StructItem(aLabelToSet),
iLenType(0),
iSize(aSizeToSet)
{}

ResourceItem * StructArrayStructItem::NewResourceItem()
{
	return new StructArrayResourceItem( this);
}

StructArrayStructItem::~StructArrayStructItem()
{}

// StructItemArray

StructItemArray::StructItemArray()
{}

StructItemArray::~StructItemArray()
{
	DeleteAll();
}

void StructItemArray::Add( StructItem * pNewItem)
{
	StructItemArrayIterator next( * this);
	StructItem * p;

	while( ( p = next() ) != NULL)
	{
		if ( p->iLabel == pNewItem->iLabel)
		{
			ErrorHandler::OutputErrorLine( "Label already in this structure.");
			exit(1);
		}
	}
	Array::Add( pNewItem);
}

// StructItemArrayIterator

StructItemArrayIterator::StructItemArrayIterator(const StructItemArray& aArray):
ArrayIterator(aArray)
{}

StructItem* StructItemArrayIterator::operator()()
{
	return ( StructItem *) ArrayIterator::operator()();
}

// StructHeader

StructHeader::StructHeader(String aLabelToSet):
iLabel(aLabelToSet),
iLenType( 0)
{}

StructHeader::StructHeader(String aLabelToSet,DataType aLenTypeToSet):
iLabel(aLabelToSet),
iLenType(aLenTypeToSet)
{}

// StructHeaderArray

int StructHeaderArray::iInUse = 0;

StructHeaderArray::StructHeaderArray()
{
	assert( iInUse == 0);
	iInUse = 1;
}

StructHeaderArray::~StructHeaderArray()
{
	assert( iInUse == 1);
	iInUse = 0;
	DeleteAll();
}

void StructHeaderArray::Add( StructHeader * pNewItem)
{
	StructHeaderArrayIterator next( * this);
	StructHeader * p;

	while( ( p = next() ) != NULL)
	{
		if ( p->iLabel == pNewItem->iLabel)
		{
			ErrorHandler::OutputErrorLine( "STRUCT with this name already stored");
			exit(1);
		}
	}
	Array::Add( pNewItem);
}

StructHeader * StructHeaderArray::Find( const String & LabelSought)
{
	StructHeaderArrayIterator next( * this);
	StructHeader * p;

	while( ( p = next() ) != NULL)
		if ( p->iLabel == LabelSought)
			return p;

	ErrorHandler::OutputErrorLine( "Label not found");
	exit(1);
}

// StructHeaderArrayIterator

StructHeaderArrayIterator::StructHeaderArrayIterator(const StructHeaderArray& aArray):
ArrayIterator(aArray)
{}

StructHeader * StructHeaderArrayIterator::operator()()
{
	return (StructHeader*)ArrayIterator::operator()();
}
