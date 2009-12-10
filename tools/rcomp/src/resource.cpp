// RESOURCE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <stdlib.h>
#include "resource.h"
#include "tokens.h"
#include "errorhan.h"

// ResourceItem

ResourceItem::ResourceItem(const String& aLabelToSet):
iLabel(aLabelToSet),
iLineNumber(0)
{}

ResourceItem & ResourceItem::operator= ( const ResourceItem &)
{
	assert(0);
	return * this;
}

void ResourceItem::Set(String aFileNameToSet,int aLineNumberToSet)
{
	iFileName = aFileNameToSet;
	iLineNumber = aLineNumberToSet;
}

void ResourceItem::RegisterErrorLocation()
{
	ErrorHandler::Register(iFileName,iLineNumber);
}

/***************************************************************/
/* SimpleResourceItem                                          */
/***************************************************************/
SimpleResourceItem::SimpleResourceItem(SimpleStructItem* aItem):
ResourceItem(aItem->iLabel),
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
			sprintf(buf, "Text length exceeds specified limit of %ld", lengthLimit);
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
ResourceItem( p->iLabel),
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
		iValues.Add(defaultvalue);
}

void ArrayResourceItem::SetSRLink(unsigned long /*aSRLinkToSet*/)
{}

// StructTypeResourceItem

StructTypeResourceItem::StructTypeResourceItem(StructTypeStructItem* p):
ResourceItem( p->iLabel),
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
ResourceItem(p->iLabel),
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

void ResourceHeader::StreamOut(RCBinaryStream& aStream, int& aSizeOfLargestResourceWhenUncompressed)
{
	ResourceDataStream stream;
	iRIA.StreamOut(stream);
	int sizeWhenUncompressed=0;
	iContainsCompressedUnicode=stream.StreamOutReturningWhetherContainsCompressedUnicode(aStream, sizeWhenUncompressed);
	/*
	* alfredh: had to comment out the following line otherwise the
	*          program would not work. someone please enlighten me.
	*
	*          this snippet part of an .rss file will trigger the assert.
	*          however, it looks like the size here is '0' - hence valid.
	*
	*          RESOURCE TBUF { buf=""; }
	*/
	//	assert(sizeWhenUncompressed>0); TODO
	if (aSizeOfLargestResourceWhenUncompressed<sizeWhenUncompressed)
	{
		aSizeOfLargestResourceWhenUncompressed=sizeWhenUncompressed;
	}
}

void ResourceHeader::StreamOutDump(RCBinaryStream& aStream)
{
	ResourceDataStream stream;
	iRIA.StreamOut(stream);
	int sizeWhenUncompressed=0;
	iContainsCompressedUnicode=stream.StreamOutReturningWhetherContainsCompressedUnicode(aStream, sizeWhenUncompressed, true);
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
