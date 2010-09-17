/* Copyright (C) 2010 Mobile Sorcery AB
 
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


#import "ConnectionCocoa.h"
#include "../connection.h"

/**
 * Bletooth Connection using cocoa API
 */
@implementation ConnectionCocoa

/**
 * Synthesize getters and setters
 */
@synthesize mAddress;
@synthesize mPort;
@synthesize mState;
@synthesize mReadQueue;
@synthesize mSemConnect;
@synthesize mConnectionOpen;
@synthesize mDataReady;
@synthesize mMutexRead;

/**
 * Open a serial port on mBluetoothDevice
 * Will run in a new cocoa thread.
 *
 * @param	This is just a dummy parameter, necessary
 *			for this function to be treated as a selector
 *			and be launched in a new cocoa thread
 */
- (void)openSerialPortProfile:(id)param
{
	// The following will be started in a new cocoa thread, from a pthread
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	IOBluetoothSDPUUID *sppServiceUUID;

	// Create an IOBluetoothSDPUUID object for the chat service UUID
	sppServiceUUID = [IOBluetoothSDPUUID 
					  uuid16:kBluetoothSDPUUID16ServiceClassSerialPort];

	// The device we want is the first in the array 
	// (even if the user somehow selected more than
	// one device in this example we care only about the first one):
	IOBluetoothDevice *device = [IOBluetoothDevice withAddress:mAddress];
	
	// Finds the service record that describes the service (UUID) we are looking for:
	IOBluetoothSDPServiceRecord	*sppServiceRecord = 
		[device getServiceRecordForUUID:sppServiceUUID];
	
	if ( sppServiceRecord == nil )
	{
		NSLog( @"Error - no spp service in selected device.\n" );
		mState = CONNERR_GENERIC;
		semaphore_signal(*mSemConnect);
		return;
	}
	
	// Open asyncronously the rfcomm channel when all the open sequence is 
	// completed my implementation of "rfcommChannelOpenComplete:" will be called.
	if ( ( [device openRFCOMMChannelAsync:&mRFCOMMChannel 
							withChannelID:mPort delegate:self] != kIOReturnSuccess ) 
		&& ( mRFCOMMChannel != nil ) )
	{
		// If the device connection is left open 
		// close it and return an error
		NSLog( @"Error - open sequence failed.***\n" );
		
		[self closeDeviceConnection:nil];
		mState = CONNERR_GENERIC;
		semaphore_signal(*mSemConnect);
		return;
		
	}
	
	// So far a lot of stuff went well, so we can assume that the device is
	// a good one and that rfcomm channel open process is going
	// well. So we keep track of the device and we (MUST) retain the RFCOMM channel:
	mBluetoothDevice = device;
	[mBluetoothDevice  retain];
	[mRFCOMMChannel retain];
	
	// Keep running the loop, otherwise the callbacks 
	// (e.g. rfcommChannelData) won't be called back...
	CFRunLoopRun();
	
	[pool release];
}

/**
 * Close the RFCOMM connection to mBluetoothDevice
 */
- (void)closeRFCOMMConnection
{
	mConnectionOpen = FALSE;
	[mRFCOMMChannel closeChannel];
}

/**
 * Close the device connection to mBluetoothDevice
 *
 * @param	This is just a dummy parameter, necessary
 *			for this function to be treated as a selector
 */
- (void)closeDeviceConnection:(id)param
{
	IOReturn error = [mBluetoothDevice closeConnection];
	if ( error != kIOReturnSuccess )
	{
		// I failed to close the connection, maybe the device is busy, no problem,
		// as soon as the device is no more busy it will close the connetion itself.
		NSLog(@"Error - failed to close the device connection with error %08lx.\n",
			  (UInt32)error);
		//return;
	}
	
	[mBluetoothDevice release];
	mBluetoothDevice = nil;
	
}

/**
 * Write data on the channel synchronously
 *
 * @param buffer	Buffer to be sent
 * @param length	Length of this buffer
 *
 * @return > 0          On success
 *         < 0 CONNERR  On failure
 */
- (int)sendData:(char*)buffer length:(UInt32)length
{
    if ( mRFCOMMChannel != nil )
    {
        UInt32				numBytesRemaining;
        IOReturn			result;
		BluetoothRFCOMMMTU	rfcommChannelMTU;
		
		numBytesRemaining = length;
		result = kIOReturnSuccess;
		
		// Get the RFCOMM Channel's MTU.  Each write can 
		// only contain up to the MTU size
		// number of bytes.
		rfcommChannelMTU = [mRFCOMMChannel getMTU];
		
		// Loop through the data until we have no more to send.
		while ( ( result == kIOReturnSuccess ) && ( numBytesRemaining > 0 ) )
		{
			// finds how many bytes I can send:
			UInt32 numBytesToSend = ( ( numBytesRemaining > rfcommChannelMTU )
									 ? rfcommChannelMTU :  numBytesRemaining );
			
			// This method won't return until the buffer has been passed
			// to the Bluetooth hardware to be sent to the remote device.
			// Alternatively, the asynchronous version of this method could
			// be used which would queue up the buffer and return immediately.
			result = [mRFCOMMChannel writeSync:buffer length:numBytesToSend];
			
			// Updates the position in the buffer:
			numBytesRemaining -= numBytesToSend;
			buffer += numBytesToSend;
		}
		
        // We are successful only if all the data was sent:
        if ( ( numBytesRemaining == 0 ) && ( result == kIOReturnSuccess ) )
		{
            return length;
		}
		else {
			return CONNERR_INTERNAL;
		}

    }
	else {
		return CONNERR_INTERNAL;
	}
}

/**
 * Called by the RFCOMM channel once the baseband
 * and rfcomm connection are completed
 * 
 * @param rfcommChannel		The channel which has been opened
 * @param error				The error returned
 */
- (void)rfcommChannelOpenComplete:(IOBluetoothRFCOMMChannel*)rfcommChannel
						   status:(IOReturn)error
{
	// If it failed to open the channel call our 
	// close routine and from there the code will
	// perform all the necessary cleanup:
	
	if ( error != kIOReturnSuccess )
	{	
		// Set the connection state and post the appropriate semaphore
		mState = CONNERR_GENERIC;
		semaphore_signal(*mSemConnect);
		NSLog(@"Error - failed to open the RFCOMM channel with error %08lx.\n",
			  (UInt32)error);
		[self rfcommChannelClosed:rfcommChannel];
		return;
	}
	else {
		// Set the connection state and post the appropriate semaphore
		mState = 1;
		semaphore_signal(*mSemConnect);
		mConnectionOpen = TRUE;
	}
}

/**
 * Called by the RFCOMM channel when new data
 * is received
 *
 * @param rfcommChannel	The channel which received data
 * @param dataPointer	Pointer to the data received
 * @param dataLength	Length of the data received
 */
- (void)rfcommChannelData:(IOBluetoothRFCOMMChannel *)rfcommChannel
					 data:(void *)dataPointer length:(size_t)dataLength
{
	pthread_mutex_lock (mMutexRead);
	if(mReadQueue == nil) {
		mReadQueue = [[NSMutableArray alloc] initWithCapacity:1];
	}
	NSData *dataAsBytes = [NSData dataWithBytes:dataPointer length:dataLength];

	[mReadQueue addObject:dataAsBytes];
	
	mDataReady = TRUE;
	pthread_mutex_unlock (mMutexRead);
}

/** 
 * Called by the RFCOMM channel when the connection is lost
 *
 * @param rfcommChannel	The channel which received data
 */
- (void)rfcommChannelClosed:(IOBluetoothRFCOMMChannel *)rfcommChannel
{
	// wait a second and close the device connection as well:
	mConnectionOpen = FALSE;
	[self performSelector:@selector(closeDeviceConnection:) 
			   withObject:nil afterDelay:1.0];
}


@end

