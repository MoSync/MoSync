#ifndef _DISCOVERY_COCOA_H_
#define _DISCOVERY_COCOA_H_

#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothSDPServiceRecord.h>


@class IOBluetoothDeviceInquiry;

//===========================================================================================================================
// Discovery
//===========================================================================================================================

@interface DiscoveryCocoa : NSObject
{
    
	
	IOBluetoothDeviceInquiry *		_inquiry;
	BOOL							_busy;
	int								_status;
    NSMutableArray*					_foundDevices;
	NSMutableArray*					_foundServices;
	IOBluetoothSDPUUID*				_currentUUID;
	void (*_callback) (void);
	
}

// misc.

@property (readwrite, assign) NSMutableArray* _foundDevices;
@property (readwrite, assign) NSMutableArray* _foundServices;
@property (readonly, assign) int _status;
@property (readwrite, assign) void (*_callback) (void);

-(void)addDeviceToList:(IOBluetoothDevice*)inDeviceRef;
-(BOOL)saveNewDeviceIfAcceptable:(IOBluetoothDevice*)inNewDevice;
-(IOReturn)startInquiry:(BOOL)withNames;
-(IOReturn)stopInquiry;
-(int)startServiceDiscovery:(BluetoothDeviceAddress*)addressPtr serviceWithUUID:(IOBluetoothSDPUUID*)uuid;


@end

#endif
