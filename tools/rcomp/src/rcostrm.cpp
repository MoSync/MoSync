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
#include <stdlib.h>
#include <memory.h>

#include "resource.h"
#include "datatype.h"
#include "parser.h"
#include "rcomp.hpp"
#include "numval.h"
#include "mem.h"
#include "errorhan.h"
#include "unicode_compressor.h"
#include "main.h"

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
using std::endl;
using std::cout;
#endif //__MSVCDOTNET__

extern int verbose;
extern String::CharacterSet SourceCharacterSet;

void Panic(int aCode) // used by UNICODE_COMPRESSOR.CPP
	{
	exit(aCode);
	}

RCTypeArray gTypes;

// StructItem based ostream functions

ostream& SimpleStructItem::StreamOut ( ostream & os)
	{
	return operator<< ( os, * this);
	}

ostream& ArrayStructItem::StreamOut ( ostream & os)
	{
	return operator<< ( os, * this);
	}

ostream& StructTypeStructItem::StreamOut ( ostream & os)
	{
	return operator<< ( os, * this);
	}

ostream& StructArrayStructItem::StreamOut ( ostream & os)
	{
	return operator<< ( os, * this);
	}

ostream& operator<< ( ostream & os, SimpleStructItem & o)
	{
	os << "SimpleStructItem        ";
	os << o.iLabel;
	os << "\t" << gTypes.GetName( o.iItemType);
	os << "\tDefault: " << o.iDefault;
	os << "\tLength limit: " << o.iLengthLimit;
	os << endl;
	return os ;
	}

ostream& operator<< ( ostream & os, ArrayStructItem & o)
	{
	os << "ArrayStructItem         ";
	os << o.iLabel;
	os << "\t" << gTypes.GetName( o.iItemType);
	os << "\tDefaults: " << o.iDefaults;
	os << "\tLen Type: " << ((o.iLenType == 0) ? "<none>" : gTypes.GetName(o.iLenType));
	os << "\tSize: " << o.iSize;
	os << endl;
	return os ;
	}

ostream& operator<< ( ostream & os, StructTypeStructItem & o)
	{
	os << "StructTypeStructItem    ";
	os << o.iLabel;
	os << endl;
	return os;
	}

ostream& operator<< ( ostream & os, StructArrayStructItem & o)
	{
	os << "StructArrayStructItem   ";
	os << o.iLabel;
	os << "\tLen Type: " << ( ( o.iLenType == 0) ? "<none>" : gTypes.GetName( o.iLenType) );
	os << "\tSize: " << o.iSize;
	os << endl;
	return os;
	}

ostream& operator<< ( ostream & os, StructItemArray & o)
	{
	if (o.Size() == 0)
		return ( os << "<none>");
	StructItemArrayIterator next(o);
	StructItem* p;
	while ((p = next() ) != NULL)
		p->StreamOut(os);
	return os;
	}

ostream& operator<< ( ostream & os, StructHeader & o)
	{
	os << "StructHeader            ";
	os << o.iLabel << endl;
	os << "\tLen Type: " << ( ( o.iLenType == 0) ? "<none>" : gTypes.GetName( o.iLenType) ) << endl;
	os << o.iSIA;
	return os;
	}

ostream& operator<< ( ostream & os, StructHeaderArray & o)
	{
	if (o.Size() == 0)
		return ( os << "<none>");
	StructHeaderArrayIterator next( o);
	StructHeader* p;
	while ( ( p = next() ) != NULL)
		os << *p;
	return os;
	}

// ResourceItem based ostream functions

ostream& SimpleResourceItem::StreamOut(ostream& os)
	{
	return operator<<(os,*this);
	}

ostream& ArrayResourceItem::StreamOut(ostream& os)
	{
	return operator<<(os,*this);
	}

ostream& StructTypeResourceItem::StreamOut(ostream& os)
	{
	return operator<<(os,*this);
	}

ostream& StructArrayResourceItem::StreamOut(ostream& os)
	{
	return operator<<(os,*this);
	}

ostream & operator<<(ostream& os,SimpleResourceItem& o)
	{
	os << "SimpleResourceItem [" << gTypes.GetName(o.iStructItem->iItemType) << " " << o.iStructItem->iLabel << "]  ";
	switch( o.iStructItem->iItemType)
		{
		case L_TEXT:
		case L_LTEXT:
		case L_BUF:
		case L_BYTE:
		case L_WORD:
		case L_LONG:
			os << o.iValue;
			break;
		case L_SRLINK:
			break;	// Don't know value yet.
		}
	os << endl;
	return os;
	}

ostream & operator<< ( ostream & os, ArrayResourceItem & o)
	{
	os << "ArrayResourceItem [" << o.iStructItem->iLabel << "]  ";
	os << o.iValues;
	os << endl;
	return os;
	}

ostream & operator<< ( ostream & os, StructTypeResourceItem & o)
	{
	static unsigned long level = 0;	// Recursion level.
	os << "StructTypeResourceItem (Level " << level++ << ") ["  << o.iStructItem->iLabel << "]  " << endl;
	os << "----------------------" << endl;
	os << o.iResourceItems;
	os << "----------------------" << endl;
	level--;
	return os;
	}

ostream & operator<< ( ostream & os, StructArrayResourceItem & o)
	{
	static unsigned long level = 0;	// Recursion level.
	os << "StructArrayResourceItem (Level " << level++ << ") ["  << o.iStructItem->iLabel << "]  " << endl;
	DataType counttype = o.iStructItem->iLenType;
	if (counttype==0)
		counttype = L_WORD;
	unsigned long nitems = o.iArrayOfResourceItemArrays.Size();
	os << "ArrayLength [" << gTypes.GetName(counttype) << "] " << nitems << endl;
	if (nitems > 0)
		{
		os << "----------------------" << endl;
		os << o.iArrayOfResourceItemArrays;
		os << "----------------------" << endl;
		}
	level--;
	return os;
	}

ostream & operator<< ( ostream & os, ResourceHeader & o)
	{
	os << "ResourceHeader          ";
	os << o.iLabel << endl;
	os << o.iRIA;
	return os;
	}

ostream & operator<< ( ostream & os, ResourceItemArray & o)
	{
	os << "++ResourceItemArray" << endl;
	if ( o.iLenType != 0)
		{
		os << "LenType: " << gTypes.GetName( o.iLenType) << "\t";
		}
	if ( o.Size() == 0)
		return ( os << "<none>");
	ResourceItemArrayIterator	next( o);
	ResourceItem * p;
	while ( ( p = next() ) != NULL)
		p->StreamOut( os);
	return os;
	}

ostream & operator<< ( ostream & os, ResourceItemArrayArray & o)
	{
	os << "--ResourceItemArrayArray" << endl;
	if ( o.Size() == 0)
		return ( os << "<none>");
	ResourceItemArrayArrayIterator	next( o);
	ResourceItemArray * p;
	while ( ( p = next() ) != NULL)
		os << * p;
	return os;
	}

static void StreamOutCompressingIfReducesSize(ResourceDataStream& aStream, const UTF16* aUncompressedUnicodeBuffer, int aUncompressedUnicodeLength)
	{
	const int uncompressedUnicodeSizeInBytes = aUncompressedUnicodeLength*2;

	//
	// Output buffer allocated to be one byte bigger than uncompressed input buffer.
	// This is to allow for what appears to be the worst case where the string cannot be
	// compressed at all.  In this case enough memory has to be allocated to contain
	// the uncompressable string, plus the one-byte compression mode tag.
	//
	const int maximumOutputLengthInBytes = uncompressedUnicodeSizeInBytes;
	unsigned char* const compressedUnicodeBuffer = new unsigned char[maximumOutputLengthInBytes];
	assert(compressedUnicodeBuffer != NULL);

	int compressedUnicodeSizeInBytes;
	CompressUnicode(compressedUnicodeBuffer, compressedUnicodeSizeInBytes, maximumOutputLengthInBytes, aUncompressedUnicodeBuffer, aUncompressedUnicodeLength);

	if (compressedUnicodeSizeInBytes < uncompressedUnicodeSizeInBytes)
		{
		aStream.StartOfCompressedUnicodeRun(uncompressedUnicodeSizeInBytes, (const unsigned char*)aUncompressedUnicodeBuffer);
		aStream.StreamIn(compressedUnicodeBuffer, compressedUnicodeSizeInBytes);
		aStream.EndOfCompressedUnicodeRun();
		}
	else if (uncompressedUnicodeSizeInBytes>0)
		{
		aStream.TwoByteAlignmentPoint();
		aStream.StreamIn((const unsigned char*)aUncompressedUnicodeBuffer, uncompressedUnicodeSizeInBytes);
		}

	delete [] compressedUnicodeBuffer;
	}

// ResourceItem based RCBinaryStream functions
// These are used to write the raw data as required in the final output file

static void StreamOutSingleValue(
	ResourceDataStream&	aStream,
	const String &		aValue,
	unsigned long		aLinkValue,
	DataType 			aItemType,
	const String *		aFileName,
	int					aLineNumber
	)
	{
	ErrorHandler::Register( aFileName, aLineNumber);	// Register in case conversion to number fails.



	switch ( aItemType)
		{

        // 8-bit text

		case L_TEXT8:
            {	// Null terminator, no leading byte count.
           		unsigned char * p = new unsigned char[ aValue.Length() + 1];
				if ( aValue.Length() > 0)
           			memcpy( p, aValue.GetAssertedNonEmptyBuffer(), aValue.Length() );
           		* ( p + aValue.Length() ) = '\0';
           		aStream.StreamIn(p, aValue.Length() + 1);
           		delete [] p;
            	break;
            }
		case L_LTEXT8:
            {	// Leading byte count, no null terminator. (For zero length string a byte containing
            	// zero is written out.
				assert ( aValue.Length() <= 255 );
           		unsigned char * p = new unsigned char[ aValue.Length() + 1];
				if ( aValue.Length() > 0)
           			memcpy( p + 1, aValue.GetAssertedNonEmptyBuffer(), aValue.Length() );
           		*p = (unsigned char)(aValue.Length());
           		aStream.StreamIn(p, aValue.Length() + 1);
           		delete [] p;
            	break;
            }
		case L_BUF8:	// Write just the characters from the string.
			aStream.StreamIn((const unsigned char*)aValue.GetAssertedNonEmptyBuffer(), aValue.Length());
			break;


        // 16-bit text



        case L_BUF16:   // write out unadorned 16-bit characters
            {
            int output_count;   // used as character count for binary output.

            // reserve enough for worst case
            output_count = aValue.Length();
            UTF16 *output_buffer = new UTF16[output_count];

            aValue.Export(output_buffer, output_count, SourceCharacterSet);

			// write out the buffer as a stream of octets.
			#if defined(__TOOLS2_WINDOWS__)
			StreamOutCompressingIfReducesSize(aStream, (const unsigned short*)output_buffer, output_count);
			#else
			StreamOutCompressingIfReducesSize(aStream, output_buffer, output_count);
			#endif

            delete [] output_buffer;

            break;
            }



        case L_TEXT16:  // write out a null-terminated string
			// this has no support in BAFL
            {
            int output_count;   // used as character count for binary output.

            // reserve enough for worst case + null terminator.
            output_count = aValue.Length();
            UTF16 *output_buffer = new UTF16[output_count +1];

            aValue.Export(output_buffer, output_count, SourceCharacterSet);
            output_buffer[output_count] = 0;
            output_count +=1;

            // write out the buffer as a stream of octets.
			#if defined(__TOOLS2_WINDOWS__)
			StreamOutCompressingIfReducesSize(aStream, (const unsigned short*)output_buffer, output_count);
			#else
			StreamOutCompressingIfReducesSize(aStream, output_buffer, output_count);
			#endif			

            delete [] output_buffer;

            break;
            }            


        case L_LTEXT16:  // write out counted string.  The count is in
                         // an octet, not a word.

            {
            int output_count;      // used as character count for binary output.
            unsigned char lbcount; // To hold the leading byte count.

            // reserve enough for worst case
            output_count = aValue.Length();
	// XXX I have commented the assert statement below out, because an LTEXT can't have length
	// XXX bigger than 255, but sometimes this happens. A solution would be to throw an error
	// XXX in structst.cpp, in the SimpleStructItem::SimpleStructItem() constructor.
            UTF16 *output_buffer = new UTF16[output_count];

            aValue.Export(output_buffer, output_count, SourceCharacterSet);

            // write out the count.

            lbcount = (unsigned char) output_count;
            aStream.StreamIn(&lbcount,1);

            // write out the buffer as a stream of octets.
	    StreamOutCompressingIfReducesSize(aStream, output_buffer, output_count);	

            delete [] output_buffer;

            break;
            }            




		case L_BYTE:
		case L_WORD:
		case L_LONG:
		case L_DOUBLE:
			assert ( aValue.Length() > 0);
			if(verbose) { MOFF; cout << "Converting " << aValue << " to number." << endl; MON;}
			NumericValue( aValue, aItemType).StreamOut(aStream);
			break;
		case L_SRLINK:
			NumericValue( aLinkValue, L_LONG).StreamOut(aStream);
			break;
		case L_LINK:
		case L_LLINK:
			{
				if ( aValue.Length() > 0)
				{
					DataType dtype = ( aItemType == L_LINK) ? L_WORD : L_LONG;
					if ( aValue.IsDecNatural() )	// If a resource id was specified explicitly e.g. -75 then output the value.
						NumericValue(aValue, dtype).StreamOut(aStream);
					else
					{
						// If FindId fails to find the link it will output an
						// appropriate error and kill the program.
						ErrorHandler::Register(aFileName, aLineNumber);
						unsigned long id = pG->ResourceNameIds.FindId(aValue);
						NumericValue(id, dtype).StreamOut(aStream);
					}
				}
				break;
			}
		default:
			{ 	MOFF; cout << "RCBinaryStream:Unknown type " << gTypes.GetName( aItemType) << endl; MON;}
		}
	}

void SimpleResourceItem::StreamOut(ResourceDataStream& aStream) const
	{
	StreamOutSingleValue(aStream, iValue, iLinkValue, iStructItem->iItemType, iFileName, iLineNumber);
	}

void ArrayResourceItem::StreamOut(ResourceDataStream& aStream) const
	{
	NumericValue * itemcount = NULL;
	if(iStructItem->iSize.Length() == 0)	// STRUCT item says [] i.e. no size specified.
		{
		if (iStructItem->iLenType == 0)	// No LEN declaration so default to WORD.
			itemcount = new NumericValue( L_WORD);
		else
			itemcount = new NumericValue( iStructItem->iLenType); // Use specified type.

		* itemcount = iValues.Size();
		itemcount->StreamOut(aStream);
		
		delete itemcount;
		}
	if ( iValues.Size() == 0)
		return;
	StringArrayIterator next( iValues);
	String * p;
	while ( ( p = next() ) != NULL)
		{
		assert( iStructItem->iItemType != L_SRLINK);	// Can't handle arrays of SRlinks.
		StreamOutSingleValue(aStream, *p, 0, iStructItem->iItemType, iFileName, iLineNumber);
		}
	}

void StructTypeResourceItem::StreamOut(ResourceDataStream& aStream) const
	{
	iResourceItems.StreamOut(aStream);
	}

void StructArrayResourceItem::StreamOut(ResourceDataStream& aStream) const
	{
	NumericValue * itemcount = NULL;
	if(iStructItem->iLenType == 0)	// No LEN declaration so default to WORD.
		itemcount = new NumericValue( L_WORD);
	else
		itemcount = new NumericValue( iStructItem->iLenType); // Use specified type.
	* itemcount = iArrayOfResourceItemArrays.Size();
	itemcount->StreamOut(aStream);
	delete itemcount;
	if(iArrayOfResourceItemArrays.Size() == 0)
		return;
	iArrayOfResourceItemArrays.StreamOut(aStream);
	}

void ResourceItemArray::StreamOut(ResourceDataStream& aStream) const
	{
	if (Size() == 0)
		return;
	static unsigned int RecursionLevel = 0; // Top level should not have resource size output.
	const bool outputSize=((iLenType != 0) && (RecursionLevel > 0));
	if (outputSize)
		{
		aStream.StartOfBlockWithSizePrefix(iLenType);
		}
	RecursionLevel++;
	ResourceItemArrayIterator next(*this);
	ResourceItem * p;
	while ( ( p = next() ) != NULL)
		{
		p->StreamOut(aStream);
		}
	RecursionLevel--;
	if (outputSize)
		{
		aStream.EndOfBlockWithSizePrefix();
		}
	}

void ResourceItemArrayArray::StreamOut(ResourceDataStream& aStream) const
	{
	if (Size() == 0)
		return;
	ResourceItemArrayArrayIterator next(*this);
	ResourceItemArray * p;
	while ( ( p = next() ) != NULL)
		p->StreamOut(aStream);
	}
