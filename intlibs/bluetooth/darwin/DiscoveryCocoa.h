#ifndef _DISCOVERY_COCOA_H_
#define _DISCOVERY_COCOA_H_

#import <IOBluetooth/objc/IOBluetoothDevice.h>



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
	void (*_callback) (void);
	
}

// misc.

@property (readwrite, assign) NSMutableArray* _foundDevices;
@property (readonly, assign) int _status;
@property (readwrite, assign) void (*_callback) (void);

-(void)addDeviceToList:(IOBluetoothDevice*)inDeviceRef;
-(BOOL)saveNewDeviceIfAcceptable:(IOBluetoothDevice*)inNewDevice;
//-(void)deviceListDoubleAction;
//-(void)updateDeviceInfoInList:(IOBluetoothDevice *)inDevice;
-(IOReturn)startInquiry:(BOOL)withNames;
-(IOReturn)stopInquiry;


@end

#endif
