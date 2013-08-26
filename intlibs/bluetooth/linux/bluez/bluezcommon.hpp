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

