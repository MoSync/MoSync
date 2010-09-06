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
* Implements character set mapping functionality.
*
*/


#include "ctable.h"



// ++++++ Range member functions ++++++



Range::Range()
    {
    i_first = i_last = 0;
    }


Range::Range( int afirst, int alast )
    {
    define(afirst, alast);
    }

void Range::define( int afirst, int alast )
    {
    // ensure that the range is stored in the right order.
    if ( afirst > alast )
        {
        i_first = alast;
        i_last = afirst;
        }
    else
        {
        i_first = afirst;
        i_last = alast;
        }
    }

int Range::count() const
    {
    return ( i_last - i_first +1 );
    }

int Range::first() const
    {
    return(i_first);
    }


int Range::part_of( int avalue ) const
    {
    // return TRUE if the value is in the range, else FALSE

    if ( (avalue >= i_first) && ( avalue <= i_last ) ) return 1;
    else return 0;

    }


// ++++++ Mapping_range member functions ++++++


Mapping_range::Mapping_range()
    {
    mappings = NULL;
    }

Mapping_range::Mapping_range( S_mapping_range *data )
    {
    range.define(data->first, data->last);
    mappings = &data->mappings[0];
    }



int Mapping_range::map(  unsigned char an8_bit_character, UTF16 &aUnicode_character ) const
    {
    if ( !range.part_of(an8_bit_character) ) return (0);

    int index = (int)an8_bit_character - range.first();
    aUnicode_character = mappings[index];
    return (1);
    }





// end of CTABLE.CPP
