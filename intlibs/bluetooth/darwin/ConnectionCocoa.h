#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
#include <pthread.h>




@interface ConnectionCocoa : NSObject

{
	IOBluetoothDevice *mBluetoothDevice;
	IOBluetoothRFCOMMChannel *mRFCOMMChannel;
	BluetoothDeviceAddress* mAddress;
	int mPort;
	int mState;
	NSMutableArray *mReadQueue;
	
	pthread_cond_t *mConditionConnect;
	pthread_mutex_t *mMutexConnect;
	pthread_cond_t *mConditionRead;
	pthread_mutex_t *mMutexRead;	
}

@property(readwrite, assign) BluetoothDeviceAddress* mAddress;
@property(readwrite, assign) int mPort;
@property(readonly) int mState;
@property(readwrite,assign,nonatomic) pthread_cond_t *mConditionConnect;
@property(readwrite,assign,nonatomic) pthread_mutex_t *mMutexConnect;
@property(readwrite,assign,nonatomic) pthread_cond_t *mConditionRead;
@property(readwrite,assign,nonatomic) pthread_mutex_t *mMutexRead;
@property(readwrite, assign) NSMutableArray *mReadQueue;


// Methods to handle the Baseband and RFCOMM connection:
- (void)openSerialPortProfile:(id)param;
- (void)closeRFCOMMConnection;
- (void)closeDeviceConnection:(id)param;

// These are methods that are called when "things" happen on the
// bluetooth connection, read along and it will all be clearer:

// Called by the RFCOMM channel on us once the baseband and rfcomm connection is completed:
- (void)rfcommChannelOpenComplete:(IOBluetoothRFCOMMChannel*)rfcommChannel status:(IOReturn)error;

// Called by the RFCOMM channel on us when new data is received from the channel:
- (void)rfcommChannelData:(IOBluetoothRFCOMMChannel *)rfcommChannel data:(void *)dataPointer length:(size_t)dataLength;

// Called by the RFCOMM channel on us when something happens and the connection is lost:
- (void)rfcommChannelClosed:(IOBluetoothRFCOMMChannel *)rfcommChannel;

@end

