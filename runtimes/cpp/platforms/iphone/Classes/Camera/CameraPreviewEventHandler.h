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
