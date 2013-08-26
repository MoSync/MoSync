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

