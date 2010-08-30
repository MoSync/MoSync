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
#include "linkarra.h"
#include "nameidma.h"
#include "rcbinstr.h"
#include "tokens.h"  
#include "mem.h"     
#include "errorhan.h"
#include "rcbinstr.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::cout;
using std::cerr;
using std::endl;
#endif //__MSVCDOTNET__
                
extern NameIdMap* pResourceNameIds;
extern int verbose;
extern RCTypeArray gTypes;

int LinkArray::iInUse = 0;

LinkArray::LinkArray()
	{
	assert(!iInUse);
	iInUse = 1;
	}

LinkArray::~LinkArray()
	{
	assert(iInUse);
	DeleteAll();
	}

void LinkArray::Add( LinkItem * pNewItem)
	{
	Array::Add( pNewItem);
	}

void LinkArray::OverwriteLinks ( RCBinaryStream & os ) const
	{
	if ( verbose) { MOFF; cout << "Writing in correct links." << endl; MON;}
	LinkArrayIterator next( *this);
	LinkItem * p;
	while ( ( p = next() ) != NULL)
		p->OverwriteLink( os);
	}

LinkItem::LinkItem(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber):
	iStreamPosition(0xeebbeebb), // garbage initialization
	iResourceName(aResourceName),
	iFileName(aFileName),
	iLineNumber(aLineNumber)
	{
	aStream.EnquireStreamPositionWhenKnown(iStreamPosition); // causes iStream to be correctly set at some point later
	}

int WordLink::NumberOfBytesOccupiesInStream()
	{ // static
	return gTypes.GetSize(L_WORD);
	}

int LongLink::NumberOfBytesOccupiesInStream()
	{ // static
	return gTypes.GetSize(L_LONG);
	}

void WordLink::OverwriteLink( RCBinaryStream & os ) const
	{
	if ( verbose) { MOFF; cout << "Writing WORD link: " << iResourceName << "\t" << iStreamPosition << endl; MON;}
	os.SetPosition(iStreamPosition);
	ErrorHandler::Register(&iFileName,iLineNumber);
	os << NumericValue(pResourceNameIds->FindId(iResourceName),L_WORD);
	}

void LongLink::OverwriteLink( RCBinaryStream & os ) const
	{
	if ( verbose) { MOFF; cout << "Writing LONG link: " << iResourceName << "\t" << iStreamPosition << endl; MON;}
	os.SetPosition(iStreamPosition);
	ErrorHandler::Register(&iFileName,iLineNumber);
	os << NumericValue(pResourceNameIds->FindId(iResourceName),L_LONG);
	}

LinkArrayIterator::LinkArrayIterator(const LinkArray& c):
	ArrayIterator(c)
	{}

LinkItem * LinkArrayIterator::operator() ()
	{
	return (LinkItem *) ArrayIterator::operator() ();
	}

WordLink::WordLink(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber):
	LinkItem(aStream,aResourceName,aFileName,aLineNumber)
	{}

LongLink::LongLink(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber):
	LinkItem(aStream,aResourceName,aFileName,aLineNumber)
	{}

ostream & WordLink::StreamOut ( ostream & os)
	{
	os << "WordLink: " << iStreamPosition << "\t" << iResourceName << endl;
	
	return os;
	}

ostream & LongLink::StreamOut( ostream & os)
	{
	os << "LongLink: " << iStreamPosition << "\t" << iResourceName << endl;
	
	return os;
	}

ostream & operator<< ( ostream & os, LinkArray & o)
	{
	os << "LinkArray *******" << endl;
	
	LinkArrayIterator next( o);
	LinkItem * p;
	
	while( ( p = next() ) != NULL)
		p->StreamOut( os);
		
	return os;
	}
