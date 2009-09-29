/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/* 
 * File:   cbtdevice.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __CBTDEVICE_HPP__
#define	__CBTDEVICE_HPP__

#include <string>
#include <mostl/algorithm>
#include "helpers/types.h"
#include "helpers/helpers.h"
#include "helpers/cpp_defs.h"

#define MAX_DEVICE_NAME 248

class CBtDevice
{


private:
    MABtAddr    mAddr;
    char        mName[MAX_DEVICE_NAME+1];

public:
    /**
     * Constructor
     *
     * @param a     Pointer to the bluetooth device address
     * @param n     Pointer to the bluetooth device name
     */
    CBtDevice ( const MABtAddr *a,
                const char *n )    
    {
        memcpy( &mAddr, a, sizeof( MABtAddr ) );
        strncpy( mName, n, MAX_DEVICE_NAME+1 );
        mName[MAX_DEVICE_NAME] = NULL;
    }

    /**
     * Copy constructor
     *
     * @param d     Reference to instance
     */
    CBtDevice ( const CBtDevice &d )
    {
        *this = d;
    }

    /**
     * Destructor
     *
     */
    ~CBtDevice ( void )
    {
    }

    /**
     * Assignment operator
     *
     */
    void operator= ( const CBtDevice &d )
    {
        memcpy( &mAddr, &d.mAddr, sizeof( MABtAddr ) );
        strcpy( mName, d.mName );
    }

    /**
     * Copies adress and name to a MaBtDevice struct
     *
     * @param p     Pointer to a MABtDevice to fill
     */
    void getInfo ( MABtDevice *p ) const
    {
        memcpy( &p->address, &mAddr, sizeof( MABtAddr ) );
        strncpy( p->name, mName, p->nameBufSize );
        p->actualNameLength = mostd::min<int>( p->nameBufSize, strlen( mName ) );
    }
};


#endif	/* _CBTDEVICE_HPP */

