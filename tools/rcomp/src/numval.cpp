// NUMVAL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <string.h>
#include <ctype.h>
#if __GNUC__ >= 3 || defined(_MSC_VER)
#include <sstream>
#else
#include <strstream.h>
#endif
#include "astring.h"
#include "numval.h"
#include "structst.h"
#include "rcompl.hpp"
#include "rcomp.hpp"
#include "mem.h"
#include "errorhan.h"
#include "rcbinstr.h"

using namespace std;

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
		ErrorHandler::OutputErrorLine( "Failed to allocate space for number.");
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

	double d = atof( Source.GetBuffer());
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
			LongValue = LongValue * 16 + DigitValue;
		}
		else
		{
			if ( ! isdigit( * pSourceChar) )
			{
				String s( "Cannot convert \"");
				s += Source;
				s += "\" to a number.";
				ErrorHandler::OutputErrorLine( s);
				exit(1);
				// unreachable code
			}
			DigitValue = (unsigned char)(* pSourceChar - '0');
			LongValue = LongValue * 10 + DigitValue;
		}	
		pSourceChar++;		
		assert( ( pSourceChar - Source.iRep) < 10000);	// Safety check!
	}

	int inrange=0;

	// Check value is within the allowed range for the type taking into account
	// a leading hyphen (minus sign) if there was one.
	/*
	switch( iNumericValueType)
	{
	case L_BYTE:							// L_BYTE is 8 bits long.
	if ( bLeadingHyphen)
	{
	if ( ! ( inrange = (LongValue <= 128) ) )			// 2 ^ 7
	ErrorHandler::OutputErrorLine( "Number too low for BYTE");
	}
	else
	if ( ! ( inrange = (LongValue <= 0xFF) ) )
	ErrorHandler::OutputErrorLine( "Number too big for BYTE");
	break;
	case L_WORD:							// L_WORD is 16-bits long.
	if ( bLeadingHyphen)
	{
	if ( ! ( inrange = (LongValue <= 32768) ) )		// 2^15
	ErrorHandler::OutputErrorLine( "Number too low for WORD");
	}
	else
	if ( ! ( inrange = (LongValue <= 0xFFFF) ) )
	ErrorHandler::OutputErrorLine( "Number too big for WORD");
	break;
	case L_LONG:							// L_LONG is 32-bits long
	if ( bLeadingHyphen)
	{
	if ( ! ( inrange = (LongValue <= 2147483648UL) ) )	// 2^31
	ErrorHandler::OutputErrorLine( "Number too low for LONG");
	}
	else
	inrange = 1;
	//				if ( ! ( inrange = (LongValue <= 2147483647) ) )	// Should be 0xFFFFFFFF but GetLong is called in grammar.
	//					ErrorHandler::OutputErrorLine( "Number too big for LONG");
	break;
	default:
	assert(0);	// Other data types cannot be converted to natural numbers.
	}
	*/
	inrange = 1;

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

	//	if ( LongValue <= 2147483648UL)
	iSignedValue = (unsigned long) LongValue;

	int inrange = 1;
	/*
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
	*/
	if ( ! inrange)	
	{
		ErrorHandler::OutputErrorLine( "Numeric value out of range for specified type");
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

/*
alfredh: does not work with gcc 2.95:

numval.cpp:301: warning: all member functions in class `__CompileTimeAssert<false>' are private
numval.cpp: In function `class RCBinaryStream & operator <<(RCBinaryStream &, NumericValue)':
numval.cpp:319: implicit declaration of function `int Dummy(...)'
numval.cpp:319: parse error before `;'

*/
template<bool> class __CompileTimeAssert {public: inline static void Dummy() {}};
template<> class __CompileTimeAssert<false> {private: inline static void Dummy() {}};
#define COMPILE_TIME_ASSERT(aCondition) {__CompileTimeAssert<aCondition>::Dummy();}

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
		//			COMPILE_TIME_ASSERT(sizeof(double) == 8); TODO
		if(8 != sizeof(double)) {
			printf("sizeof(double) is %u and not 8\n", sizeof(double));
			assert(0);
		}
		os.Write( (unsigned char *) & o.iDoubleValue, 8);
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
		//			COMPILE_TIME_ASSERT(sizeof(double) == 8);
		if(8 != sizeof(double)) {
			printf("sizeof(double) is %u and not 8\n", sizeof(double));
			assert(0);
		}
		aStream.StreamIn((const unsigned char *)&iDoubleValue, 8);
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
	//	assert( iULongValue <= 2147483647);	// Check that we are not holding a number that is really positive only.
	return iSignedValue;
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
