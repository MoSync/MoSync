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
 * File:   connectionbluez.cpp
 * Author: Ali Mosavian
 *
 * Created on July 29, 2009
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "bluezcommon.hpp"
#include "bluetooth/config_bluetooth.h"
#include "connectionbluez.hpp"
#include "helpers/helpers.h"



#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

using namespace Bluetooth::BlueZ;


/**
 * Constructor, creates the socket
 *
 * @param a Pointer to bluetooth adress
 * @param p Port to connect to
 */
BtSppConnectionBluez::BtSppConnectionBluez ( const MABtAddr* a, uint p )
:mSocket( INVALID_SOCKET )
{
    mAddr.rc_family = AF_BLUETOOTH;
    mAddr.rc_channel = (uint8_t)p;
    // Convert MoSync address to bluez adress
    ma2ba( mAddr.rc_bdaddr.b, a->a );
}

/**
 * Destructor, will close any connection and free
 * resources
 *
 */
BtSppConnectionBluez::~BtSppConnectionBluez ( void )
{
    close( );
}


/**
 * Attempts to connect to the remote host
 *
 * @return > 0          On success
 *         < 0 CONNERR  On failure
 */
int BtSppConnectionBluez::connect ( void )
{
    if ( mSocket != INVALID_SOCKET )
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
int BtSppConnectionBluez::read ( void* dst, int max )
{
    if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    int res = recv( mSocket, dst, max, 0 );
    if ( res == 0 )
        return CONNERR_CLOSED;
    else if ( res < 0 )
        return CONNERR_GENERIC;
    else
        return res;
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
int BtSppConnectionBluez::write ( const void* src, int len )
{
    if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    int res = send( mSocket, src, len, 0 );
    if ( res < 0 )
        return CONNERR_GENERIC;
    else
        return res;
}

bool BtSppConnectionBluez::isConnected() {
	return mSocket != INVALID_SOCKET;
}

/**
 * Closes any open connections
 *
 */
void BtSppConnectionBluez::close ( void )
{
    if ( mSocket == INVALID_SOCKET )
        return;

    // Send any waiting data
    shutdown( mSocket, SHUT_RDWR );

    // Close descriptor
    ::close( mSocket );
    mSocket = INVALID_SOCKET;
}

/**
 * Returns the adress of the remote host.
 *
 * @param a     Structure to fill
 */
int BtSppConnectionBluez::getAddr ( MAConnAddr& a )
{
    if ( mSocket == INVALID_SOCKET )
        return CONNERR_GENERIC;

    a.family  = CONN_FAMILY_BT;
    a.bt.port = mAddr.rc_channel;

    // Convert bluez address to MoSync address
    ba2ma( a.bt.addr.a, mAddr.rc_bdaddr.b );
    return 1;
}
