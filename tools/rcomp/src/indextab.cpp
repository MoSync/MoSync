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


#include <assert.h>
#include <stdlib.h>
#include "errorhan.h"
#include "indextab.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::hex;
using std::dec;
using std::endl;
using std::ostream;
#endif //__MSVCDOTNET__

int IndexTable::iInUse = 0;

IndexTableItem::IndexTableItem(ResourceHeader* aResource)
	{
	iResource=aResource;
	iOffset[0] = '\0';
	iOffset[1] = '\0';
	}

IndexTableItem::~IndexTableItem()
	{
	delete iResource;
	}

void IndexTableItem::SetOffset(unsigned long aOffsetToSet)
	{
	assert(aOffsetToSet <= 0xFFFF);
	
	iOffset[0] = char(aOffsetToSet & 0xFF);
	iOffset[1] = char((aOffsetToSet & 0xFF00)>>8);
	}

RCBinaryStream & operator<< ( RCBinaryStream & os, const IndexTableItem & o)
	{
	os.Write( o.iOffset, 2);
	return os;
	}

ostream & operator<< ( ostream & os, const IndexTableItem & o)
	{
	unsigned long offset = (o.iOffset[1]<<8)|o.iOffset[0];
	os << "IndexTableItem " << hex << offset << dec << "\t" << o.iResource->iLabel << endl;
	return os;
	}

IndexTable::IndexTable()
	{
	assert( !iInUse);
	iInUse = 1;		// Only one instance can be created at a time. This is because the class
	}				// manages a lump of memory which is freed on destruction.

IndexTable::~IndexTable()
	{
	assert( iInUse);
	DeleteAll();
	iInUse = 0;
	}

void IndexTable::Add( ResourceHeader* aResource)
	{
	Array::Add( new IndexTableItem( aResource) );
	}

void IndexTable::SetIndexOffset(unsigned long aOffsetToSet)
	{
	assert(aOffsetToSet <= 0xFFFF);
	
	iOffset[0] = char(aOffsetToSet & 0xFF);
	iOffset[1] = char((aOffsetToSet & 0xFF00)>>8);
	}


ostream & operator<< ( ostream & os, const IndexTable & o)
	{
	os << "IndexTable" << endl;
	
	if ( o.Size() == 0)
		{
		os << "<None>" << endl;
		return os;
		}
		
	IndexTableIterator next( o);
	IndexTableItem * p;
	
	while( ( p = next() ) != NULL)
		os << * p;

	unsigned long offset = (o.iOffset[1]<<8)|o.iOffset[0];
	os << "IndexTable     " << hex << offset << dec << endl;
	return os;
	}

RCBinaryStream & operator<< ( RCBinaryStream & os, const IndexTable & o)
	{
	if ( o.Size() == 0)
		return os;
		
	IndexTableIterator next( o);
	IndexTableItem * p;
	
	while( ( p = next() ) != NULL)
		os << * p;

	os.Write( o.iOffset, 2);
	return os;
	}

IndexTableIterator::IndexTableIterator(const IndexTable & a):
	ArrayIterator(a)
	{}

IndexTableItem * IndexTableIterator::operator() ()
	{
	return (IndexTableItem *) ArrayIterator::operator() ();
	}
