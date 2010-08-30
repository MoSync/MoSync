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
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "nameidma.h"
#include "errorhan.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::endl;
#endif //__MSVCDOTNET__

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

