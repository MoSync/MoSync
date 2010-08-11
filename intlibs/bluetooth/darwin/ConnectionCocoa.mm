//
//  ChatBluetoothInterface_ObjC.m
//  IOBluetoothFamily
//
//  Created by Eric Brown on Fri Apr 18 2003.
//  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
//

// Only compile this file if we are building the ObjC version of the app.



#import "ConnectionCocoa.h"





@implementation ConnectionCocoa

@synthesize mAddress;
@synthesize mPort;
@synthesize mState;
@synthesize mReadQueue;

@synthesize mMutexConnect;
@synthesize mConditionConnect;
@synthesize mMutexRead;
@synthesize mConditionRead;



- (void)openSerialPortProfile:(id)param
{
	// The following will be started in a new cocoa thread, from a pthread
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	IOBluetoothSDPUUID *sppServiceUUID;

	// Create an IOBluetoothSDPUUID object for the chat service UUID
	sppServiceUUID = [IOBluetoothSDPUUID uuid16:kBluetoothSDPUUID16ServiceClassSerialPort];

	// The device we want is the first in the array (even if the user somehow selected more than
	// one device in this example we care only about the first one):
	IOBluetoothDevice *device = [IOBluetoothDevice withAddress:mAddress];
	
	// Finds the service record that describes the service (UUID) we are looking for:
	IOBluetoothSDPServiceRecord	*sppServiceRecord = [device getServiceRecordForUUID:sppServiceUUID];
	
	if ( sppServiceRecord == nil )
	{
		NSLog( @"Error - no spp service in selected device.  ***This should never happen since the selector forces the user to select only devices with spp.***\n" );
		mState = -5;
	}
	
	// Open asyncronously the rfcomm channel when all the open sequence is completed my implementation of "rfcommChannelOpenComplete:" will be called.
	if ( ( [device openRFCOMMChannelAsync:&mRFCOMMChannel withChannelID:mPort delegate:self] != kIOReturnSuccess ) && ( mRFCOMMChannel != nil ) )
	{
		// Something went bad (looking at the error codes I can also say what, but for the moment let's not dwell on
		// those details). If the device connection is left open close it and return an error:
		NSLog( @"Error - open sequence failed.***\n" );
		
		[self closeDeviceConnection:nil];
		mState = -5;
		
	}
	
	// So far a lot of stuff went well, so we can assume that the device is a good one and that rfcomm channel open process is going
	// well. So we keep track of the device and we (MUST) retain the RFCOMM channel:
	mBluetoothDevice = device;
	[mBluetoothDevice  retain];
	[mRFCOMMChannel retain];
	
	// Set the connection state and signal mCondition that it is ready
	mState = 1;
	pthread_mutex_lock(mMutexConnect);
	pthread_cond_signal(mConditionConnect);
	pthread_mutex_unlock(mMutexConnect);
	
	// Keep running the loop, otherwise the callbacks (e.g. rfcommChannelData) won't be called back...
	CFRunLoopRun();
	
	[pool release];
}

- (void)closeRFCOMMConnection
{
	
	[mRFCOMMChannel closeChannel];
	
}

- (void)closeDeviceConnection:(id)param
{

	IOReturn error = [mBluetoothDevice closeConnection];
	if ( error != kIOReturnSuccess )
	{
		// I failed to close the connection, maybe the device is busy, no problem, as soon as the device is no more busy it will close the connetion itself.
		NSLog(@"Error - failed to close the device connection with error %08lx.\n", (UInt32)error);
	}
	
	[mBluetoothDevice release];
	mBluetoothDevice = nil;

	

}

// Called by the RFCOMM channel on us once the baseband and rfcomm connection is completed:
- (void)rfcommChannelOpenComplete:(IOBluetoothRFCOMMChannel*)rfcommChannel status:(IOReturn)error
{
	// If it failed to open the channel call our close routine and from there the code will
	// perform all the necessary cleanup:
	if ( error != kIOReturnSuccess )
	{
		NSLog(@"Error - failed to open the RFCOMM channel with error %08lx.\n", (UInt32)error);
		[self rfcommChannelClosed:rfcommChannel];
		return;
	}
	

	
	// The RFCOMM channel is now completly open so it is possible to send and receive data
	// ... add the code that begin the send data ... for example to reset a modem:
	//[rfcommChannel writeSync:"ATZ\n" length:4];
}

// Called by the RFCOMM channel on us when new data is received from the channel:
- (void)rfcommChannelData:(IOBluetoothRFCOMMChannel *)rfcommChannel data:(void *)dataPointer length:(size_t)dataLength
{

	if(mReadQueue == nil) {
		mReadQueue = [[NSMutableArray alloc] initWithCapacity:1];
		
	}
	NSData *dataAsBytes = [NSData dataWithBytes:dataPointer length:dataLength];

	[mReadQueue addObject:dataAsBytes];
	
	pthread_mutex_lock(mMutexRead);
	pthread_cond_signal(mConditionRead);
	pthread_mutex_unlock(mMutexRead);

}

// Called by the RFCOMM channel on us when something happens and the connection is lost:
- (void)rfcommChannelClosed:(IOBluetoothRFCOMMChannel *)rfcommChannel
{

	// wait a second and close the device connection as well:
	[self performSelector:@selector(closeDeviceConnection:) withObject:nil afterDelay:1.0];

}

@end

