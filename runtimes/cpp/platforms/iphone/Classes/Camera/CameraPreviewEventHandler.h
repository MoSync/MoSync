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

#import <Foundation/Foundation.h>

#include <helpers/cpp_defs.h>

//This delegate is needed for capturing video frame data from the camera, when using the maCameraPreview* syscalls
//It is also used as an observer for the "adjustingFocus"-property of the camera.
@interface CameraPreviewEventHandler:NSObject<AVCaptureVideoDataOutputSampleBufferDelegate> {
@public
    void* mPreviewBuf; //buffer for storing the sub section of the frame
    MARect mPreviewArea; //the sub area captured from the frame
    int mEventStatus; //MA_CAMERA_PREVIEW_FRAME/AUTO_FOCUS
    bool mCaptureOutput; //only want to capture one frame and then wait for maCameraPreviewEventConsumed is called, this bool is used for that
    unsigned int mCaptureOnFocus; //flag for capturing frame on autofocus, which should only be done every second time, because on the first event,
    //the camera hasn't focus yet
@private
    dispatch_queue_t mSerialQueue;
}
- (id)init;
- (void)dealloc;
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection;
- (void)image2Buf:(CMSampleBufferRef)sampleBuf;
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context;
- (dispatch_queue_t)getQueue;
- (void*)previewBuf;
- (void)setPreviewBuf:(void*)previewBuf;
- (MARect)previewArea;
- (void)setPreviewArea:(MARect)previewArea;
@property void* previewBuf;
@property MARect previewArea;

@end
