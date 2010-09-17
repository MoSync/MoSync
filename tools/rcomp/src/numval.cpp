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
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;
#else //!__MSVCDOTNET__
#ifndef __LINUX__
#include <strstrea.h>
#endif //!__LINUX__
#endif //__MSVCDOTNET__

#include "astring.h"
#include "numval.h"  
#include "structst.h"
#include "parser.h"
#include "rcomp.hpp"
#include "mem.h"     
#include "errorhan.h"
#include "rcbinstr.h"

#if defined(__VC32__)
#pragma warning( disable : 4702 )	// unreachable code
#endif

NumericValue::NumericValue( const String & Source, DataType NumericValueType):
	iNumericValueType( NumericValueType),
	iData( NULL),
	iULongValue( 0),
	iSignedValue( 0),
	iDoubleValue( 0.0)
	{
	AllocateSpace();
	ConvertToNumber( Source);
	}

NumericValue::NumericValue( DataType NumericValueType):
	iNumericValueType( NumericValueType),
	iData( NULL),
	iULongValue( 0),
	iSignedValue( 0),
	iDoubleValue( 0.0)
	{
	AllocateSpace();
	}

NumericValue::NumericValue( unsigned long ValueToSet, DataType NumericValueType):
	iNumericValueType( NumericValueType),
	iData( NULL),
	iULongValue( 0),
	iSignedValue( 0),
	iDoubleValue( 0.0)
	{
	AllocateSpace();
	StoreValue( ValueToSet);
	}

NumericValue::~NumericValue()
	{
	delete [] iData;
	}

NumericValue::NumericValue( const NumericValue & Source):
	iNumericValueType( Source.iNumericValueType),
	iData( NULL),
	iULongValue( 0),
	iSignedValue( 0),
	iDoubleValue( 0.0)	
	{
	AllocateSpace();
	memcpy( iData, Source.iData, iSize);
	}

void NumericValue::AllocateSpace()
	{
	switch(iNumericValueType)
		{
		case L_BYTE:
			iData = new unsigned char [1];
			iSize = 1;
			break;
		case L_WORD:
			iData = new unsigned char [2];
			iSize = 2;
			break;
		case L_LONG:
			iData = new unsigned char [4];
			iSize = 4;
			break;
		default:
			if ( iNumericValueType != L_DOUBLE)
				assert(0);	// Cannot use NumericValue for specified data type.
		}

	if ( iNumericValueType != L_DOUBLE && iData == NULL)
		{
		ErrorHandler::OutputErrorLine( "Error: Failed to allocate space for number.");
		exit(1);
		}
	}

const unsigned char * NumericValue::Data() const
	{
	return iData;
	}

unsigned long NumericValue::Size() const
	{
	return iSize;
	}

DataType NumericValue::NumericValueType() const
	{
	return iNumericValueType;
	}

void NumericValue::ConvertToNumber( const String & Source)
	{
	if ( iNumericValueType == L_DOUBLE)
		ConvertToDouble( Source);
	else
		ConvertToNatural( Source);
	}

void NumericValue::ConvertToDouble( const String & Source)
	{
	assert( iNumericValueType == L_DOUBLE);
	assert( Source.Length() > 0);
	
	double d = atof( Source.GetAssertedNonEmptyBuffer());
	if ( d == 0.0 && !( Source == "0.0" || Source == "0") )
		{ MOFF; cerr << "atof may have failed for " << Source << endl; MON;}
		
	iDoubleValue = d;
	}
 
#if defined(__VC32__)
#pragma warning( disable : 4706 ) // assignment within conditional expression
#endif

void NumericValue::ConvertToNatural( const String & Source)
	{
	unsigned long	LongValue = 0;
	
	assert( sizeof( unsigned long) >= 4);	// Assume that LongValue can hold at least 2^32 - 1.
	
	const char *	pSourceChar = Source.iRep;
	int	bLeadingHyphen = 0;
	int	bHexNumber = 0;

	if ( pSourceChar[0] == '0' && pSourceChar[1] == 'x')
		{
		bHexNumber = 1;
		pSourceChar++;
		pSourceChar++;
		}
	
	if ( pSourceChar[0] == '-')
		{
		bLeadingHyphen = 1;
		pSourceChar++;
		}

	while ( * pSourceChar != '\0')
		{
		unsigned char DigitValue;
		
		if ( bHexNumber)
			{
			assert( isxdigit( * pSourceChar) );
			if ( isdigit( * pSourceChar) )
				DigitValue = (unsigned char)(* pSourceChar - '0');
			else
				DigitValue = (unsigned char)(toupper( * pSourceChar) - 'A' + 10);
				if (LongValue >= 0x10000000)
					{
					String st("Number \"");	
					st += Source;
					st +=  "\" is too big ";
					ErrorHandler::OutputErrorLine(st);	//prevents overflow if number is bigger than 2^32 - 1.
					}
			LongValue = LongValue * 16 + DigitValue;
			}
		else
			{
			if ( ! isdigit( * pSourceChar) )
				{
				String s( "Error: Cannot convert \"");
				s += Source;
				s += "\" to a number.";
				ErrorHandler::OutputErrorLine( s);
				exit(1);
				// unreachable code
				}
			DigitValue = (unsigned char)(* pSourceChar - '0');
			if ((LongValue > 429496729) || ((LongValue == 429496729) && (DigitValue > 5)))
				{
				String st("Number \"");	
				st += Source;
				st +=  "\" is too big ";
				ErrorHandler::OutputErrorLine(st);	//prevents overflow if number is bigger than 2^32 - 1.
				}
			LongValue = LongValue * 10 + DigitValue;
			}	
		pSourceChar++;		
		assert( ( pSourceChar - Source.iRep) < 10000);	// Safety check!
		}

	int inrange=0;
	
	// Check value is within the allowed range for the type taking into account
	// a leading hyphen (minus sign) if there was one.
	switch( iNumericValueType)
		{
		case L_BYTE:							// L_BYTE is 8 bits long.
			if ( bLeadingHyphen)
				{
				if ( ! ( inrange = (LongValue <= 128) ) )			// 2 ^ 7
					ErrorHandler::OutputErrorLine( "Error: Number too low for BYTE");
				}
			else
				if ( ! ( inrange = (LongValue <= 0xFF) ) )
					ErrorHandler::OutputErrorLine( "Error: Number too big for BYTE");
			break;
		case L_WORD:							// L_WORD is 16-bits long.
			if ( bLeadingHyphen)
				{
				if ( ! ( inrange = (LongValue <= 32768) ) )		// 2^15
					ErrorHandler::OutputErrorLine( "Error: Number too low for WORD");
				}
			else
				if ( ! ( inrange = (LongValue <= 0xFFFF) ) )
					ErrorHandler::OutputErrorLine( "Error: Number too big for WORD");
			break;
		case L_LONG:							// L_LONG is 32-bits long
			if ( bLeadingHyphen)
				{
				if ( ! ( inrange = (LongValue <= 0x80000000) ) )	// 2^31
					ErrorHandler::OutputErrorLine( "Error: Number too low for LONG");
				}
			else
				if ( ! ( inrange = (LongValue <= 0xFFFFFFFF ) ) ) // This test is a bit pointless as long cannot be greater than 0xffffffff
					ErrorHandler::OutputErrorLine( "Error: Number too big for LONG");
			break;
		default:
			assert(0);	// Other data types cannot be converted to natural numbers.
		}

	if(!inrange)
		exit(1);
	
	StoreValue( LongValue);

	// If there was a hyphen then make the stored number negative (using two's complement).
	if ( bLeadingHyphen)
		{
		LongValue = (LongValue ^ 0xFFFFFFFFu)+1;

		// Output file will be treated as little-endian.
		switch ( iNumericValueType)
			{
			case L_LONG:
				iData[3] = (unsigned char)((LongValue & 0xFF000000) >> 24);
				iData[2] = (unsigned char)((LongValue & 0xFF0000) >> 16);
			case L_WORD:
				iData[1] = (unsigned char)((LongValue & 0xFF00) >> 8);
			case L_BYTE:
				iData[0] = (unsigned char)(LongValue & 0xFF);
			}
		}
	}

#if defined(__VC32__)
#pragma warning( default : 4706 ) // assignment within conditional expression
#endif

void NumericValue::StoreValue( unsigned long LongValue)
	{		
	iULongValue = LongValue;
	
	if ( LongValue <= 0x80000000)
		iSignedValue = (unsigned long) LongValue;

	int inrange = 1;
		
	switch( iNumericValueType)
		{
		case L_BYTE:
			inrange = ( LongValue <= 0xFF);
			break;
		case L_WORD:
			inrange = ( LongValue <= 0xFFFF);
			break;
		case L_LONG:
			inrange = ( LongValue <= 0xFFFFFFFF);
		}

	if ( ! inrange)	
		{
		ErrorHandler::OutputErrorLine( "Error: Numeric value out of range for specified type");
		exit(1);
		}

	// Output file will be treated as little-endian.
	switch ( iNumericValueType)
		{
		case L_LONG:
			iData[3] = (unsigned char)((LongValue & 0xFF000000) >> 24);
			iData[2] = (unsigned char)((LongValue & 0xFF0000) >> 16);
		case L_WORD:
			iData[1] = (unsigned char)((LongValue & 0xFF00) >> 8);
		case L_BYTE:
			iData[0] = (unsigned char)(LongValue & 0xFF);
		}
	}

template<bool> class __CompileTimeAssert {public: __CompileTimeAssert(...) {}};
template<> class __CompileTimeAssert<false> {};
struct COMPILE_TIME_ERROR {};
#define COMPILE_TIME_ASSERT(aCondition) { __CompileTimeAssert<(aCondition)> __temp = __CompileTimeAssert<(aCondition)>(COMPILE_TIME_ERROR()); }

RCBinaryStream & operator<< ( RCBinaryStream & os, NumericValue o)
	{
	switch( o.iNumericValueType)
		{
		case L_BYTE:
			os.Write( o.iData, 1);
			break;
		case L_WORD:
			os.Write( o.iData, 2);
			break;
		case L_LONG:
			os.Write( o.iData, 4);
			break;
		case L_DOUBLE:
			COMPILE_TIME_ASSERT(sizeof(double) == 8);
			os.Write(reinterpret_cast<unsigned char*>(&o.iDoubleValue), 8);
			break;
		default:
			assert(0);
		}
	
	return os;
	}

void NumericValue::StreamOut(ResourceDataStream& aStream) const
	{
	switch (iNumericValueType)
		{
		case L_BYTE:
			aStream.StreamIn(iData, 1);
			break;
		case L_WORD:
			aStream.StreamIn(iData, 2);
			break;
		case L_LONG:
			aStream.StreamIn(iData, 4);
			break;
		case L_DOUBLE:
			COMPILE_TIME_ASSERT(sizeof(double) == 8);
			aStream.StreamIn(reinterpret_cast<const unsigned char*>(&iDoubleValue), 8);
			break;
		default:
			assert(0);
		}
	}

NumericValue & NumericValue::operator= ( unsigned long ValueToSet)
	{
	StoreValue( ValueToSet);
	
	return * this;
	}

unsigned long NumericValue::GetULong() const
	{
	return iULongValue;
	}

long NumericValue::GetLong() const
	{
	assert( iULongValue <= 2147483647);	// Check that we are not holding a number that is really positive only.
	return iSignedValue;
	}

bool NumericValue::CheckSigned(unsigned long aValue, DataType aNumericValueType)
	{
	switch (aNumericValueType)
		{
		case L_BYTE:
			if (aValue > 0x7f) return false;
			break;
		case L_WORD:
			if (aValue > 0x7fff) return false;
			break;
		case L_LONG:
			if (aValue > 0x7fffffff) return false;
			break;
		default:
			assert(0);
		}
	return true;
	}

String NumericValue::ltoa( long Source)
	{
	char	v[10];	// long can have no more than 10 digits in this implementation.
	char *	pv = v;
	long	x;
	
	if ( Source < 0)
		x = - Source;
	else
		x = Source;

	if ( x == 0)
		* pv++ = '0';
	else
		{
		while( x > 0)
			{
			assert( pv <= (v+9) );

			* pv = char(x%10 + '0');
			pv++;
			x /= 10;
			}
		}

	// Now reverse digits so they are in the correct order. Put in terminating null and hyphen
	// if necessary.
	
	char	r[12];
	char *	pr = r;

	if ( Source < 0)
		{
		r[0] = '-';
		pr++;
		}

	while( pv != v)
		{
		assert( pr < (r+11) );
		* pr++ = * --pv;
		}
		
	* pr = '\0';
	
	return r;
	}
