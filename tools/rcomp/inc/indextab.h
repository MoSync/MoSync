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


#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
using std::ostream;
#else //!__MSVCDOTNET__
#include <iostream.h>
#endif //__MSVCDOTNET__

#include "rcbinstr.h"
#include "array.h"
#include "resource.h"


class IndexTableItem : public ArrayItem
	{
	friend ostream& operator<<(ostream & os,const IndexTableItem& o);
	friend RCBinaryStream& operator<<(RCBinaryStream & os,const IndexTableItem& o);
public:
	IndexTableItem(ResourceHeader* aResource);
	~IndexTableItem();
	void SetOffset(unsigned long aOffsetToSet);
	ResourceHeader& Resource() {return *iResource;}
public:
	ResourceHeader* iResource;
	unsigned char iOffset[2];
	};

class IndexTable : public Array
	{
	friend ostream& operator<<(ostream& os,const IndexTable& o);
	friend RCBinaryStream& operator<<(RCBinaryStream& os,const IndexTable& o);
public:
	IndexTable();
	~IndexTable();
	void Add(ResourceHeader* aResource);
	void SetIndexOffset(unsigned long aOffset);
private:
	static int iInUse;
	unsigned char iOffset[2];
	};

class IndexTableIterator : public ArrayIterator
	{
public:
	IndexTableIterator(const IndexTable & aTable);
	IndexTableItem* operator()();
	};
