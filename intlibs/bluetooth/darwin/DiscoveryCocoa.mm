#import <unistd.h>
#include <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
#import "DiscoveryCocoa.h"
#include "../discovery.h"
#include "../config_bluetooth.h"
#include "../btinit.h"
#include "../discovery.h"
#include "helpers/helpers.h"

/**
 * Discovery class for both devices and services
 */
@implementation DiscoveryCocoa

@synthesize _foundDevices;
@synthesize _foundServices;
@synthesize _status;
@synthesize _callback;

/**
 * Starts a device discovery.
 *
 * @param withNames Perform name requests as well or not
 *
 * @return Success or failure
 */
-(IOReturn)startInquiry:(BOOL)withNames
{
	IOReturn	status;
	
	[self stopInquiry];
	
	_inquiry = [IOBluetoothDeviceInquiry inquiryWithDelegate:self];
	[_inquiry setUpdateNewDeviceNames:withNames];
	
	status = [_inquiry	start];
	if( status == kIOReturnSuccess )
	{
		
		[_inquiry	retain];
		_status = 0;
	}
	else
	{
		_status = CONNERR_INTERNAL;
	}
	
	return( status );
}

/**
 * Called when the device discovery completes
 *
 * @param sender	The instance that requested an inquiry
 * @param error		Eventual error to be processed
 * @param aborted	True if the discovery was aborted
 */
- (void) deviceInquiryComplete:(IOBluetoothDeviceInquiry*)sender
					error:(IOReturn)error	
					aborted:(BOOL)aborted
{
	if( error )
	{
		_status = CONNERR_INTERNAL;
		
	}
	else
	{
		_status = 1;
		_callback ( );
	}
}

/**
 * Called when a device is found
 *
 * @param sender The instance that found a device
 * @param device The device that was found
 */
- (void) deviceInquiryDeviceFound:(IOBluetoothDeviceInquiry*)sender	
						   device:(IOBluetoothDevice*)device
{
	[self saveNewDeviceIfAcceptable:device];
}

/**
 * Saves the new device in an array if it
 * has not been saved already
 *
 * @param inNewDevice The device to be saved
 *
 * @return	True if the device was added to the array
 *			False otherwise
 */
-(BOOL)saveNewDeviceIfAcceptable:(IOBluetoothDevice*)inNewDevice
{
	NSEnumerator* enumerator;
	IOBluetoothDevice* tmpDevice;
	const BluetoothDeviceAddress*  newDeviceAddress = [inNewDevice getAddress];
	
	if( !_foundDevices )
	{
		_foundDevices = [[NSMutableArray alloc] initWithCapacity:1];
		if( !_foundDevices ) return( FALSE );
		[_foundDevices retain];
	}
	
	// walk the devices in the array.
	enumerator = [_foundDevices objectEnumerator];
	if( enumerator )
	{
		const BluetoothDeviceAddress* tempAddress = NULL;
		
		while( (tmpDevice = [enumerator nextObject]) )
		{
			tempAddress = [tmpDevice getAddress];
			
			if( memcmp( newDeviceAddress, tempAddress, 
					   sizeof( BluetoothDeviceAddress ) ) == 0 )
			{
				// Already have it.
				return( FALSE );
			}
		}
	}
	
	[_foundDevices addObject:inNewDevice];
	
	// Return that we haven't seen it.
	
	return( TRUE );
}

/**
 * Stops a running inquiry
 *
 * @return Success or failure
 */
- (IOReturn) stopInquiry
{
	IOReturn ret = kIOReturnNotOpen;
	
	if( _inquiry )
	{
		ret = [_inquiry stop];
		[_inquiry release];
		_inquiry = nil;
	}
	
	return ret;
}

/**
 * Starts a service discovery on a specified device
 *
 * @param addressPtr	Address of the device
 * @param uuid			UUID of the protocol to look for
 *
 * @return <0			If it failed
 *			0			If it suceeded
 */
-(int)startServiceDiscovery:(BluetoothDeviceAddress*)addressPtr serviceWithUUID:(IOBluetoothSDPUUID*)uuid
{
	if ( _status > 0 ) {
		_status = 0;
		if( !_foundServices )
		{
			_foundServices = [[NSMutableArray alloc] initWithCapacity:1];
			if( !_foundServices ) return( -1 );
			[_foundServices retain];
		}
		else {
			[_foundServices removeAllObjects];
		}

		IOBluetoothDevice  *dev = [IOBluetoothDevice withAddress:addressPtr];
		if(!dev) return CONNERR_INTERNAL;
		_currentUUID = uuid;
		[dev performSDPQuery:self];
		return 0;
	}
	else {
		_status = CONNERR_INTERNAL;
		return CONNERR_INTERNAL;
	}

}

/**
 * Called when the service discovery is completed
 * 
 * @param device	The device on which the discovery
 *					was performed
 * @param status	The status of the service discovery
 *					that just completed
 */
-(void)sdpQueryComplete:(IOBluetoothDevice *)device status:(IOReturn)status
{
	NSArray *servicesArray = [device getServices];
	NSArray *uuidArray = [NSArray arrayWithObject:_currentUUID];
	
	// Using fast enumeration
	for (IOBluetoothSDPServiceRecord *s in servicesArray) {
		if ( [s hasServiceFromArray:uuidArray] ) {
			[_foundServices addObject:s];
		}
	}
	
	//_status = [_foundServices count];
	_status = 1;
	
	// Post a MoSync event when the discovery is finished
	_callback ( );
}


@end