

#import "CameraConfirgurator.h"


@implementation CameraConfirgurator


-(int)setCameraProperty: (AVCaptureDevice*)device withProperty:(NSString*)property withValue: (NSString*) value{
	[device retain];
	int result = 0;
	NSError *error = nil;
	if([device lockForConfiguration:&error])
	{
		
		if([property isEqualToString:@"flash-mode"])
		{
			if ([value isEqualToString:@"on"]) {
				if ([device isFlashModeSupported:AVCaptureFlashModeOn]) {
					device.flashMode = AVCaptureFlashModeOn;
				}
				if ([device isTorchModeSupported:AVCaptureTorchModeOff]) {
					device.torchMode = AVCaptureTorchModeOff;
				}
						  
			}
			else if ([value isEqualToString:@"off"]) {
					if ([device isFlashModeSupported:AVCaptureFlashModeOff]) {
						device.flashMode = AVCaptureFlashModeOff;
					}
					if ([device isTorchModeSupported:AVCaptureTorchModeOff]) {
						device.torchMode = AVCaptureTorchModeOff;
					}
			}
			else if ([value isEqualToString:@"auto"]) {
					if ([device isFlashModeSupported:AVCaptureFlashModeAuto]) {
						device.flashMode = AVCaptureFlashModeAuto;
					}
					if ([device isTorchModeSupported:AVCaptureTorchModeOff]) {
						device.torchMode = AVCaptureTorchModeOff;
					}
			}
			else if ([value isEqualToString:@"torch"]) {
					if ([device isFlashModeSupported:AVCaptureFlashModeOff]) {
						device.flashMode = AVCaptureFlashModeOff;
					}
					if ([device isTorchModeSupported:AVCaptureTorchModeOn]) {
						device.torchMode = AVCaptureTorchModeOn;
					}
			}
			else {
				result = -2;
			}

		}
		else if([property isEqualToString:@"focus-mode"]) 
		{
			if ([value isEqualToString:@"auto"]) {
				if ([device isFocusModeSupported:AVCaptureFocusModeAutoFocus]) {
					device.focusMode = AVCaptureFocusModeAutoFocus;
				}
			}
			else if ([value isEqualToString:@"fixed"]) {
				if ([device isFocusModeSupported:AVCaptureFocusModeLocked]) {
					device.focusMode = AVCaptureFocusModeLocked;
				}
			}
			else {
				result = -2;
			}

			
		}

		else 
		{
			result = -2;
		}
		
		[device unlockForConfiguration];
	}
	else 
	{
		result = -2;
		
	}	
	[device release];
	[error release];
	return result;
}

-(NSString *)getCameraProperty: (AVCaptureDevice*)device withProperty:(NSString*)property{
	[device retain];
	NSString *result = nil;
	NSError *error = nil;
	if([device lockForConfiguration:&error])
	{
		
		if([property isEqualToString:@"flash-mode"])
		{
			if (device.torchMode == AVCaptureTorchModeOn) {
				result = @"torch";
			}
			else if(device.flashMode == AVCaptureFlashModeOn){
				result = @"on";
			}
			else if(device.flashMode == AVCaptureFlashModeOff){
				result = @"off";
			}
			else if(device.flashMode == AVCaptureFlashModeAuto){
				result = @"auto";
			}
			else {
				result = @"off";
			}

			
		}
		else if([property isEqualToString:@"focus-mode"]) 
		{
			if (device.focusMode == AVCaptureFocusModeAutoFocus) {
				result = @"auto";
			}
			else if (device.focusMode == AVCaptureFocusModeLocked) {
				result = @"fixed";
			}
			else {
				result = @"auto";
			}
		}
		else if([property isEqualToString:@"zoom-supported"]) 
		{
			result = @"false";
		}
		else if([property isEqualToString:@"flash-supported"]) 
		{
			if (device.hasFlash) {
				result = @"true";
			}
			else {
				result = @"false";
			}
		}
		else 
		{
			result =nil;
		}
		
		[device unlockForConfiguration];
	}
	else 
	{
		result = nil;
		
	}	
	[device release];
	[error release];
	return result;
	
}
@end
