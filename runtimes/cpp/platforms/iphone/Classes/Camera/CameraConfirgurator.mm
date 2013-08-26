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


#import "CameraConfirgurator.h"
#import <helpers/cpp_defs.h>

@implementation CameraConfirgurator


-(int)setCameraProperty: (AVCaptureDevice*)device withProperty:(NSString*)property withValue: (NSString*) value{
	[device retain];
	int result = MA_CAMERA_RES_OK;
	NSError *error = nil;
	if([device lockForConfiguration:&error])
	{

		if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_MODE]])
		{
			if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_ON]]) {
				if ([device isFlashModeSupported:AVCaptureFlashModeOn]) {
					device.flashMode = AVCaptureFlashModeOn;
					device.torchMode = AVCaptureTorchModeOff;
				}
				else {
					result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
				}
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_OFF]]) {
					if ([device isFlashModeSupported:AVCaptureFlashModeOff]) {
						device.flashMode = AVCaptureFlashModeOff;
						device.torchMode = AVCaptureTorchModeOff;
					}
					else {
						result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_AUTO]]) {
					if ([device isFlashModeSupported:AVCaptureFlashModeAuto]) {
						device.flashMode = AVCaptureFlashModeAuto;
						device.torchMode = AVCaptureTorchModeOff;
					}
					else {
						result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_TORCH]]) {
					if ([device isTorchModeSupported:AVCaptureTorchModeOn]) {
						device.flashMode = AVCaptureFlashModeOff;
						device.torchMode = AVCaptureTorchModeOn;
					}
					else {
						result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}
			}
			else {
				result = MA_CAMERA_RES_INVALID_PROPERTY_VALUE;
			}

		}
		else if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_MODE]])
		{
			if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_AUTO]]) {
				if ([device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
					device.focusMode = AVCaptureFocusModeContinuousAutoFocus;
				}
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_FIXED]]) {
				if ([device isFocusModeSupported:AVCaptureFocusModeLocked]) {
					device.focusMode = AVCaptureFocusModeLocked;
				}
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_INFINITY]]) {
				result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
			}
			else if ([value isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_MACRO]]) {
				result = MA_CAMERA_RES_VALUE_NOTSUPPORTED;
			}
			else {
				result = MA_CAMERA_RES_INVALID_PROPERTY_VALUE;
			}

		}
		else
		{
			result = MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
		}

		[device unlockForConfiguration];
	}
	else
	{
		result = MA_CAMERA_RES_FAILED;

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

		if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_MODE]])
		{
			if (device.torchMode == AVCaptureTorchModeOn) {
				result = [NSString stringWithUTF8String:MA_CAMERA_FLASH_TORCH];
			}
			else if(device.flashMode == AVCaptureFlashModeOn){
				result = [NSString stringWithUTF8String:MA_CAMERA_FLASH_ON];
			}
			else if(device.flashMode == AVCaptureFlashModeOff){
				result = [NSString stringWithUTF8String:MA_CAMERA_FLASH_OFF];
			}
			else if(device.flashMode == AVCaptureFlashModeAuto){
				result = [NSString stringWithUTF8String:MA_CAMERA_FLASH_AUTO];
			}
			else {
				result = [NSString stringWithUTF8String:MA_CAMERA_FLASH_OFF];
			}


		}
		else if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FOCUS_MODE]])
		{
			if (device.focusMode == AVCaptureFocusModeAutoFocus) {
				result = [NSString stringWithUTF8String:MA_CAMERA_FOCUS_AUTO];
			}
			else if (device.focusMode == AVCaptureFocusModeLocked) {
				result = [NSString stringWithUTF8String:MA_CAMERA_FOCUS_FIXED];
			}
			else {
				result = [NSString stringWithUTF8String:MA_CAMERA_FOCUS_AUTO];
			}
		}
		else if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_MAX_ZOOM]])
		{
			result = @"0";
		}
		else if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_ZOOM_SUPPORTED]])
		{
			result = @"false";
		}
		else if([property isEqualToString:[NSString stringWithUTF8String:MA_CAMERA_FLASH_SUPPORTED]])
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
