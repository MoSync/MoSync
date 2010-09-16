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


#include "array.h"  
#include "astring.h"

class RCBinaryStream;
class ResourceDataStream;

class LinkItem : public ArrayItem
	{
public:
	LinkItem(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber);
	virtual void OverwriteLink(RCBinaryStream& os) const=0;
	virtual ostream& StreamOut(ostream& os)=0;
public:
	String iResourceName;	// Name of resource to which link points.
	unsigned long iStreamPosition;
	String iFileName;
	int iLineNumber;
	};

class LinkArray : public Array
	{
	friend ostream& operator<<(ostream& os,LinkArray& o);
public:
	LinkArray();
	~LinkArray();
	void Add(LinkItem* aNewItem);
	void OverwriteLinks ( RCBinaryStream & os ) const;
private:
	static int iInUse;	// Only one instance of this class may exist at a time.
	};

class WordLink : public LinkItem
	{
public:
	WordLink(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber);
	virtual void OverwriteLink(RCBinaryStream& os) const;
	virtual ostream& StreamOut(ostream& os);
	static int NumberOfBytesOccupiesInStream();
	};

class LongLink : public LinkItem
	{
public:
	LongLink(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber);
	virtual void OverwriteLink(RCBinaryStream& os) const;
	virtual ostream& StreamOut(ostream& os);
	static int NumberOfBytesOccupiesInStream();
	};

class LinkArrayIterator : public ArrayIterator
	{
public:
	LinkArrayIterator(const LinkArray& aArray);
	LinkItem* operator()();
	};
