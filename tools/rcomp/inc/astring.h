// ASTRING.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <iostream>
#include "array.h"

class RCBinaryStream;
class NumericValue;

class String : public ArrayItem
{
	friend std::ostream& operator<< ( std::ostream& os, const String & a);
	friend class NumericValue;
	friend class RCBinaryStream;
public:
	enum CharacterSet { UNKNOWN, ASCII, CP1252, CP850, ISOLatin1, ShiftJIS, Unicode, UTF8 };

	String();
	String(char* pszText);
	String(const String& SourceString);
	~String();
	void Reset();
	char& operator[] (unsigned long CharIndex) const;
	String& operator= (const String & SourceString);
	String& operator+= (const String & SourceString);
	int operator== (const String & CompareString) const; 
	int operator!= (const String & CompareString) const; 
	String operator+ (const String & SecondString) const;
	unsigned long Length() const;
	const char * GetBuffer() const;
	int IsDecNatural() const;	// Returns TRUE(1) if all decimal digits (disregards leading -).
	String& Upper();
	const unsigned char* UCRep (unsigned long aIndex) const;

	// export string to caller-supplied buffer mapping from fromchset.
	int Export( unsigned short *buffer, int& length, CharacterSet fromchset ) const;
	unsigned long ExportLength( CharacterSet tochset, CharacterSet fromchset ) const;

private:
	char* iRep;
	unsigned long iLength;
};

extern String::CharacterSet SourceCharacterSet;
extern String::CharacterSet TargetCharacterSet;

#endif // __ASTRING_H__
