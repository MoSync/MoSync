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
 * File:   DiscoveryCocoa.h
 * Author: Romain Chalant
 *
 * Created on July 29, 2009
 */

#ifndef _DISCOVERY_COCOA_H_
#define _DISCOVERY_COCOA_H_

#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothSDPServiceRecord.h>


@class IOBluetoothDeviceInquiry;

/**
 * Discovery class for both devices and services
 */
@interface DiscoveryCocoa : NSObject
{
	IOBluetoothDeviceInquiry* _inquiry;
	int	_status;
    NSMutableArray*	_foundDevices;
	NSMutableArray*	_foundServices;
	IOBluetoothSDPUUID* _currentUUID;
	void (*_callback) (void);
}

@property (readwrite, assign) NSMutableArray* _foundDevices;
@property (readwrite, assign) NSMutableArray* _foundServices;
@property (readonly, assign) int _status;
@property (readwrite, assign) void (*_callback) (void);

/**
 * Starts a device discovery.
 *
 * @param withNames Perform name requests as well or not
 *
 * @return Success or failure
 */
-(IOReturn)startInquiry:(BOOL)withNames;

/**
 * Saves the new device in an array if it
 * has not been saved already
 *
 * @param inNewDevice The device to be saved
 *
 * @return	True if the device was added to the array
 *			False otherwise
 */
-(BOOL)saveNewDeviceIfAcceptable:(IOBluetoothDevice*)inNewDevice;

/**
 * Stops a running inquiry
 *
 * @return Success or failure
 */
-(IOReturn)stopInquiry;

/**
 * Starts a service discovery on a specified device
 *
 * @param addressPtr	Address of the device
 * @param uuid			UUID of the protocol to look for
 *
 * @return <0			If it failed
 *			0			If it suceeded
 */
-(int)startServiceDiscovery:(BluetoothDeviceAddress*)addressPtr serviceWithUUID:(IOBluetoothSDPUUID*)uuid;

@end

#endif
