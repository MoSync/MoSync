/* Copyright (C) 2010 MoSync AB

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
