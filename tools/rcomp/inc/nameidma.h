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


#ifndef _NAMEIDMA_H_
#define _NAMEIDMA_H_

#include "array.h"  
#include "astring.h"

class NameIdItem : public ArrayItem
	{
	friend ostream& operator<<(ostream & os, NameIdItem & o);
public:
	NameIdItem(String aNameToSet,unsigned long aIdToSet);
public:
	String iName;
	unsigned long iId;
	};

class NameIdMap : public Array
	{
	friend ostream& operator<<(ostream & os,NameIdMap& aMap);
public:
	NameIdMap();
	~NameIdMap();
	void Add( String Name, unsigned long aId);
	unsigned long FindId( String Name) const;
	int IsStored( String Name) const;

	};

class NameIdMapIterator : public ArrayIterator
	{
public:
	NameIdMapIterator( const NameIdMap & c);
	NameIdItem * operator() ();
	};

#endif
