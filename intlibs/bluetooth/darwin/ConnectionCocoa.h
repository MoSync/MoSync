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
 * File:   ConnectionCocoa.h
 * Author: Romain Chalant
 *
 * Created on August 5, 2010
 */

#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
#include <pthread.h>
#include <mach/semaphore.h>
#include <mach/task.h>

/**
 * Bletooth Connection using cocoa API
 */
@interface ConnectionCocoa : NSObject

{
	IOBluetoothDevice *mBluetoothDevice;
	IOBluetoothRFCOMMChannel *mRFCOMMChannel;
	BluetoothDeviceAddress* mAddress;
	int mPort;
	int mState;
	NSMutableArray *mReadQueue;
	semaphore_t *mSemConnect;
	bool mConnectionOpen;
	bool mDataReady;
	pthread_mutex_t *mMutexRead;
}

@property(readwrite, assign) BluetoothDeviceAddress* mAddress;
@property(readwrite, assign) int mPort;
@property(readonly) int mState;
@property(readwrite,assign, nonatomic) semaphore_t *mSemConnect;
@property(readwrite, assign, nonatomic) NSMutableArray *mReadQueue;

@property(readonly, assign) bool mConnectionOpen;
@property(readwrite, assign) bool mDataReady;
@property(readwrite, assign, nonatomic) pthread_mutex_t *mMutexRead;

/**
 * Open a serial port on mBluetoothDevice
 * Will run in a new cocoa thread.
 *
 * @param	This is just a dummy parameter, necessary
 *			for this function to be treated as a selector
 *			and be launched in a new cocoa thread
 */
- (void)openSerialPortProfile:(id)param;

/**
 * Close the RFCOMM connection to mBluetoothDevice
 */
- (void)closeRFCOMMConnection;

/**
 * Close the device connection to mBluetoothDevice
 *
 * @param	This is just a dummy parameter, necessary
 *			for this function to be treated as a selector
 */
- (void)closeDeviceConnection:(id)param;

/**
 * Write data on the channel synchronously
 *
 * @param buffer	Buffer to be sent
 * @param length	Length of this buffer
 *
 * @return > 0          On success
 *         < 0 CONNERR  On failure
 */
- (int)sendData:(char*)buffer length:(UInt32)length;

/**
 * Called by the RFCOMM channel once the baseband
 * and rfcomm connection are completed
 * 
 * @param rfcommChannel		The channel which has been opened
 * @param error				The error returned
 */
- (void)rfcommChannelOpenComplete:(IOBluetoothRFCOMMChannel*)rfcommChannel
						   status:(IOReturn)error;

/**
 * Called by the RFCOMM channel when new data
 * is received
 *
 * @param rfcommChannel	The channel which received data
 * @param dataPointer	Pointer to the data received
 * @param dataLength	Length of the data received
 */
- (void)rfcommChannelData:(IOBluetoothRFCOMMChannel *)rfcommChannel 
					 data:(void *)dataPointer length:(size_t)dataLength;

/** 
 * Called by the RFCOMM channel when the connection is lost
 *
 * @param rfcommChannel	The channel which received data
 */
- (void)rfcommChannelClosed:(IOBluetoothRFCOMMChannel *)rfcommChannel;

@end

