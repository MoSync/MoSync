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
#include <stdio.h>
#include "resource.h"
#include "tokens.h"
#include "errorhan.h"

// ResourceItem

ResourceItem::ResourceItem(const String& aLabelToSet, int aResourceItemType):
	iLabel(aLabelToSet),
	iLineNumber(0),
	iResourceItemType(aResourceItemType)
	{}

ResourceItem & ResourceItem::operator= ( const ResourceItem &)
	{
	assert(0);
	return * this;
	}

void ResourceItem::Set(const String* aFileNameToSet, int aLineNumberToSet)
	{
	iFileName = aFileNameToSet;
	iLineNumber = aLineNumberToSet;
	}

void ResourceItem::RegisterErrorLocation()
	{
	ErrorHandler::Register(iFileName,iLineNumber);
	}

int ResourceItem::GetResourceItemType()
	{
	return iResourceItemType;
	}

String ResourceItem::GetLabel()
	{
	return iLabel;
	}

/***************************************************************/
/* SimpleResourceItem                                          */
/***************************************************************/
SimpleResourceItem::SimpleResourceItem(SimpleStructItem* aItem):
	ResourceItem(aItem->iLabel, ESimpleResourceItem),
	iStructItem(aItem),
	iValueSet(0)
	{}	

void SimpleResourceItem::Set(const String& aValueToSet)
	{
	ResourceItem::Set( ErrorHandler::GetFileName(), ErrorHandler::GetLineNumber() );
	
	unsigned long lengthLimit = 0;	// max length of string, 0 = unchecked
	switch ( iStructItem->iItemType)
		{
		case L_LTEXT8:
		case L_LTEXT16:
			lengthLimit = 255;	// length encoded as a single byte
			// fall through...
		case L_TEXT8:
		case L_BUF8:
		case L_TEXT16:
		case L_BUF16:
			if ( iStructItem->iLengthLimit.Length() > 0)
			{
				NumericValue limit(iStructItem->iLengthLimit, L_LONG);
				lengthLimit = limit.GetULong();
			}
			if ( lengthLimit && aValueToSet.ExportLength(TargetCharacterSet,SourceCharacterSet) > lengthLimit )
			{
				char buf[256];
				sprintf(buf, "Text length exceeds specified limit of %lx", lengthLimit);
				ErrorHandler::OutputErrorLine( buf);
				exit(1);
			}
		}
	
	iValue = aValueToSet;
	iValueSet = 1;
	}

void SimpleResourceItem::Set(const StringArray& /*aValues*/)
	{
	// Can't store array of values
	ErrorHandler::OutputErrorLine( "SimpleResourceItem cannot hold array of values");
	exit(1);
	}

ResourceItemArray * SimpleResourceItem::GetRIA()
	{
	assert(0);	// Don't have RIA in this class.
	return NULL;
	}

void SimpleResourceItem::AddDefault()
	{
	if(!iValueSet)
		iValue = iStructItem->iDefault;
	}

void SimpleResourceItem::SetSRLink(unsigned long aSRLinkToSet)
	{
	if(iStructItem->iItemType == L_SRLINK)
		iLinkValue = aSRLinkToSet;
	}

// ArrayResourceItem

ArrayResourceItem::ArrayResourceItem( ArrayStructItem * p):
	ResourceItem( p->iLabel, EArrayResourceItem),
	iStructItem(p)
	{}	

ArrayResourceItem::~ArrayResourceItem()
	{
	iValues.DeleteAll();
	}

void ArrayResourceItem::Set(const String& /*aValue*/)
	{
	// Can't store single value
	ErrorHandler::OutputErrorLine( "ArrayResourceItem requires array of values");
	exit(1);
	}

void ArrayResourceItem::Set(const StringArray& aValuesToSet)
	{
	ResourceItem::Set(ErrorHandler::GetFileName(),ErrorHandler::GetLineNumber());
	iValues = aValuesToSet;
	}

ResourceItemArray * ArrayResourceItem::GetRIA()
	{
	assert(0);	// Don't have RIA in this class.
	return NULL;
	}

void ArrayResourceItem::AddDefault()
	{
	StringArrayIterator NextActual(iValues);
	String* actualvalue;
	StringArrayIterator NextDefault( iStructItem->iDefaults);
	String* defaultvalue;
	// Iterate through actual values and default values.
	while((actualvalue = NextActual()) != NULL)
		{
		if((defaultvalue = NextDefault()) == NULL)
			return;	// Stop if end of defaults reached.
		}
	// Having reached the end of the actual values see if there are any
	// default values to add.
	while((defaultvalue = NextDefault()) != NULL)
		iValues.Add(new String (*defaultvalue));
	}

void ArrayResourceItem::SetSRLink(unsigned long /*aSRLinkToSet*/)
	{}

// StructTypeResourceItem

StructTypeResourceItem::StructTypeResourceItem(StructTypeStructItem* p):
	ResourceItem( p->iLabel, EStructTypeResourceItem),
	iStructItem(p)
	{}	

void StructTypeResourceItem::Set(const String& aStructName)
	{
	ResourceItem::Set(ErrorHandler::GetFileName(),ErrorHandler::GetLineNumber());
	iResourceItems.FillFromStruct(aStructName);
	}

void StructTypeResourceItem::Set(const StringArray& /*aValues*/)
	{
	// Can't store array of values
	ErrorHandler::OutputErrorLine( "StructTypeResourceItem cannot hold array of values");
	exit(1);
	}

ResourceItemArray * StructTypeResourceItem::GetRIA()
	{
	return& iResourceItems;
	}

void StructTypeResourceItem::AddDefault()
	{
	ResourceItemArrayIterator NextActualItem(iResourceItems);
	ResourceItem * pActualItem;
	
	// Add defaults to each of the items already set up.
	while( ( pActualItem = NextActualItem() ) != NULL)
		pActualItem->AddDefault();
	}

void StructTypeResourceItem::SetSRLink(unsigned long /*aSRLinkToSet*/)
	{}

// StructArrayResourceItem

StructArrayResourceItem::StructArrayResourceItem(StructArrayStructItem* p):
	ResourceItem(p->iLabel, EStructArrayResourceItem),
	iStructItem(p),
	iLastRIA( NULL)
	{}	

void StructArrayResourceItem::Set( const String & StructName)
	{
	ResourceItem::Set( ErrorHandler::GetFileName(), ErrorHandler::GetLineNumber() );
	ResourceItemArray* p = new ResourceItemArray;
	iArrayOfResourceItemArrays.Add(p);
	p->FillFromStruct( StructName);
	iLastRIA = p;
	}

void StructArrayResourceItem::Set(const StringArray& /*aValues*/)
	{}

ResourceItemArray * StructArrayResourceItem::GetRIA()
	{
	return iLastRIA;
	}

void StructArrayResourceItem::AddDefault()
	{
	ResourceItemArrayArrayIterator next(iArrayOfResourceItemArrays);
	ResourceItemArray * p;
	while( ( p = next() ) != NULL)
		p->AddDefault();
	}

void StructArrayResourceItem::SetSRLink(unsigned long /*aSRLinkToSet*/)
	{}

// ResourceItemArray

ResourceItemArray::ResourceItemArray():
	iLenType(0)
	{}
	
ResourceItemArray::~ResourceItemArray()
	{
	DeleteAll();
	}
	
void ResourceItemArray::Add( ResourceItem * p)
	{
	Array::Add( p);
	}

void ResourceItemArray::FillFromStruct(const String& aStructName)
	{
	extern StructHeaderArray * pSHA;
	StructHeader * pSH = pSHA->Find(aStructName);
	StructItemArrayIterator next( pSH->iSIA);
	StructItem * pItem;
	while ( ( pItem = next() ) != NULL)
		Add( pItem->NewResourceItem() );
	iLenType = pSH->iLenType;
	}

void ResourceItemArray::Set( const String & Label, const String & Value)
	{
	ResourceItem * p = Find( Label);
	p->Set( Value);
	}

void ResourceItemArray::Set( const String & Label, const StringArray & Values)
	{
	ResourceItem * p = Find( Label);
	p->Set( Values);
	}

ResourceItem * ResourceItemArray::Find( const String & LabelSought)
	{
	ResourceItemArrayIterator next( * this);
	ResourceItem * p;
	
	while( ( p = next() ) != NULL)
		if ( p->iLabel == LabelSought)
			return p;
	
	ErrorHandler::OutputErrorLine( "Label not found");
	exit(1);
	}

void ResourceItemArray::AddDefault()
	{
	ResourceItemArrayIterator next( * this);
	ResourceItem * p;
	
	while( ( p = next() ) != NULL)
		p->AddDefault();
	}

// ResourceHeader

ResourceHeader::ResourceHeader(String aLabelToSet):
	iLabel(aLabelToSet),
	iLocal(0),
	iResourceId(0xdeaddead),
	iFormatAsHex(0),
	iContainsCompressedUnicode(false)
	{
	}

ResourceHeader::ResourceHeader():
	iLocal(0),
	iResourceId(0xdeaddead),
	iFormatAsHex(0),
	iContainsCompressedUnicode(false)
	{
	}

ResourceHeader::~ResourceHeader()
	{
	}

void ResourceHeader::AddDefault()
	{
	iRIA.AddDefault();
	}

void ResourceHeader::SetResourceId(NameIdMap& aMap, unsigned long aId, int aFormatAsHex)
	{
	iResourceId = aId;
	iFormatAsHex = aFormatAsHex;
	if (iLabel.Length() > 0)
		aMap.Add(iLabel, aId);

	ResourceItemArrayIterator next(iRIA);
	ResourceItem * pItem;
	while( ( pItem = next() ) != NULL)
		pItem->SetSRLink(aId);
	}

void ResourceHeader::StreamOut(RCBinaryStream& aStream, int& aSizeOfLargestResourceWhenUncompressed, const char* aDumpFile)
	{
	ResourceDataStream stream;
	iRIA.StreamOut(stream);
	if (aDumpFile!=NULL)
		{
		// dump the resource to aDumpFile in its uncompressed and unpadded state - LINKs, LLINKs, etc will be undefined
		stream.Dump(aDumpFile);
		}
	int sizeWhenUncompressed=0;
	iContainsCompressedUnicode=stream.StreamOutReturningWhetherContainsCompressedUnicode(aStream, sizeWhenUncompressed);
	// this check is unnecessary, zero size resources are allowed so
	// there is no benefit to this assert. 

	if (aSizeOfLargestResourceWhenUncompressed<sizeWhenUncompressed)
		{
		aSizeOfLargestResourceWhenUncompressed=sizeWhenUncompressed;
		}
	}

// ResourceItemArrayArray

ResourceItemArrayArray::ResourceItemArrayArray()
	{}

ResourceItemArrayArray::~ResourceItemArrayArray()
	{
	DeleteAll();
	}
	
void ResourceItemArrayArray::Add( ResourceItemArray * pNewItem)
	{
	Array::Add( pNewItem);
	}

// ResourceItemArrayIterator

ResourceItemArrayIterator::ResourceItemArrayIterator( const ResourceItemArray & c):
	ArrayIterator( c)
	{}

ResourceItem * ResourceItemArrayIterator::operator()()
	{
	return (ResourceItem *) ArrayIterator::operator()();
	}

// ResourceItemArrayArrayIterator

ResourceItemArrayArrayIterator::ResourceItemArrayArrayIterator(const ResourceItemArrayArray & c):
	ArrayIterator( c)
	{}

ResourceItemArray* ResourceItemArrayArrayIterator::operator()()
	{
	return (ResourceItemArray*) ArrayIterator::operator()();
	}
