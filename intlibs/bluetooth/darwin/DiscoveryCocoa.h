#import <IOBluetooth/objc/IOBluetoothDevice.h>



@class IOBluetoothDeviceInquiry;

//===========================================================================================================================
// Discovery
//===========================================================================================================================

@interface DiscoveryCocoa : NSObject
{
    
	
	IOBluetoothDeviceInquiry *		_inquiry;
	BOOL							_busy;
	
    NSMutableArray*					_foundDevices;
	int testInt;
	
}

// misc.

@property (readwrite,assign) NSMutableArray* _foundDevices;
@property int testInt;

-(void)addDeviceToList:(IOBluetoothDevice*)inDeviceRef;
-(BOOL)saveNewDeviceIfAcceptable:(IOBluetoothDevice*)inNewDevice;
//-(void)deviceListDoubleAction;
//-(void)updateDeviceInfoInList:(IOBluetoothDevice *)inDevice;
-(IOReturn)startInquiry;
//-(IOReturn)stopInquiry;


@end
