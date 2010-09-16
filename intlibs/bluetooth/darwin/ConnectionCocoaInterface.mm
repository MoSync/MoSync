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
* File:   ConnectionCocoaInterface.mm
* Author: Romain Chalant
*
* Created on August 5, 2010
*/
#include <IOBluetooth/objc/IOBluetoothDevice.h>
#include <IOBluetoothUI/IOBluetoothUIUserLib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "ConnectionCocoa.h"
#include "ConnectionCocoaInterface.h"

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
	// Convert MoSync address to cocoa address
	BluetoothDeviceAddress *addressPtr = new BluetoothDeviceAddress();
	addressPtr->data[0] = a->a[0];
	addressPtr->data[1] = a->a[1];
	addressPtr->data[2] = a->a[2];
	addressPtr->data[3] = a->a[3];
	addressPtr->data[4] = a->a[4];
	addressPtr->data[5] = a->a[5];
	
	// Initialize cocoa connection class
	mConnection = [[ConnectionCocoa alloc] init];
	
	// Initialize darwin kernel semaphores
	// (no counting POSIX semaphores on darwin)
	mConnection.mSemConnect = new semaphore_t;
	semaphore_create(mach_task_self(), mConnection.mSemConnect, SYNC_POLICY_FIFO, 0);
	
	// Initialize mutex for the read queue
	mConnection.mMutexRead = new pthread_mutex_t;
	pthread_mutex_init(mConnection.mMutexRead, NULL);
	
	// Pass address and port to mConnection
	mConnection.mAddress = addressPtr;
	mConnection.mPort = p;
}

/**
 * Destructor, will close any connection and free
 * resources
 *
 */
BtSppConnectionCocoa::~BtSppConnectionCocoa ( void )
{
	pthread_mutex_destroy(mConnection.mMutexRead);
	delete(mConnection.mMutexRead);
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
	// Create the connection in a new cocoa thread
	NSThread* connectionThread = [[NSThread alloc] initWithTarget:mConnection
								selector:@selector(openSerialPortProfile:)
								object:nil];
	[connectionThread start];
	
	// Wait until the connection state is set
	semaphore_wait( *mConnection.mSemConnect );
	
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
	// Check if the connection was established
	// then check if data is available
	// if there is no data, sleep for 50 ms and try again
	// otherwise, fetch the data
	while ( mConnection.mConnectionOpen ) {
		if( mConnection.mDataReady ) {
			
			while ( [mConnection.mReadQueue count] > 0 ) {
				
				pthread_mutex_lock (mConnection.mMutexRead);
				// Collect the garbage !
				NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
				// Get data from the queue
				NSData *nsdata = [mConnection.mReadQueue objectAtIndex:0];
				// If the data object is empty, remove it from the queue and break
				if( [nsdata length] == 0 ) {
				 	[mConnection.mReadQueue removeObjectAtIndex:0];
					[pool release];
					pthread_mutex_unlock (mConnection.mMutexRead);
					break;
				}
				// If max is too big, just read what we can
				if( (unsigned int)max > [nsdata length] ) {
					max = [nsdata length];
					NSRange readRange;
					readRange.location = 0;
					readRange.length = max;
					[nsdata getBytes: dst range:readRange];
					[mConnection.mReadQueue removeObjectAtIndex:0];
				}
				// Otherwise, get "max" bytes
				else {
					
					// Write max bytes to dst
					NSRange readRange;
					readRange.location = 0;
					readRange.length = max;
					[nsdata getBytes: dst range:readRange];
					
					// Get the remaining bytes
					NSRange remainingRange;
					remainingRange.location = max;
					remainingRange.length = [nsdata length] - max;
					char *remainingBytes = new char[remainingRange.length];
					[nsdata getBytes: remainingBytes range:remainingRange];
					NSData *remainingData = [NSData dataWithBytes:remainingBytes 
														   length:remainingRange.length];
					delete(remainingBytes);
					// Replace the data object from which we read max bytes
					// by one that contains only unread bytes
					[mConnection.mReadQueue replaceObjectAtIndex:0 
													  withObject:remainingData];
				}
				[pool release];
				pthread_mutex_unlock (mConnection.mMutexRead);
				return max;
			}
			
			mConnection.mDataReady = FALSE;
		}
		else {
			usleep(50000);
		}
	}
	return CONNERR_CLOSED;
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
	// Check if the connection was established
	if ( mConnection.mState > 0 ) {
		char *buf = new char[len];
		memcpy(buf, src, len);
		int result = [mConnection sendData:buf length:len];
		delete(buf);
		return result;
	}
	else {
		return INVALID_SOCKET;
	}
}

/**
 * Closes any open connections
 *
 */
void BtSppConnectionCocoa::close ( void )
{	
	// Check if the connection was established
	if ( mConnection.mState > 0 ) {
		
		// Wait for the last data
		//semaphore_wait( *mConnection.mSemRead );
		
		// Close the RFCOMM Channel after 1sec
		[mConnection closeRFCOMMConnection];
		
		
	}
}

/**
 * Returns the adress of the remote host.
 *
 * @param a     Structure to fill
 */
int BtSppConnectionCocoa::getAddr ( MAConnAddr& a )
{
	// Check if the connection was established
	if ( mConnection.mState > 0 ) {
		a.family  = CONN_FAMILY_BT;
		a.bt.port = mConnection.mPort;
		// Convert the address to the MoSync format
		a.bt.addr.a[0] = mConnection.mAddress->data[0];
		a.bt.addr.a[1] = mConnection.mAddress->data[1];
		a.bt.addr.a[2] = mConnection.mAddress->data[2];
		a.bt.addr.a[3] = mConnection.mAddress->data[3];
		a.bt.addr.a[4] = mConnection.mAddress->data[4];
		a.bt.addr.a[5] = mConnection.mAddress->data[5];
		return 1;
	}	
    else {
		return INVALID_SOCKET;
	}
}
