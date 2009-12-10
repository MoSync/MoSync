// LINKARRA.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "array.h"
#include "astring.h"

class RCBinaryStream;
class ResourceDataStream;

class LinkItem : public ArrayItem
{
public:
	LinkItem(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber);
	virtual void OverwriteLink(RCBinaryStream& os) const=0;
	virtual std::ostream& StreamOut(std::ostream& os)=0;
public:
	String iResourceName;	// Name of resource to which link points.
	unsigned long iStreamPosition;
	String iFileName;
	int iLineNumber;
};

class LinkArray : public Array
{
	friend std::ostream& operator<<(std::ostream& os,LinkArray& o);
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
	virtual std::ostream& StreamOut(std::ostream& os);
	static int NumberOfBytesOccupiesInStream();
};

class LongLink : public LinkItem
{
public:
	LongLink(ResourceDataStream& aStream,const String& aResourceName,const String& aFileName,int aLineNumber);
	virtual void OverwriteLink(RCBinaryStream& os) const;
	virtual std::ostream& StreamOut(std::ostream& os);
	static int NumberOfBytesOccupiesInStream();
};

class LinkArrayIterator : public ArrayIterator
{
public:
	LinkArrayIterator(const LinkArray& aArray);
	LinkItem* operator()();
};
