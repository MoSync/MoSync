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


#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "astring.h" 
#include "stringar.h"
#include "structst.h"
#include "stack.h"   
#include "rcbinstr.h"
#include "nameidma.h"


enum { EResourceItem, ESimpleResourceItem, EArrayResourceItem, EStructTypeResourceItem, EStructArrayResourceItem };

class ResourceItem;

// ResourceItemArray

class ResourceItemArray : public Array, public StackItem, public ArrayItem
	{
	friend ostream & operator<< ( ostream & os, ResourceItemArray & o);
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
	friend ostream & operator<< ( ostream & os, ResourceItemArrayArray & o);
public:
	ResourceItemArrayArray();
	~ResourceItemArrayArray();
	void Add( ResourceItemArray * pNewItem);
	void StreamOut(ResourceDataStream& aStream) const;
	};

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
	friend ostream & operator<< ( ostream & os, ResourceHeader & o);
public:
	ResourceHeader( String LabelToSet);
	ResourceHeader();
	~ResourceHeader();
	void AddDefault();
	void Write( ostream &os);
	void SetResourceId(NameIdMap& aMap, unsigned long aId, int aFormatAsHex);
	void StreamOut(RCBinaryStream& aStream, int& aSizeOfLargestResourceWhenUncompressed, const char* aDumpFile);
	inline bool ContainsCompressedUnicode() const {return iContainsCompressedUnicode;}
public:
	String iLabel;
	ResourceItemArray iRIA;
	unsigned char iLocal;
	unsigned long iResourceId;
	int iFormatAsHex;
	bool iContainsCompressedUnicode;
	};

// ResourceItem

class ResourceItem : public ArrayItem, public StackItem
	{
public:
	virtual ResourceItemArray * GetRIA() = 0;
	virtual void Set( const String & ValueToSet) = 0;
	virtual void Set( const StringArray & ValuesToSet) = 0;
	virtual ostream & StreamOut ( ostream & os) = 0;
	virtual void StreamOut(ResourceDataStream& aStream) const = 0;
	virtual void AddDefault() = 0;
	virtual void SetSRLink( unsigned long SRLinkToSet) = 0;	
	void Set(const String* FileNameToSet, int LineNumberToSet);
	void RegisterErrorLocation();
	int GetResourceItemType();
	String GetLabel();
protected:
	ResourceItem( const String & Label, int aResourceItemType=EResourceItem);
	ResourceItem & operator= ( const ResourceItem &);
public:
	const String& iLabel;
	const String* iFileName;
	int iLineNumber;
private:
	int iResourceItemType;
	};

// SimpleResourceItem

class SimpleResourceItem : public ResourceItem
	{
public:
	SimpleResourceItem(SimpleStructItem*);
	void Set(const String& aValueToSet);
	void Set(const StringArray& aValuesToSet);
	virtual ostream& StreamOut(ostream& os);
	friend ostream& operator<<(ostream& os,SimpleResourceItem & o);
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

// ArrayResourceItem

class ArrayResourceItem : public ResourceItem
	{
	friend ostream& operator<<(ostream& os,ArrayResourceItem& aItem);
public:
	ArrayResourceItem(ArrayStructItem *);
	~ArrayResourceItem();
	void Set(const String& aValueToSet);
	void Set(const StringArray& aValuesToSet);
	ostream & StreamOut(ostream& os);
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

// StructTypeResourceItem

class StructTypeResourceItem : public ResourceItem
	{
public:
	StructTypeResourceItem(StructTypeStructItem*);
	void Set(const String& ValueToSet);
	void Set(const StringArray& ValuesToSet);
	ostream& StreamOut(ostream& os);
	friend ostream& operator<<(ostream& os,StructTypeResourceItem& o);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long aSRLinkToSet);
	ResourceItemArray* GetRIA();
public:
	StructTypeStructItem* iStructItem;
	ResourceItemArray iResourceItems;
	};

// StructArrayResourceItem

class StructArrayResourceItem : public ResourceItem
	{
	friend ostream& operator<<(ostream& os,StructArrayResourceItem& aItem);
public:
	StructArrayResourceItem(StructArrayStructItem*);
	void Set(const String& ValueToSet);
	void Set(const StringArray& ValuesToSet);
	ResourceItemArray* GetRIA();
	ostream& StreamOut(ostream& os);
	virtual void StreamOut(ResourceDataStream& aStream) const;
	virtual void AddDefault();
	virtual void SetSRLink(unsigned long SRLinkToSet);
public:
	StructArrayStructItem* iStructItem;
	ResourceItemArrayArray iArrayOfResourceItemArrays;
	ResourceItemArray* iLastRIA;
	};

#endif
