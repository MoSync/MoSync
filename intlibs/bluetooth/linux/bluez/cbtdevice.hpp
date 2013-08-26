/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
        mName[MAX_DEVICE_NAME] = 0;
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
    void getInfo ( MABtDeviceNative *p ) const
    {
        memcpy( &p->address, &mAddr, sizeof( MABtAddr ) );
        strncpy( p->name, mName, p->nameBufSize );
        p->actualNameLength = mostd::min<int>( p->nameBufSize, strlen( mName ) );
    }
};


#endif	/* _CBTDEVICE_HPP */

