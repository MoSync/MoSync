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


#ifndef __ASTRING_H__
#define __ASTRING_H__

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
using std::ostream;
#else //!__MSVCDOTNET__
#include <iostream.h>
#endif //__MSVCDOTNET__

#include "array.h"

#include "wide.h"

enum ValueIfSubStringNotFound { EStringNotFound = -1 } ;

class RCBinaryStream;
class NumericValue;

class String : public ArrayItem
	{
	friend ostream& operator<< ( ostream& os, const String & a);
	friend class NumericValue;
	friend class RCBinaryStream;
public:
    enum CharacterSet { UNKNOWN, ASCII, CP1252, CP850, ISOLatin1, ShiftJIS, Unicode, UTF8 };

	String();
	String(const char* pszText);
	String(const String& SourceString);
	~String();
	void Reset();
	char& operator[] (unsigned long CharIndex) const;
	String& operator= (const String & SourceString);
	String& operator+= (const String & SourceString);
	String& operator+= (const char * SourceChar);
	int operator== (const String & CompareString) const; 
	int operator!= (const String & CompareString) const; 
	String operator+ (const String & SecondString) const;
	unsigned long Length() const;
	const char * GetBuffer() const;
	const char * GetAssertedNonEmptyBuffer() const;
	int IsDecNatural() const;	// Returns TRUE(1) if all decimal digits (disregards leading -).
	String& Upper();
    const unsigned char* UCRep (unsigned long aIndex) const;
	int FindSubString(String aSubString, int aStart=0);
	String ExtractSubString(const unsigned int aStart, const unsigned int aFinish);
	int Atoi();

    // export string to caller-supplied buffer mapping from fromchset.
    int Export( UTF16 *buffer, int& length, CharacterSet fromchset ) const;
    unsigned long ExportLength( CharacterSet tochset, CharacterSet fromchset ) const;

private:
	char* iRep;
	unsigned long iLength;
	};

// Compares the binary representation of two strings.
class StringLess
	{
public:
	bool operator()(const String&, const String&) const;
	};

extern String::CharacterSet SourceCharacterSet;
extern String::CharacterSet TargetCharacterSet;

#endif // __ASTRING_H__
