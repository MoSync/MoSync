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
#include <IOBluetooth/objc/IOBluetoothDevice.h>
#include <IOBluetoothUI/IOBluetoothUIUserLib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "ConnectionCocoa.h"
#include "ConnectionCocoaInterface.h"


using namespace Bluetooth::Darwin;
/**
 * Globals
 */
//static ConnectionCocoa *gConnection = NULL;

/**
 * Constructor, creates the socket
 *
 * @param a Pointer to bluetooth adress
 * @param p Port to connect to
 */
BtSppConnectionCocoa::BtSppConnectionCocoa ( const MABtAddr* a, uint p )
{
	mPort=p;
	// Convert MoSync address to cocoa address
	mAddressPtr = new BluetoothDeviceAddress();
	mAddressPtr->data[0] = a->a[0];
	mAddressPtr->data[1] = a->a[1];
	mAddressPtr->data[2] = a->a[2];
	mAddressPtr->data[3] = a->a[3];
	mAddressPtr->data[4] = a->a[4];
	mAddressPtr->data[5] = a->a[5];
	mConnection = [[ConnectionCocoa alloc] init];
	[mConnection retain];
}

/**
 * Destructor, will close any connection and free
 * resources
 *
 */
BtSppConnectionCocoa::~BtSppConnectionCocoa ( void )
{
	delete(mAddressPtr);
	delete(mConnection);
}


/**
 * Attempts to connect to the remote host
 *
 * @return > 0          On success
 *         < 0 CONNERR  On failure
 */
int BtSppConnectionCocoa::connect ( void )
{
	mConnection.mAddress = mAddressPtr;
	mConnection.mPort = mPort;
	
	// Using pthread instead of cocoa threads here,
	// because this class is created from a pthread.
	
	//Initialize condition and mutex
	pthread_cond_t conditionConnect;
	pthread_cond_init(&conditionConnect, NULL);
	pthread_mutex_t mutexConnect;
	pthread_mutex_init(&mutexConnect, NULL);
	
	// Pass condition and mutex to the obj-c class
	mConnection.mMutexConnect=&mutexConnect;
	mConnection.mConditionConnect=&conditionConnect;
	
	// Create the connection in a new cocoa thread
	// Here, we can use cocoa threads :)
	NSThread* connectionThread = [[NSThread alloc] initWithTarget:mConnection
								selector:@selector(openSerialPortProfile:)
								object:nil];
	[connectionThread start];
	
	// Wait until the connection state is set
	pthread_mutex_lock(&mutexConnect);
	pthread_cond_wait(&conditionConnect, &mutexConnect);
	pthread_mutex_unlock(&mutexConnect);
	pthread_mutex_destroy(&mutexConnect);
	
	// Now the connection state is set, just return it
	return mConnection.mState;
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
	//Initialize condition and mutex
	pthread_cond_t conditionRead;
	pthread_cond_init(&conditionRead, NULL);
	pthread_mutex_t mutexRead;
	pthread_mutex_init(&mutexRead, NULL);
	
	// Pass condition and mutex to the obj-c class
	mConnection.mMutexRead=&mutexRead;
	mConnection.mConditionRead=&conditionRead;
	
	// Wait until there is some data to be read
	pthread_mutex_lock(&mutexRead);
	pthread_cond_wait(&conditionRead, &mutexRead);
	pthread_mutex_unlock(&mutexRead);
	
	// Actually read the data
	NSData *nsdata = [mConnection.mReadQueue objectAtIndex:0];
	if(!nsdata) return 1;
	max = [nsdata length];
	const void *data = [nsdata bytes];
	memcpy(dst, data, max);
	[mConnection.mReadQueue removeObjectAtIndex:0];

	return max;
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
	// Signal that the last data must be read now
	pthread_mutex_lock(mConnection.mMutexRead);
	pthread_cond_signal(mConnection.mConditionRead);
	pthread_mutex_unlock(mConnection.mMutexRead);
	pthread_mutex_destroy(mConnection.mMutexRead);
	
	// Close the RFCOMM Channel
	[mConnection closeRFCOMMConnection];

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
