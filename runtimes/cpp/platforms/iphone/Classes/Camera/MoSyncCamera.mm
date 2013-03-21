/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

#import <AVFoundation/AVFoundation.h>

#import "MoSyncCamera.h"
#import "CameraConfirgurator.h"
#import "CameraPreviewWidget.h"
#import "CameraPreviewEventHandler.h"
#import "MoSyncMain.h"

#include "MoSyncUISyscalls.h"
#include "MemStream.h"
#include "MoSyncGraphics.h"

//This struct holds information about what resources are connected
//to a single camera. Each device camera has it's own instance
//(So, one for most phones, and two for iPhone 4, for example)
struct CameraInfo {
	AVCaptureSession *captureSession;
	AVCaptureVideoPreviewLayer *previewLayer;
	AVCaptureDevice *device; //The physical camera device
	AVCaptureStillImageOutput *stillImageOutput;
	AVCaptureVideoDataOutput *videoDataOutput;
	UIView *view;
};

//There is only a single instance of this struct, and it holds info about the
//devices on the system, as well as which one is the selected one for the camera
//syscalls
struct CameraSystemInfo {
	int numCameras;
	int currentCamera;
	BOOL initialized;
	CameraInfo *cameraInfo;
};

CameraSystemInfo gCameraSystem={0,0,FALSE,NULL};

CameraConfirgurator *gCameraConfigurator;

CameraPreviewEventHandler* gCameraPreviewEventHandler = NULL;

//This performs lazy initialization of the camera system, the first time
//a relevant camera syscall is called.
void initCameraSystem()
{

	if( gCameraSystem.initialized == FALSE )
	{
		gCameraConfigurator = [[CameraConfirgurator alloc] init];
		CameraInfo *cameraInfo;
		int numCameras = 0;

		//This will also include microphones and maybe other, non camera devices
		NSArray *devices = [AVCaptureDevice devices];
		AVCaptureDevice *backCamera = NULL;
		AVCaptureDevice *frontCamera = NULL;

		for ( AVCaptureDevice *device in devices)
		{
			//This weeds out the devices we don't need
			if ( [device hasMediaType:AVMediaTypeVideo] )
			{
				numCameras++;
				//The following code assumes that all cameras not facing back,
				//will be facing forward. This works for the current phones,
				//but it could probably fail if Apple ever introduces a device
				//with three or more cameras
				if ( [device position] == AVCaptureDevicePositionBack )
				{
					backCamera = device;
				}
				else
				{
					frontCamera = device;
				}
			}
		}

		if( numCameras > 0 )
		{
			int positionCounter = 0;
			cameraInfo = new CameraInfo[numCameras];

			//Back facing camera should be first, then front facing, then the rest
			if ( backCamera != NULL )
			{
				cameraInfo[positionCounter].device = backCamera;
				positionCounter++;
			}
			if ( frontCamera != NULL )
			{
				cameraInfo[positionCounter].device = frontCamera;
				positionCounter++;
			}

			for ( AVCaptureDevice *device in devices )
			{
				if ( [device hasMediaType:AVMediaTypeVideo ] &&
					device != backCamera && device != frontCamera)
				{
					cameraInfo[positionCounter].device = device;
					positionCounter++;
				}
			}

			for (int i=0; i<numCameras; i++) {
				cameraInfo[i].captureSession = NULL;
				cameraInfo[i].previewLayer = NULL;
				cameraInfo[i].view = NULL;
			}
		}

		gCameraSystem.numCameras = numCameras;
		gCameraSystem.cameraInfo = cameraInfo;
		gCameraSystem.initialized = TRUE;
	}
}

//This function not only returns information about the currently selected amera, but
//also performs lazy initialization on the session object
CameraInfo *getCurrentCameraInfo()
{
	initCameraSystem();

	if( gCameraSystem.numCameras == 0 )
	{
		return NULL;
	}

	CameraInfo *curCam = &gCameraSystem.cameraInfo[ gCameraSystem.currentCamera ];
	if( curCam->captureSession == NULL ) {

		curCam->captureSession = [[AVCaptureSession alloc] init];

		NSError *error = nil;
		AVCaptureDeviceInput *input =
		[AVCaptureDeviceInput deviceInputWithDevice:curCam->device error:&error];
		curCam->stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
		NSDictionary *outputSettings = [[NSDictionary alloc] initWithObjectsAndKeys:
										AVVideoCodecJPEG, AVVideoCodecKey, nil];
		[curCam->stillImageOutput setOutputSettings:outputSettings];
		//set up video output as well
		curCam->videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
		outputSettings = [NSDictionary dictionaryWithObject:
						  [NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey];  //this format is recommended for iphone 3/4
		curCam->videoDataOutput.videoSettings = outputSettings;
		[outputSettings release];
		if ([curCam->captureSession canSetSessionPreset:AVCaptureSessionPresetMedium]) {
			curCam->captureSession.sessionPreset = AVCaptureSessionPresetMedium;
		}
		[curCam->captureSession addInput:input];
		[curCam->captureSession addOutput:curCam->stillImageOutput];
		//perhaps init the videoDataOutput here as well?
	}
	return curCam;
}

void StopAllCameraSessions()
{
	if( gCameraSystem.initialized == TRUE )
	{
		for ( int i = 0; i < gCameraSystem.numCameras; i++ )
		{
			if( gCameraSystem.cameraInfo[i].captureSession )
			{
				[gCameraSystem.cameraInfo[i].captureSession stopRunning];
			}
		}
	}
}

SYSCALL(int, maCameraStart())
{
	@try {
		CameraInfo *info = getCurrentCameraInfo();
		if( info )
		{
			//In this case, no preview widget was assigned to this camera.
			//Run the sublayer to the main mosync view at full screen
			if( !info->view )
			{
				if( !info->previewLayer )
				{
					info->previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:info->captureSession];
				}

				MoSync_AddLayerToView(info->previewLayer);
				MoSync_UpdateView(gBackbuffer->image);
			}
			else
			{
				info->previewLayer.frame = info->view.bounds;
			}

			//Have to do it this way, because otherwise it hijacks the main thread or something wierd
			[info->captureSession	performSelectorOnMainThread:@selector(startRunning)
												   withObject:nil
												waitUntilDone:YES];

			if(info->device.torchMode == AVCaptureTorchModeOn)
			{
				[info->device lockForConfiguration:nil];
				info->device.torchMode = AVCaptureTorchModeOff;
				info->device.torchMode = AVCaptureTorchModeOn;
				[info->device unlockForConfiguration];
			}
		}
		return 1;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraStop())
{
	@try {
		CameraInfo *info = getCurrentCameraInfo();
		if( info )
		{
			[info->captureSession stopRunning];

			//In this case, we don't have a preview widget,
			//so we need to remove the layer from the main view.
			if ( !info->view ) {
				[info->previewLayer removeFromSuperlayer];
			}
		}
		return 1;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraSetPreview(MAHandle widgetHandle))
{
	@try {
		CameraPreviewWidget *widget = (CameraPreviewWidget*) [getMoSyncUI() getWidget:widgetHandle];
		if( !widget )
		{
			return 0;
		}

		UIView *newView = widget.view;

		CameraInfo *info = getCurrentCameraInfo();
		if (!info)
		{
			// Camera is not available.
			return MA_CAMERA_RES_FAILED;
		}
		if( !info->previewLayer )
		{
			info->previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:info->captureSession];
			//info->previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
		}

		if ( info->view )
		{
			//Remove the preview layer from the other view
			[info->previewLayer removeFromSuperlayer];
		}

		//If this widget was assigned to another camera, we need to remove that
		//other camera's preview layer from it.
		for ( int i=0; i<gCameraSystem.numCameras; i++ ) {
			if( gCameraSystem.cameraInfo[i].view == newView )
			{
				[gCameraSystem.cameraInfo[i].previewLayer removeFromSuperlayer];
				gCameraSystem.cameraInfo[i].view = NULL;
			}
		}

		info->view = newView;
		widget.previewLayer = info->previewLayer;
		[info->view.layer addSublayer:info->previewLayer];
		info->previewLayer.frame = info->view.bounds;
		[info->view.layer needsLayout];

		//We need to "reset" the view in case we switched sublayers (because we switched cameras)
		//First we hide the view
		info->view.hidden = YES;

		//Then we tell the main thread (that's outside the user code loop) to show it again.
		//We can't do it on the same "calling circle" because the UI system won't know that we
		//tried to show and hide the view
		[widget performSelectorOnMainThread:@selector(showViewAgain) withObject:nil waitUntilDone:NO];
		return 1;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraSelect(MAHandle cameraNumber))
{
	@try {
		initCameraSystem();

		if (cameraNumber < 0 || cameraNumber >=gCameraSystem.numCameras) {
			return 0;
		}

		gCameraSystem.currentCamera = cameraNumber;
		return 1;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraNumber())
{
	@try {
		initCameraSystem();
		return gCameraSystem.numCameras;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraSnapshot(int formatIndex, MAHandle placeholder))
{
	@try {
		CameraInfo *info = getCurrentCameraInfo();
		if (!info)
		{
			// Camera is not available.
			return MA_CAMERA_RES_FAILED;
		}

		AVCaptureConnection *videoConnection =	[info->stillImageOutput.connections objectAtIndex:0];
		if ([videoConnection isVideoOrientationSupported])
		{
			[videoConnection setVideoOrientation:UIDeviceOrientationPortrait];//[UIDevice currentDevice].orientation];
			if([UIDevice currentDevice].orientation == UIDeviceOrientationPortrait)
				NSLog(@"video orientation is set to Portrait");
		}
		[info->stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection
															completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
																if (imageDataSampleBuffer != NULL) {
																	NSData *imageData = [AVCaptureStillImageOutput
																						 jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
																	Base::MemStream *stream = new Base::MemStream([imageData length]);
																	stream->write([imageData bytes],[imageData length]);
																	Base::gSyscall->resources.add_RT_BINARY(placeholder, stream);
																}}];
		return 1;

	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraSnapshotAsync(int formatIndex))
{
	@try {
		CameraInfo *info = getCurrentCameraInfo();
        if (!info)
        {
            // Camera is not available.
            return MA_CAMERA_RES_FAILED;
        }

		AVCaptureConnection *videoConnection =	[info->stillImageOutput.connections objectAtIndex:0];
		if ([videoConnection isVideoOrientationSupported])
		{
			[videoConnection setVideoOrientation:UIDeviceOrientationPortrait];
		}

        int placeholder = maCreatePlaceholder();

        void (^captionCompletionHandler)(CMSampleBufferRef, NSError*) = ^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
            MAEvent event;
            event.type = EVENT_TYPE_CAMERA_SNAPSHOT;
            event.snapshotImageDataHandle = 0;
            event.snapshotFormatIndex = 0;
            event.snapshotImageDataRepresentation = MA_IMAGE_REPRESENTATION_JPEG;
            event.snapshotReturnCode = MA_CAMERA_RES_OK;

            if ( (NULL != imageDataSampleBuffer) && (NULL == error)) {
                NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
                Base::MemStream *stream = new Base::MemStream([imageData length]);

                stream->write([imageData bytes],[imageData length]);
                Base::gSyscall->resources.add_RT_BINARY(placeholder, stream);

                event.snapshotImageDataHandle = placeholder;
                event.snapshotFormatIndex = formatIndex;
            }
            else
            {
                event.snapshotImageDataRepresentation = MA_IMAGE_REPRESENTATION_UNKNOWN;
                event.snapshotReturnCode = MA_CAMERA_RES_FAILED;
            }
            Base::gEventQueue.put(event);
        };

		[info->stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection
                                                            completionHandler:captionCompletionHandler];
		return MA_CAMERA_RES_OK;
	}
	@catch (NSException * e) {
		return MA_CAMERA_RES_FAILED;
	}
}

SYSCALL(int, maCameraRecord(int stopStartFlag))
{
	return -1;
}

SYSCALL(int, maCameraSetProperty(const char *property, const char* value))
{
	@try {
		int result = 0;
		CameraInfo *info = getCurrentCameraInfo();
		if (!info)
		{
			// Camera is not available.
			return MA_CAMERA_RES_FAILED;
		}

		NSString *propertyString = [[NSString alloc] initWithUTF8String:property];
		NSString *valueString = [[NSString alloc] initWithUTF8String:value];
		result = [gCameraConfigurator setCameraProperty: info->device
										   withProperty: propertyString
											  withValue: valueString];
		[propertyString release];
		[valueString release];
		return result;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraGetProperty(const char *property, char *value, int maxSize)) //@property the property to get (string), @value will contain the value of the property when the func returns, @maxSize the size of the value buffer
{
	@try {
		NSString *propertyString = [[NSString alloc ] initWithUTF8String:property];
		CameraConfirgurator *configurator = [[CameraConfirgurator alloc] init];
		CameraInfo *info = getCurrentCameraInfo();
		if (!info)
		{
			// Camera is not available.
			return MA_CAMERA_RES_FAILED;
		}

		NSString* retval = [[configurator getCameraProperty:info->device withProperty:propertyString] retain];
		if(!retval)
		{
			return MA_CAMERA_RES_FAILED;
		}
		int length = maxSize;
		int realLength = [retval length];
		if(realLength > length) {
			return -2;
		}

		[retval getCString:value maxLength:length encoding:NSUTF8StringEncoding]; //stores the cstring value of retval in value
		[retval release];
		[propertyString release];
		[configurator release];

		return realLength;
	}
	@catch (NSException * e) {
		return -1;
	}
}

SYSCALL(int, maCameraPreviewSize()) //should really be named maCameraGetPreviewSize since it will be a getter
{
	@try {
		CameraInfo *info = getCurrentCameraInfo(); //get the info struct belonging to the currently active camera
		AVCaptureInput *input = [info->captureSession.inputs objectAtIndex:0];
		AVCaptureInputPort *port = [input.ports objectAtIndex:0];
		CMFormatDescriptionRef formatDescription = port.formatDescription;
		CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(formatDescription);
		int size = EXTENT((int) dimensions.width, (int) dimensions.height);
		return size;
	}
	@catch (NSException *exception) {
		return -1;
	}
}

SYSCALL(int, maCameraPreviewEventEnable(int previewEventType, void* previewBuffer, const MARect* previewArea))
{
	//2 events, MA_CAMERA_PREVIEW_FRAME and MA_CAMERA_PREVIEW_AUTO_FOCUS
	@try {

		if(!gCameraPreviewEventHandler) //no delegate, set one up for handling preview frames
		{
			CameraInfo *info = getCurrentCameraInfo();
			gCameraPreviewEventHandler = [[CameraPreviewEventHandler alloc] init];
			gCameraPreviewEventHandler->mPreviewArea = *previewArea;
			gCameraPreviewEventHandler->mPreviewBuf = previewBuffer;
			//set the gCameraPreviewEventHandler as delegate and set its dispatch queue as the queue for handling frames
			[info->videoDataOutput setSampleBufferDelegate:gCameraPreviewEventHandler queue:[gCameraPreviewEventHandler getQueue]];
			if ([info->captureSession canAddOutput:info->videoDataOutput]) {
				[info->captureSession addOutput:info->videoDataOutput]; //add the video data output to the capture session
			}
		}
		if(previewEventType == MA_CAMERA_PREVIEW_FRAME)
		{
			gCameraPreviewEventHandler->mEventStatus = previewEventType;
			gCameraPreviewEventHandler->mCaptureOutput = true; //start capturing output right away
			return 1;
		}
		else if(previewEventType == MA_CAMERA_PREVIEW_AUTO_FOCUS)
		{
			//set the gCameraPreviewEventHandler as an observer for the adjustingFocus
			//property of the current camera device
			CameraInfo *info = getCurrentCameraInfo();
			[info->device addObserver:gCameraPreviewEventHandler forKeyPath:@"adjustingFocus" options:NSKeyValueObservingOptionNew context:nil]; //nil is not recommended, but which context should be set?
			gCameraPreviewEventHandler->mEventStatus = previewEventType;
			gCameraPreviewEventHandler->mCaptureOutput = false; //don't capture until we got adjustingFocus event in the previewEventHandler
			return 1;
		}
		else //unsupported event type
		{
			return -1;
		}

	}
	@catch (NSException *exception) {
		return -1;
	}
}

SYSCALL(int, maCameraPreviewEventDisable()) //wouldn't it be neater to be able to disable a specific preview event?
{
	@try {
		if(gCameraPreviewEventHandler)
		{
			CameraInfo *info = getCurrentCameraInfo();
			[info->captureSession removeOutput:info->videoDataOutput];//stop listening for camera output
			[gCameraPreviewEventHandler release];
			gCameraPreviewEventHandler = NULL; //so that next maCameraPreviewEventEnable call can init it again
		}
	}
	@catch (NSException *exception) {
		return -1;
	}
	return 1;
}

SYSCALL(int, maCameraPreviewEventConsumed())
{
	if(gCameraPreviewEventHandler)
	{
		if(gCameraPreviewEventHandler->mEventStatus == MA_CAMERA_PREVIEW_FRAME)
		{
			gCameraPreviewEventHandler->mCaptureOutput = true; //frame event consumed, capture output again
			return 1;
		}
		if(gCameraPreviewEventHandler->mEventStatus == MA_CAMERA_PREVIEW_AUTO_FOCUS)
		{
			return 1;
		}
		return -1;
	}
	else
	{
		return -1;//no cameraPreviewEventHandler present.
	}
}
