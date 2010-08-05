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
* File:   ConnectionCocoa.mm
* Author: Romain Chalant
*
* Created on August 5, 2010
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>



#include "bluetooth/config_bluetooth.h"
#include "ConnectionCocoa.h"
#include "helpers/helpers.h"



#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

using namespace Bluetooth::Darwin;


/**
 * Constructor, creates the socket
 *
 * @param a Pointer to bluetooth adress
 * @param p Port to connect to
 */
BtSppConnectionCocoa::BtSppConnectionCocoa ( const MABtAddr* a, uint p )
{
   /* mAddr.rc_family = AF_BLUETOOTH;
    mAddr.rc_channel = (uint8_t)p;
    // Convert MoSync address to bluez adress
    ma2ba( mAddr.rc_bdaddr.b, a->a );*/
}

/**
 * Destructor, will close any connection and free
 * resources
 *
 */
BtSppConnectionCocoa::~BtSppConnectionCocoa ( void )
{
    //close( );
}


/**
 * Attempts to connect to the remote host
 *
 * @return > 0          On success
 *         < 0 CONNERR  On failure
 */
int BtSppConnectionCocoa::connect ( void )
{
    /*if ( mSocket != INVALID_SOCKET )
    {
        LOGBT( "Socket already connected" );
        return CONNERR_INTERNAL;
    }

    // Create socket
    mSocket = socket( AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM );
    if ( mSocket == INVALID_SOCKET )
    {
        LOGBT( "Failed to create socket" );
        return CONNERR_INTERNAL;
    }

    // Attempt to connect
    if ( ::connect( mSocket, (sockaddr *)&mAddr, sizeof( sockaddr_rc ) ) == -1 )
    {
        LOGBT( "Failed to connect" );
        ::close( mSocket );
        mSocket = INVALID_SOCKET;
        return CONNERR_GENERIC;
    }

    // TODO: Explcicitly put in blocking mode ?
*/
    return 1;
}

/**
 * Reads one to max bytes from the the socket without blocking.
 *
 * @param dst   Pointer to the buffer to write to.
 * @param max   Maximum number of bytes to read.
 *
 * @return Number of read bytes or (0 < ) CONNERR if failed.
 */
int BtSppConnectionCocoa::read ( void* dst, int max )
{
   /* if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    int res = recv( mSocket, dst, max, 0 );
    if ( res == 0 )
        return CONNERR_CLOSED;
    else if ( res < 0 )
        return CONNERR_GENERIC;
    else
        return res;*/
	return 0;
}

/**
 * Sends the supplied number of bytes to the destination.
 *
 * @param src   Pointer to the buffer that holds the data.
 * @param len   Number of bytes to send.
 *
 * @return > 0           The data has been sent.
 *         0 < (CONNERR) If there was an error.
 */
int BtSppConnectionCocoa::write ( const void* src, int len )
{
    /*if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    int res = send( mSocket, src, len, 0 );
    if ( res < 0 )
        return CONNERR_GENERIC;
    else
        return res;*/
	return 0;
}

/**
 * Closes any open connections
 *
 */
void BtSppConnectionCocoa::close ( void )
{
   /* if ( mSocket == INVALID_SOCKET )
        return;

    // Send any waiting data
    shutdown( mSocket, SHUT_RDWR );

    // Close descriptor
    ::close( mSocket );
    mSocket = INVALID_SOCKET;*/
}

/**
 * Returns the adress of the remote host.
 *
 * @param a     Structure to fill
 */
int BtSppConnectionCocoa::getAddr ( MAConnAddr& a )
{
   /* if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    a.family  = CONN_FAMILY_BT;
    a.bt.port = mAddr.rc_channel;

    // Convert bluez address to MoSync address
    ba2ma( a.bt.addr.a, mAddr.rc_bdaddr.b );*/
    return 1;
}
