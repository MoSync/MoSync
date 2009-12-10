// NAMEIDMA.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameidma.h"
#include "errorhan.h"

using namespace std;

#if defined(__VC32__)
#pragma warning( disable : 4702 )	// unreachable code
#endif

NameIdMap::NameIdMap()
{}

NameIdMap::~NameIdMap()
{
	DeleteAll();
}

void NameIdMap::Add(String aName, unsigned long aId)
{
	Array::Add(new NameIdItem(aName,aId));
}

unsigned long NameIdMap::FindId(String aNameSought) const
{
	NameIdMapIterator next(*this);
	NameIdItem* p;
	while((p = next()) != NULL)
		if(p->iName == aNameSought)
			return p->iId;
	ErrorHandler::OutputErrorLine("Link name not found");
	exit(1);
	// C4702: unreachable code			
}

int NameIdMap::IsStored(String aNameSought) const
{
	NameIdMapIterator next( * this);
	NameIdItem * p;
	while((p = next())!=NULL)
		if(p->iName == aNameSought)
			return 1;
	return 0;
}

ostream & operator<<(ostream& os,NameIdMap& o)
{
	os << "ResourceNameIds ******" << endl;
	NameIdMapIterator next(o);
	NameIdItem* p;
	while((p = next() ) != NULL)
		os << *p;
	return os;
}

//

NameIdItem::NameIdItem(String aNameToSet,unsigned long aIdToSet):
iName(aNameToSet),
iId(aIdToSet)
{}

ostream & operator<<(ostream& os,NameIdItem& o)
{
	os << "NameIdItem " << o.iName << "\t" << o.iId << endl;
	return os;
}

NameIdMapIterator::NameIdMapIterator(const NameIdMap& aMap):
ArrayIterator(aMap)
{}

NameIdItem* NameIdMapIterator::operator()()
{
	return (NameIdItem*)ArrayIterator::operator()();
}

