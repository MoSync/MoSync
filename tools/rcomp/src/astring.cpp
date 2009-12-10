// STRING.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "ctable.h"     // character code mapping classes
#include "astring.h"

extern Mapping_range CP1252_exceptions;
// table of exceptions from CP1252 1:1 mapping with Unicode.


#undef STRING_DEBUG

String::String()
{
	iLength=0;
	iRep=NULL;
}

String::String(char* aText)
{
	iLength=(int)strlen(aText);
	iRep=new char[iLength+1];
	assert(iRep!=NULL);
	strcpy(iRep,aText);
}

String::String(const String& SourceString)
{
	iLength=SourceString.iLength;
	iRep=new char[ iLength + 1];
	assert( iRep != 0);
	if(iLength==0) *iRep='\0';
	else strcpy( iRep, SourceString.iRep);
}

String::~String()
{
	delete [] iRep;
}

void String::Reset()
{
	iLength=0;
	delete [] iRep;
	iRep=NULL;
}

char& String::operator[] (unsigned long CharIndex) const
{
	if ( CharIndex > iLength)
	{
		assert( 0);
		return iRep[ iLength];	// i.e. \0
	}
	return iRep[ CharIndex];
}

String& String::operator=(const String& SourceString)
{
	if(&SourceString==this)
		return *this;
	delete [] iRep;
	iLength=SourceString.iLength;
	if ( iLength == 0)
	{
		iRep=NULL;
		return * this;
	}
	iRep=new char [ iLength + 1];
	assert( iRep != NULL);
	strcpy( iRep, SourceString.iRep);
	return *this;
}

String& String::operator+= (const String & SourceString)
{
	char * pOldRep=iRep;
	iLength += SourceString.iLength;
	if ( iLength == 0)
		iRep=NULL;
	else
	{
		iRep=new char [ iLength + 1];
		assert( iRep != NULL);
		strcpy( iRep, pOldRep);
		strcpy( iRep + strlen( pOldRep), SourceString.iRep);
	}
	delete [] pOldRep;
	return *this;
}

int String::operator== (const String & CompareString) const
{
	return(!strcmp( iRep, CompareString.iRep));
}

int String::operator!= (const String & CompareString) const
{
	return(strcmp( iRep, CompareString.iRep));
}

unsigned long String::Length() const
{
	return iLength;
}

std::ostream& operator<< ( std::ostream& os, const String & a)
{
	return ( os << ( ( a.iLength <= 0) ? "<empty>" : a.iRep) );
}

const char * String::GetBuffer() const
{
	assert( iRep != NULL);
	assert( iLength > 0);
	return iRep;
}

int String::IsDecNatural() const
{
	assert( iLength > 0);
	unsigned long i=0;	
	if ( iRep[0] == '-')
		i++;
	for( ; i < iLength; i++)
	{
		if (!isdigit( iRep[i]) )
			return 0;	// Non-digit found.
	}
	return 1; // Successful - all numeric.
}

String & String::Upper()
{
	for(unsigned long i=0;i<iLength;i++)
		iRep[i]=char(toupper(iRep[i]));
	return *this;
}

String String::operator+ (const String & SecondString) const
{
	String	s;
	s.iLength=iLength + SecondString.iLength;
	s.iRep=new char[ s.iLength + 1];
	strcpy( s.iRep, iRep);
	strcpy( s.iRep + iLength, SecondString.iRep);
	return s;	
}

const unsigned char* String::UCRep (unsigned long aIndex) const
{
	return (const unsigned char*)&iRep[aIndex];
}

static unsigned short getUTF8(const unsigned char* aUtfByte, unsigned int& aIndex, unsigned int aMax)
{
	unsigned char utfByte = *aUtfByte++;
	aIndex +=1;
	unsigned short unicodeChar = (unsigned short) utfByte;

	// Slightly cavalier decoding - always write something
	// and don't consume bytes which don't fit the pattern!
	if ((utfByte & 0xe0) == 0xc0)
	{
		unicodeChar = (unsigned short)((utfByte&0x1f)<<6);
		if (aIndex < aMax)
		{
			utfByte = (unsigned char)(*aUtfByte++);
			if ((utfByte&0xc0)==0x80)
			{
				unicodeChar |= (utfByte&0x3f);
				aIndex +=1;
			}
		}
	}
	else
		if ((utfByte & 0xf0) == 0xe0)
		{
			unicodeChar = (unsigned short)((utfByte&0x0f)<<12);
			if (aIndex < aMax)
			{
				utfByte = (unsigned char)(*aUtfByte++);
				if ((utfByte&0xc0)==0x80)
				{
					unicodeChar |= (utfByte&0x3f)<<6;
					aIndex +=1;
				}
			}
			if (aIndex < aMax)
			{
				utfByte = (unsigned char)(*aUtfByte++);
				if ((utfByte&0xc0)==0x80)
				{
					unicodeChar |= (utfByte&0x3f);
					aIndex +=1;
				}
			}
		}
		return unicodeChar;
}

int String::Export(unsigned short *buffer, int& length, CharacterSet fromchset ) const
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Export the string from its internal form to the caller supplied buffer
// (which in this case is meant to be Unicode).  On entry, length indicates
// the number of characters in the buffer.  On exit, this is set to the number
// of characters actually written. The export involves mapping from the
// specified character set to Unicode.
//
// The return value is normally TRUE.  If not, truncation has occurred.
//
//
// NB, this function is currently under development and character set
// mapping is not yet properly implemented.
// NB2. it's also largely duplicated in String::ExportLength, which should
// be updated to match...
// ---------------------------------------------------------------------------
{
	unsigned int index = 0;           // index into internal buffer
	int outcount = 0;        // count of characters written to export buffer


	// Because of multibyte character sets, the general code pattern for
	// copying the characters has to work left to right to allow for
	// byte sequence interpretation.  The raw byte count of such a string
	// can be greater than the number of characters it represents.
	switch ( fromchset )
	{
	case CP1252:
		// In this case, we know that there is only a narrow range
		// of characters that aren't a direct mapping.

		while ( (index < iLength) && ( outcount < length ) )
		{

			// To allow for direct Unicode characters in CP1252 strings, we
			// insert a special character followed by the UTF8 sequence

			if (*UCRep(index) == UnicodeEscape)
			{
				index +=1;
				if (index < iLength)
				{
					buffer[outcount] = getUTF8(UCRep(index), index, iLength);
				}
			}
			else
			{
				buffer[outcount] = *UCRep(index);
				index +=1;
			}

			// Now, see if the character ended up in the forbidden range.  If so, map
			// it to the correct Unicode character.

			if ( buffer[outcount] < 255 )
			{
				unsigned char temp;
				temp = (unsigned char)buffer[outcount];
				CP1252_exceptions.map(temp, buffer[outcount]);
			}

			outcount += 1;


		} // end of loop to export characters
		break;



	case UTF8: 

		while ( (index < iLength) && ( outcount < length ) )
		{
			buffer[outcount] = getUTF8(UCRep(index), index, iLength);
			outcount +=1;
		} // end of loop to export characters
		break;


	default: // this should eventually become an exception

		while ( (index < iLength) && ( outcount < length ) )
		{
			buffer[outcount] = *UCRep(index);
			outcount +=1;
			index += 1;
		} // end of loop to export characters
		break;


	} // end of switch on character set.

	length = outcount;

	// If the index is not now equal to the internal length then
	// the string was truncated on export.

	if ( index != iLength ) return 0; else return 1;



} // end of Export to Unicode function.


// What length of exported text does this String represent?

unsigned long String::ExportLength (CharacterSet tochset, CharacterSet fromchset) const
{
	if (tochset != Unicode)
		return iLength;

	unsigned int index = 0;           // index into internal buffer
	unsigned long outcount = 0;       // count of output characters

	switch ( fromchset )
	{
	case CP1252:
		// In this case, we know that there is only a narrow range
		// of characters that aren't a direct mapping.

		while ( (index < iLength) )
		{

			// To allow for direct Unicode characters in CP1252 strings, we
			// insert a special character followed by the UTF8 sequence

			if (*UCRep(index) == UnicodeEscape)
			{
				index +=1;
				if (index < iLength)
				{
					(void) getUTF8(UCRep(index), index, iLength);
				}
			}
			else
			{
				index +=1;
			}
			outcount += 1;
		}
		break;

	case UTF8: 

		while ( (index < iLength) )
		{
			(void) getUTF8(UCRep(index), index, iLength);
			outcount +=1;
		}
		break;


	default: // this should eventually become an exception

		outcount = iLength;
		break;

	} // end of switch on character set.

	return outcount;
}



// end of ASTRING.CPP
