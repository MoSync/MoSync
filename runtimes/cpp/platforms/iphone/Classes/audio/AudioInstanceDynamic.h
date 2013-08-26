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
#import <AudioToolbox/AudioToolbox.h>

#define NUM_AUDIO_QUEUE_BUFFERS 10
@interface AudioInstanceDynamic : NSObject
{
    AudioStreamBasicDescription mDataFormat;
    AudioQueueRef mQueue;
	NSMutableArray* mBuffers;
	NSMutableArray* mAvailableBuffers;
	int mPendingBuffers;
	int mBufferSize;
}


/**
 * Init function.
 */
-(id) initWithSampleRate:(int)sampleRate numChannels:(int)numChannels andBufferSize:(int)bufferSize error:(int*)error;

-(BOOL) isPrepared;
-(BOOL) isPreparing;
-(BOOL) prepare:(BOOL)async;
-(BOOL) play;
-(void) pause;
-(void) stop;
-(void) setPosition:(int)millis;
-(int) getPosition;
-(void) setNumberOfLoops:(int)numLoops;
-(void) setVolume:(float)volume;
-(int) getLength;

-(int) submitBufferData:(const void*)data ofNumBytes:(int) numBytes;
-(int) getPendingBufferCount;

-(void) tagBufferAsAvailable:(AudioQueueBufferRef)buf;
-(AudioQueueBufferRef) createBuffer: (int) numBytes;

- (void) dealloc;

@end
