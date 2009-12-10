// NUMVAL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __NUMVAL_H__
#define __NUMVAL_H__

#include "datatype.h"

class String;
class ResourceDataStream;

class NumericValue
{
	friend class String;
	friend RCBinaryStream& operator<<(RCBinaryStream& os,NumericValue aValue);
public:
	NumericValue(const String & Source, DataType NumericValueType);
	NumericValue(DataType NumericValueType);
	NumericValue(unsigned long ValueToSet, DataType NumericValueType);
	~NumericValue();
	NumericValue(const NumericValue & Source);
	const unsigned char* Data() const;
	unsigned long Size() const;
	DataType NumericValueType() const;
	unsigned long GetULong() const;
	long GetLong() const;
	NumericValue& operator=(unsigned long aValueToSet);
	static String ltoa(long aValue);
	void StreamOut(ResourceDataStream& aStream) const;
private:
	void AllocateSpace();
	void ConvertToNumber(const String& aSource);
	void ConvertToDouble(const String& aSource);
	void ConvertToNatural(const String& aSource);
	void StoreValue(unsigned long aLongValue);
private:	
	unsigned long iSize;
	DataType iNumericValueType;
	unsigned char* iData;
	unsigned long iULongValue;
	long iSignedValue;
	double iDoubleValue;
};

#endif
