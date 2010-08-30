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
* STRING.CPP
*
*/


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

String::String(const char* aText)
	{
	ArrayItem();
	iLength=strlen(aText);
	iRep=new char[iLength+1];
	assert(iRep!=NULL);
	strcpy(iRep,aText);
	}

String::String(const String& SourceString): ArrayItem(SourceString) 
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

ostream& operator<< ( ostream& os, const String & a)
	{
	return ( os << ( ( a.iLength <= 0) ? "<empty>" : a.iRep) );
	}

const char * String::GetBuffer() const
	{
	assert (iRep != NULL);
	return iRep;
	}

const char * String::GetAssertedNonEmptyBuffer() const
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

bool StringLess::operator()(const String& aLeft, const String& aRight) const
	{
	const char* bufferLeft = aLeft.GetBuffer();
	const char* bufferRight = aRight.GetBuffer();
	for (;;)
		{
		if (*bufferLeft != *bufferRight || *bufferLeft == 0)
			return *bufferLeft < *bufferRight;
		++bufferLeft;
		++bufferRight;
		}
	}

const unsigned char* String::UCRep (unsigned long aIndex) const
	{
	return (const unsigned char*)&iRep[aIndex];
	}

static UTF32 getUTF8(const unsigned char* aUtfByte, unsigned int& aIndex, unsigned int aMax)
	{
	unsigned char utfByte = *aUtfByte++;
	aIndex +=1;
	UTF32 unicodeChar = (UTF32) utfByte;

	// Slightly cavalier decoding - always write something
	// and don't consume bytes which don't fit the pattern!
	if ((utfByte & 0xe0) == 0xc0)
		{
		unicodeChar = (UTF16)((utfByte&0x1f)<<6);
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
		unicodeChar = (UTF16)((utfByte&0x0f)<<12);
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
	else if ((utfByte & 0xF8) == 0xF0)	// 4 bytes UTF-8
		{
		unicodeChar = (UTF32)((utfByte & 0x07) << 18);
		if (aIndex < aMax)
			{
			utfByte = (unsigned char)(*aUtfByte++);
			if ((utfByte&0xc0)==0x80)
				{
				unicodeChar |= (utfByte&0x3f)<<12;
				aIndex +=1;
				}
			}
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

int String::FindSubString(String aSubString, int aStart)
	{
	for(unsigned int j=aStart; j<iLength - aSubString.Length() + 1; j++)
		{
		bool match = true;
		for(unsigned int k=0; k< aSubString.Length(); k++)
			if(iRep[j+k]!=aSubString[k])
				match = false;
		if(match) return j;
		}
	return EStringNotFound;
	}


String String::ExtractSubString(const unsigned int aStart, const unsigned int aFinish)
	{
	// ensure that the passed bounds are valid 
	
	if( aStart > iLength ) 
		{
		String substr = "";
		return substr;
		}
	else if( aFinish > iLength ) {
		assert( !"This condition should never happen" );
		String substr = "";
		return substr;
		}
	else if( aStart > aFinish ) {
		assert( !"This condition should never happen" );
		String substr = "";
		return substr;
		}

	// if valid - go and copy everything
	else {
		char *char_substr = &(iRep[aStart]);
		char temp_char = iRep[aFinish + 1];
		iRep[aFinish + 1] = 0;
		String substr( char_substr );
		iRep[aFinish + 1] = temp_char;
		return substr;
		}
	}

String& String::operator+= (const char * SourceChar)
	{
	char * pOldRep = iRep;
	iLength += strlen(SourceChar);
	iRep = new char [iLength + 1];
	strcpy( iRep, pOldRep);
	strcpy( iRep + iLength-strlen(SourceChar),  SourceChar);
	delete [] pOldRep;
	return * this;
	}

int String::Atoi()
	{
	return atoi(iRep);
	}

int String::Export( UTF16 *buffer, int& length, CharacterSet fromchset ) const
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
   for(int i=0;i<length;i++)
     {
	   buffer[i] = 0;
     }

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
               buffer[outcount] = (UTF16)getUTF8(UCRep(index), index, iLength);
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
		UTF32 tu32 = getUTF8(UCRep(index), index, iLength);
		if (tu32 <= 0xFFFF)
			{
			buffer[outcount] = (UTF16)tu32;
			outcount +=1;
			}
		else
			{
			if ( tu32 > 0x10ffff )
			{
			printf("Surrogate character code must be a number in the range 0x10000 to 0x10ffff\n");
			printf("Error: rcomp.exe line %d\n", __LINE__);
			}
			
			buffer[outcount] = (UTF16)(0xD7C0 + (tu32 >> 10));			// high surrogate
			outcount++;
			if (outcount < length)
				{
				buffer[outcount] = (UTF16)(0xDC00 | (tu32 & 0x3FF));		// low surrogate
				outcount++;
				}
			else
				{
				printf("Error: rcomp.exe line %d\n", __LINE__);
				}
		}
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
			UTF32 tu32 = getUTF8(UCRep(index), index, iLength);
			outcount +=1;
			if (tu32 > 0xFFFF)
				++outcount;
			}
		break;


	default: // this should eventually become an exception

		outcount = iLength;
		break;

		} // end of switch on character set.

	return outcount;
	}



// end of ASTRING.CPP
