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

#import <AVFoundation/AVFoundation.h>

#import "CameraPreviewEventHandler.h"
#import "Platform.h"

//#include <helpers/cpp_defs.h>

@implementation CameraPreviewEventHandler
- (id)init{
    if((self = [super init]))
    {
        mCaptureOnFocus = 0;
        mEventStatus = -1; //neither FRAME nor AUTO_FOCUS
        mCaptureOutput = false;
        mSerialQueue = dispatch_queue_create("com.mosync.cameraPreviewQueue", NULL); //need a serial queue to get preview frames in order
    }
    return self;
}
- (void)dealloc{
    dispatch_release(mSerialQueue); //release dispatch queue
    //no [super dealloc]; on delegates
}
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if(mCaptureOutput){//only capture one frame at a time
        [self image2Buf:sampleBuffer];//copy the image data within previewArea from sampleBuffer to mPreviewBuf
        MAEvent event;
        event.type = EVENT_TYPE_CAMERA_PREVIEW;
        event.status = mEventStatus;
        Base::gEventQueue.put(event);
        mCaptureOutput = false;//stop capturing output until maCameraPreviewEventConsumed is called
    }
}
- (void)image2Buf:(CMSampleBufferRef)sampleBuf{//process and store image data in mPreviewBuf, mosync wants the image data in RGB888 format, but here we get it in BGRA8888
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuf);
    CVPixelBufferLockBaseAddress(imageBuffer,0);//the zero is just a flag, not an offset
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t bytesPerPixel = bytesPerRow/width;
    int rowOffset = mPreviewArea.top;
    int colOffset = mPreviewArea.left;
    void *srcBuf = CVPixelBufferGetBaseAddress(imageBuffer);
    //fill the buffer with the image data inside the rect specified by mPreviewArea, need to fill the buffer with RGBA8888, so have to convert BGRA8888->RGBA8888
    for(size_t i = rowOffset; i < rowOffset+mPreviewArea.height; ++i){ //copy row by row
        int bufOffset = (i*bytesPerRow) + (colOffset*bytesPerPixel);
        memcpy((char*)mPreviewBuf+((i-rowOffset)*bytesPerPixel*mPreviewArea.width), (char*)srcBuf+bufOffset, mPreviewArea.width*bytesPerPixel); //move 3 steps in dest buf and 4 steps in source buf (RBGA->RGB)
    }
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
}
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if(mEventStatus != MA_CAMERA_PREVIEW_AUTO_FOCUS){
        return;//don't capture auto focus events if they're not enabled
    }
    if ([keyPath isEqualToString:@"adjustingFocus"] )
    {
        if(mCaptureOnFocus % 2) {//only capture a frame on the 2nd of two AF events
            mCaptureOutput = true;
        }
        ++mCaptureOnFocus;
    }
}
- (dispatch_queue_t)getQueue{
    return mSerialQueue;
}
- (void*)previewBuf{
    return mPreviewBuf;
}
- (void)setPreviewBuf:(void*)previewBuf{
    mPreviewBuf = previewBuf;
}
- (MARect)previewArea{
    return mPreviewArea;
}
- (void)setPreviewArea:(MARect)previewArea{
    mPreviewArea = previewArea;
}

@end
