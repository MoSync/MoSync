// LINKARRA.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include "linkarra.h"
#include "nameidma.h"
#include "rcbinstr.h"
#include "tokens.h"
#include "mem.h"
#include "errorhan.h"
#include "rcbinstr.h"

using namespace std;

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
iResourceName(aResourceName),
iStreamPosition(0xeebbeebb), // garbage initialization
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
	ErrorHandler::Register(iFileName,iLineNumber);
	os << NumericValue(pResourceNameIds->FindId(iResourceName),L_WORD);
}

void LongLink::OverwriteLink( RCBinaryStream & os ) const
{
	if ( verbose) { MOFF; cout << "Writing LONG link: " << iResourceName << "\t" << iStreamPosition << endl; MON;}
	os.SetPosition(iStreamPosition);
	ErrorHandler::Register(iFileName,iLineNumber);
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
