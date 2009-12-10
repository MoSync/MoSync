// RESOURCE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "astring.h"
#include "stringar.h"
#include "structst.h"
#include "stack.h"
#include "rcbinstr.h"
#include "nameidma.h"


class ResourceItem;

// ResourceItemArray

class ResourceItemArray : public Array, public StackItem, public ArrayItem
{
	friend std::ostream & operator<< ( std::ostream & os, ResourceItemArray & o);
public:
	ResourceItemArray();
	~ResourceItemArray();
	void Add( ResourceItem * pNewItem);
	void FillFromStruct( const String & StructName);
	void Set( const String & Label, const String & Value);
	void Set( const String & Label, const StringArray & Values);
	ResourceItem * Find( const String & LabelSought);
	void AddDefault();
	void StreamOut(ResourceDataStream& aStream) const;
private:
	DataType iLenType;
};

std::ostream & operator<< ( std::ostream & os, ResourceItemArray & o);

// ResourceItemArrayIterator

class ResourceItemArrayIterator : public ArrayIterator
{
public:
	ResourceItemArrayIterator( const ResourceItemArray & c);
	ResourceItem * operator() ();
};

// ResourceItemArrayArray

class ResourceItemArrayArray : public Array
{
	friend std::ostream & operator<< ( std::ostream & os, ResourceItemArrayArray & o);
public:
	ResourceItemArrayArray();
	~ResourceItemArrayArray();
	void Add( ResourceItemArray * pNewItem);
	void StreamOut(ResourceDataStream& aStream) const;
};

std::ostream & operator<< ( std::ostream & os, ResourceItemArrayArray & o);

// ResourceItemArrayArrayIterator

class ResourceItemArrayArrayIterator : public ArrayIterator
{
public:
	ResourceItemArrayArrayIterator( const ResourceItemArrayArray & c);
	ResourceItemArray * operator() ();
};


// ResourceHeader

class ResourceHeader : public ArrayItem
{
	friend std::ostream & operator<< ( std::ostream & os, ResourceHeader & o);
public:
	ResourceHeader( String LabelToSet);
	ResourceHeader();
	~ResourceHeader();
	void AddDefault();
	void Write( std::ostream &os);
	void SetResourceId(NameIdMap& aMap, unsigned long aId, int aFormatAsHex);
	void StreamOut(RCBinaryStream& aStream, int& aSizeOfLargestResourceWhenUncompressed);
	void StreamOutDump(RCBinaryStream& aStream);
	inline bool ContainsCompressedUnicode() const {return iContainsCompressedUnicode;}
public:
	String iLabel;
	ResourceItemArray iRIA;
	unsigned char iLocal;
	unsigned long iResourceId;
	int iFormatAsHex;
	bool iContainsCompressedUnicode;
};

std::ostream & operator<< ( std::ostream & os, ResourceHeader & o);

// ResourceItem

class ResourceItem : public ArrayItem, public StackItem
{
public:
	virtual ResourceItemArray * GetRIA() = 0;
	virtual void Set( const String & ValueToSet) = 0;
	virtual void Set( const StringArray & ValuesToSet) = 0;
	virtual std::ostream & StreamOut ( std::ostream & os) = 0;
	virtual void StreamOut(ResourceDataStream& aStream) const = 0;
	virtual void AddDefault() = 0;
	virtual void SetSRLink( unsigned long SRLinkToSet) = 0;	
	void Set( String FileNameToSet, int LineNumberToSet);
	void RegisterErrorLocation();
protected:
	ResourceItem( const String & Label);
	ResourceItem & operator= ( const ResourceItem &);
public:
	const String& iLabel;
	String iFileName;
	int iLineNumber;
};

// SimpleResourceItem

class SimpleResourceItem : public ResourceItem
{
public:
	SimpleResourceItem(SimpleStructItem*);
	void Set(const String& aValueToSet);
	void Set(const StringArray& aValuesToSet);
	virtual std::ostream& StreamOut(std::ostream& os);
	friend std::ostream& operator<<(std::ostream& os,SimpleResourceItem & o);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long aSRLinkToSet);
private:
	ResourceItemArray* GetRIA();
public:
	SimpleStructItem* iStructItem;
private:
	String iValue;
	unsigned long iLinkValue;
	unsigned char iValueSet;
};

std::ostream& operator<<(std::ostream& os,SimpleResourceItem & o);

// ArrayResourceItem

class ArrayResourceItem : public ResourceItem
{
	friend std::ostream& operator<<(std::ostream& os,ArrayResourceItem& aItem);
public:
	ArrayResourceItem(ArrayStructItem *);
	~ArrayResourceItem();
	void Set(const String& aValueToSet);
	void Set(const StringArray& aValuesToSet);
	std::ostream & StreamOut(std::ostream& os);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long aSRLinkToSet);
private:
	ResourceItemArray* GetRIA();
public:
	ArrayStructItem* iStructItem;
private:
	StringArray iValues;
};

std::ostream& operator<<(std::ostream& os,ArrayResourceItem& aItem);

// StructTypeResourceItem

class StructTypeResourceItem : public ResourceItem
{
public:
	StructTypeResourceItem(StructTypeStructItem*);
	void Set(const String& ValueToSet);
	void Set(const StringArray& ValuesToSet);
	std::ostream& StreamOut(std::ostream& os);
	friend std::ostream& operator<<(std::ostream& os,StructTypeResourceItem& o);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long aSRLinkToSet);
	ResourceItemArray* GetRIA();
public:
	StructTypeStructItem* iStructItem;
	ResourceItemArray iResourceItems;
};

std::ostream& operator<<(std::ostream& os,StructTypeResourceItem& o);

// StructArrayResourceItem

class StructArrayResourceItem : public ResourceItem
{
	friend std::ostream& operator<<(std::ostream& os,StructArrayResourceItem& aItem);
public:
	StructArrayResourceItem(StructArrayStructItem*);
	void Set(const String& ValueToSet);
	void Set(const StringArray& ValuesToSet);
	ResourceItemArray* GetRIA();
	std::ostream& StreamOut(std::ostream& os);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long SRLinkToSet);
public:
	StructArrayStructItem* iStructItem;
	ResourceItemArrayArray iArrayOfResourceItemArrays;
	ResourceItemArray* iLastRIA;
};

std::ostream& operator<<(std::ostream& os,StructArrayResourceItem& aItem);

#endif
