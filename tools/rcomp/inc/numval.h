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
	static bool CheckSigned(unsigned long aValue,DataType NumericValueType);
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
