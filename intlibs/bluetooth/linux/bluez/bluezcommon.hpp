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
 * File:   bluezcommon.hpp
 * Author: Ali Mosavian
 *
 * Created on July 30, 2009
 */

#ifndef __BLUEZCOMMON_HPP__
#define	__BLUEZCOMMON_HPP__

#include <algorithm>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include "helpers/types.h"
#include "helpers/cpp_defs.h"


/**
 * Bluetooth adress 00:40:05:60:3C:0F is stored as
 *
 *  - Bluez :  0F:3C:60:05:40:00
 *  - MoSync:  00:40:05:60:3C:0F
 */

/**
 * Converts a bluez bluetooth address to a mosync
 * bluetooth address.
 *
 * @param d    Pointer to a six byte array where the converted
 *             adress will be stored.
 * @param s    Pointer to a six byte array where the address
 *             is stored.
 */
static inline void ba2ma ( u8 *d, const u8 *s )
{
    for ( int i = 0; i < BTADDR_LEN; i++ )
        d[i] = s[BTADDR_LEN-i-1];
}


/**
 * Converts a mosync bluetooth address to a bluez
 * bluetooth adress.
 *
 * @param d    Pointer to a six byte array where the converted
 *             adress will be stored.
 * @param s    Pointer to a six byte array where the address
 *             is stored.
 */
static inline void ma2ba ( u8 *d, const u8 *s )
{
    for ( int i = 0; i < BTADDR_LEN; i++ )
        d[i] = s[BTADDR_LEN-i-1];
}


static inline int reverseByteOrder ( int a )
{
    uint8_t *b = (uint8_t*)&a;
    std::swap<uint8_t>( b[0], b[3] );
    std::swap<uint8_t>( b[1], b[2] );
    return a;
}

static inline void mauuid_to_uuid128 ( uuid_t &d, const MAUUID &s )
{
    uint32_t uuid[4];

    // Convert to big endian
    for ( int i = 0; i < 4; i++ )
        uuid[i] = reverseByteOrder( s.i[i] );

    sdp_uuid128_create( &d, &uuid );
}

static inline void uuid128_to_mauuid ( MAUUID &d, const uuid_t &s )
{
    const uint32_t *uuid = (const uint32_t *)&s.value;

    // Convert to litte endian
    for ( int i = 0; i < 4; i++ )
        d.i[i] = reverseByteOrder( uuid[i] );
}
#endif	/* __BLUEZCOMMON_HPP__ */

