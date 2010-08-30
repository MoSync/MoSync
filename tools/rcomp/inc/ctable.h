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
* Definitions supporting character set mapping functionality.
* Supported conversion is currently only between one of a set of
* supported 8-bit character sets and Unicode.
* Author:  Pute, January 1997.
*
*/


#ifndef __CTABLE_H__
#define __CTABLE_H__
#include <stdlib.h>
#include "wide.h"

#if defined(__VC32__)
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union
#endif

//
// Structures that can be easily initialised from an initialiser list.
// These are then used by the mapping classes defined underneath.
// An alternative mode of operation would be to initialise the
// classes from filed data, which would remove the need for the
// structures.
//


struct S_mapping_range
    {
    int first;
    int last;
    UTF16 mappings[32];
    };

// Intended to hold a set of contiguous mappings from characters in
// the range defined by first and last.





class Range
    {
private:
    int i_first;
    int i_last;
    // these define the range.  Arguably this could be a template class,
    // but the STL needs to be explored for this stuff.

public:

    Range ();
    Range ( int afirst, int alast );
    int count() const;                       // how many entries?
    int part_of( int avalue ) const;         // is avalue part of the range?
    int first() const;                       // return the first value
    void define ( int afirst, int alast );   // define new range.

    };



class Mapping_range
    {
private:
    Range range;
    UTF16 *mappings;

public:
    Mapping_range();
    Mapping_range( S_mapping_range *data );

    int map( unsigned char an8_bit_character, UTF16 &aUnicode_character ) const;
    //
    // If the 8-bit character is within range, the Unicode character is set
    // to its mapping, otherwise the Unicode character is unmodified.
    // The return value is true or false to indicate whether or not a
    // mapping took place.

    };


// Need to supply an escape mechanism for Unicode characters enbedded in CP1252 strings
const unsigned char UnicodeEscape = 0x81;	// indicates embedded UTF8 encoding of Unicode character


#endif // CTABLE_H not defined
// end of CTABLE.H

