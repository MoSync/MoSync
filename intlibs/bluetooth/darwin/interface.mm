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
 * File:   interface.mm
 * Author: Romain Chalant
 *
 * Created on July 29, 2009
 */
#include <cstdlib>
#include <list>
#import <IOBluetooth/objc/IOBluetoothHostController.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothSDPServiceRecord.h>
#include "../config_bluetooth.h"
#include "../btinit.h"
#include "../discovery.h"
#include "helpers/helpers.h"
#include "DiscoveryCocoa.h"


using namespace Bluetooth;

/**
 * Globals
 */
static DiscoveryCocoa *gDiscovery = NULL;
//static NSMutableArray *gFoundServices = NULL;



namespace Bluetooth
{


/**
 * Will initialize the bluetooth system
 *  
 */
void MABtInit ( void )
{
	gDiscovery = [[DiscoveryCocoa alloc] init];
}

/**
 * Will close the bluetooth system
 *
 */
void MABtClose ( void )
{	
	if ( gDiscovery == NULL )
	 {
	 LOGBT( "gDiscovery is NULL" );
	 return;
	 }
	 
	 [gDiscovery release];
}


/**
 * Returns the adress of the bluetooth interface
 *
 * @param a     Structure to fill with interface address
 *
 * @return < 0    On failure.
 */
int getLocalAddress ( MABtAddr& a )
{
	// Initialize the host controller and get the local address
	IOBluetoothHostController *hc = [[IOBluetoothHostController alloc] init];
	if(!hc) return CONNERR_INTERNAL;
	BluetoothDeviceAddress * localAddressPtr = new BluetoothDeviceAddress();
	[hc getAddress:localAddressPtr];
	// Convert the address to the MoSync format
	if (localAddressPtr) {
		a.a[0] = localAddressPtr->data[0];
		a.a[1] = localAddressPtr->data[1];
		a.a[2] = localAddressPtr->data[2];
		a.a[3] = localAddressPtr->data[3];
		a.a[4] = localAddressPtr->data[4];
		a.a[5] = localAddressPtr->data[5];
		delete(localAddressPtr);
		[hc release];
		return 0;
	}
	else {
		[hc release];
		delete(localAddressPtr);
		return CONNERR_INTERNAL;
	}
}


/**
 * Returns the device discovery state.
 *
 * @return	0 - Still working
 *          1 - Finished successfully
 *        < 0 - (CONNERR) Failed.
 */
int maBtDiscoveryState ( void )
{
	MAASSERT( gDiscovery != NULL );
	return gDiscovery._status;	
}


/**
 * Cancels an on going device discovery.
 * Note: If an operation was canceled, its last BT event will have 
 *       the status CONNERR_CANCELED. This is an asynchronous operation. 
 *       It is not safe to start another discovery before you've recieved 
 *       the CONNERR_CANCELED event.
 *
 * @return 0 if there was no active operation
 *         1 if there was.
 */
int maBtCancelDiscovery ( void )
{
	MAASSERT( gDiscovery != NULL );
	if (gDiscovery == NULL) {
		return 0;
	}
	else if (gDiscovery._status == 0) {
		[gDiscovery stopInquiry];
		return 1;
	}
	else {
		[gDiscovery stopInquiry];
		return 0;
	}
}


/**
 * Start a new device discovery operation in the background
 * and invokes the callback once it has finished.
 *
 * Note: Only one discovery operation can be active at
 * 	 a time.
 *
 * @param cb	- The callback which will be invoked once the
 *                discovery operation has finished.
 * @param n 	- Attempt to discover device names
 */
int maBtStartDeviceDiscovery ( MABtCallback cb,
                               bool n )
{	
	MAASSERT( gDiscovery != NULL );
	IOReturn status = [gDiscovery startInquiry:n];
	if (status == kIOReturnSuccess) {
		gDiscovery._callback = cb;
		return 0;
	}
	else {
		return CONNERR_INTERNAL;
	}
}


/**
 * Returns the next discovered device.
 *
 * @param d Pointer to the structure to fill.
 *
 * @return 1 if there was a device, 0 if not
 */
int maBtGetNewDevice ( MABtDevice* d )
{	
	if ( [gDiscovery._foundDevices count] != 0 ) {
		
		// Pop the next device from the stack
		IOBluetoothDevice *dev = [gDiscovery._foundDevices objectAtIndex:0];
		
		// Get the device address
		const BluetoothDeviceAddress * addressPtr = [dev getAddress];
		if (addressPtr) {
			// Convert Cocoa address to MoSync address
			d->address.a[0] = addressPtr->data[0];
			d->address.a[1] = addressPtr->data[1];
			d->address.a[2] = addressPtr->data[2];
			d->address.a[3] = addressPtr->data[3];
			d->address.a[4] = addressPtr->data[4];
			d->address.a[5] = addressPtr->data[5];
			// Fill the name field of the NABtDevice structure
			NSString* deviceNameString	= [dev getNameOrAddress];
			d->actualNameLength=deviceNameString.length;
			const char *cstr = new char[deviceNameString.length+1];
			cstr = [deviceNameString cStringUsingEncoding:NSASCIIStringEncoding];
			if (cstr == NULL) cstr = "NO NAME";
			strcpy(d->name, cstr);
		}
		// Remove the found device from the stack
		[gDiscovery._foundDevices removeObjectAtIndex:0];
		return 1;
	}
	else {
		return 0;
	}
}


/**
 * Starts a new service discovery operation given a device
 * adress and a family of services to search for.
 *
 * @param a    The address of the device to do service discovery on.
 * @param u    The service family UUID to search for.
 * @param cb   The callback to invoke once discovery has finished.
 */
int maBtStartServiceDiscovery ( const MABtAddr* a,
                                const MAUUID* u,
                                MABtCallback cb )
{
	MAASSERT( gDiscovery != NULL );
	// Convert MoSync address to Cocoa address
	BluetoothDeviceAddress * addressPtr = new BluetoothDeviceAddress();
	addressPtr->data[0] = a->a[0];
	addressPtr->data[1] = a->a[1];
	addressPtr->data[2] = a->a[2];
	addressPtr->data[3] = a->a[3];
	addressPtr->data[4] = a->a[4];
	addressPtr->data[5] = a->a[5];
	// Convert MoSync UUID to Cocoa address
	IOBluetoothSDPUUID * refUUID = [IOBluetoothSDPUUID uuid16:u->i[0]];
	// Set the callback
	gDiscovery._callback = cb;
	// Start the service discovery
	return [gDiscovery startServiceDiscovery: addressPtr serviceWithUUID:refUUID];
}

/**
 * Returns the next service, can be called once a service
 * discovery has finished.
 *
 * @param d   Pointer to structure to fill
 *
 * @return > 0 If there was any more services to get
 */
int maBtGetNewService ( MABtService* d )
{
    MAASSERT( gDiscovery != NULL );
	if ( [gDiscovery._foundServices count] != 0 ) {
		// Pop the next service from the stack
		IOBluetoothSDPServiceRecord * record = [gDiscovery._foundServices objectAtIndex:0];
		
		// Fill the name field of the NABtDevice structure
		NSString *serviceName = [record getServiceName];
		const char *cstr = new char[serviceName.length+1];
		cstr = [serviceName cStringUsingEncoding:NSASCIIStringEncoding];
		if (cstr == NULL) cstr = "NO NAME";
		strcpy(d->name, cstr);
		
		// Fill the port field of the NABtDevice structure
		BluetoothRFCOMMChannelID port;
		[record getRFCOMMChannelID:&port];
		d->port = port;
		
		// Fills the uuids field of the MABtService struture
		// Because there is no method for getting a list of protocol UUIDs
		// the service has, we do it by hand...
		// Protocol UUIDs are the hex numbers from 0x0001 to 0x0100
		// See Bluetooth reference documentation.
		std::list<MAUUID>   mUuidList;
		for(int i = 0x0001; i<=0x0100; i++) {
			IOBluetoothSDPUUID* refUUID = [IOBluetoothSDPUUID uuid16:i];
			NSArray *uuidArray = [NSArray arrayWithObject:refUUID];
			if ( [record hasServiceFromArray:uuidArray] ) {
				// If the current UUID is found, convert it to a MAUUID and put it in the list
				const MAUUID mauuid = {{ i, 0x00001000, 0x80000080, 0x5F9B34FB }};
				const MAUUID &mauuidRef = mauuid;
				mUuidList.push_back( mauuidRef );
			}
		}
		// Copy all the UUIDs found to the structure
		std::list<MAUUID>::iterator it;
		int i = 0;
		for ( it = mUuidList.begin( ); it != mUuidList.end( ); ++it )
			d->uuids[i++] = *it;

		// Remove the found service from the stack
		[gDiscovery._foundServices removeObjectAtIndex:0];
		return 1;
	}
	else {
		return 0;
	}

}


/**
 * Returns the size of the next service, of any.
 *
 * @param d     Pointer to the structure to fill.
 *
 * @return > 0 if there was a service, 0 if not
 */
int maBtGetNextServiceSize ( MABtServiceSize* d )
{
	MAASSERT( gDiscovery != NULL );
	if ( [gDiscovery._foundServices count] != 0 ) {
		
		// Pop the next service from the stack
		IOBluetoothSDPServiceRecord * record = [gDiscovery._foundServices objectAtIndex:0];
		
		// Fils the nameBufSize field of the MABtServiceSize structure
		NSString *serviceName = [record getServiceName];
		d->nameBufSize = serviceName.length;
		
		// Fills the nUuids field of the MABtServiceSize struture
		// Because there is no method for counting how many protocol UUIDs
		// the service has, we do it by hand...
		// Protocol UUIDs are the hex numbers from 0x0001 to 0x0100
		// See Bluetooth reference documentation.
		int nUuids = 0;
		for(int i = 0x0001; i<=0x0100; i++) {
			IOBluetoothSDPUUID* refUUID = [IOBluetoothSDPUUID uuid16:i];
			NSArray *uuidArray = [NSArray arrayWithObject:refUUID];
			if ( [record hasServiceFromArray:uuidArray] ) {
				nUuids++;
			}
		}
		d->nUuids      = nUuids;
		return 1;
	}
	else return 0; 
}

}
