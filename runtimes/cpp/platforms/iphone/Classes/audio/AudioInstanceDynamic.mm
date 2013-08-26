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

#import "AudioInstanceDynamic.h"
#import <helpers/cpp_defs.h>
#include "Platform.h"

static void playCallback(
						 void* inUserData, AudioQueueRef inAudioQueue, AudioQueueBufferRef inBuffer)
{
	AudioInstanceDynamic* i = (AudioInstanceDynamic*) inUserData;

	// notify the system that this buffer is available again.
	[i tagBufferAsAvailable:inBuffer];
}

@implementation AudioInstanceDynamic

-(id) initWithSampleRate:(int)sampleRate numChannels:(int)numChannels andBufferSize:(int)bufferSize error:(int*)error
{
	id ret;

	if( (ret = [self init]) )
	{
		mDataFormat.mSampleRate = (float)sampleRate;
		mDataFormat.mFormatID = kAudioFormatLinearPCM;
		mDataFormat.mFramesPerPacket = 1;
		mDataFormat.mChannelsPerFrame = numChannels;
		mDataFormat.mBytesPerFrame = numChannels*sizeof(short);
		mDataFormat.mBytesPerPacket = numChannels*sizeof(short);
		mDataFormat.mBitsPerChannel = 16;
		mDataFormat.mReserved = 0;
		mDataFormat.mFormatFlags =	kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked | kAudioFormatFlagsNativeEndian;

		mBufferSize = bufferSize * numChannels * sizeof(short);

		AudioQueueNewOutput(
											&mDataFormat,
											playCallback,
											self,
											NULL,                   // run loop
											kCFRunLoopCommonModes,  // run loop mode
											0,                      // flags
											&mQueue
											);

		mBuffers = [[NSMutableArray alloc] init];
		mAvailableBuffers = [[NSMutableArray alloc] init];
	}

	mPendingBuffers = 0;

	[self setVolume:1.0];

	return ret;
}

-(BOOL) isPrepared
{
	return NO;
}

-(BOOL) isPreparing
{
	return NO;
}

-(BOOL) prepare:(BOOL)async
{
	return NO;
}

-(BOOL) play
{
	AudioQueueStart(mQueue, NULL);
	return YES;
}

-(void) pause
{
	AudioQueuePause(mQueue);
}

-(void) stop
{
	AudioQueueStop(mQueue, NULL);
}

-(void) setPosition:(int)millis
{
}

-(int) getPosition
{
	return 0;
}

-(void) setNumberOfLoops:(int)numLoops
{
}

-(void) setVolume:(float)volume
{
	AudioQueueSetParameter(
						   mQueue,
						   kAudioQueueParam_Volume,
						   volume
						   );
}

-(int) getLength
{
	return 0;
}


// internal
-(void) tagBufferAsAvailable:(AudioQueueBufferRef)buf
{
	@synchronized(mAvailableBuffers)
	{
		[mAvailableBuffers addObject:[NSValue valueWithPointer:buf]];
		mPendingBuffers--;
	}
}

-(AudioQueueBufferRef) createBuffer: (int) numBytes
{
	AudioQueueBufferRef buffer;
	AudioQueueAllocateBuffer(mQueue, numBytes, &buffer);
	[mBuffers addObject:[NSValue valueWithPointer:buffer]];
	[mAvailableBuffers addObject:[NSValue valueWithPointer:buffer]];
	return buffer;
}

-(int) submitBufferData:(const void*)data ofNumBytes:(int) numBytes
{
	@synchronized(mAvailableBuffers)
	{
		AudioQueueBufferRef buffer = nil;

		for(int i = 0; i < [mAvailableBuffers count]; i++)
		{
			NSValue* bufValue = [mAvailableBuffers objectAtIndex:0];
			AudioQueueBufferRef buf = (AudioQueueBufferRef) [bufValue pointerValue];
			if(buf->mAudioDataBytesCapacity >= numBytes)
			{
				buffer = buf;
				[mAvailableBuffers removeObjectAtIndex:i];
				break;
			}
		}

		if(buffer == nil)
		{
			buffer = [self createBuffer: numBytes];
			[mAvailableBuffers removeObjectAtIndex:0];
		}

		mPendingBuffers++;
		buffer->mAudioDataByteSize = numBytes;
		memcpy(buffer->mAudioData, data, numBytes);
		AudioQueueEnqueueBuffer(mQueue, buffer, 0, NULL);
	}

	return MA_AUDIO_ERR_OK;
}

-(int) getPendingBufferCount
{
	return mPendingBuffers;
}


/**
 * Release all the objects.
 */
- (void) dealloc
{
	[super dealloc];

	@synchronized(mAvailableBuffers)
	{
		for(int i = 0; i < [mBuffers count]; i++)
		{
			NSValue* bufValue = [mAvailableBuffers objectAtIndex:0];
			AudioQueueBufferRef buf = (AudioQueueBufferRef) [bufValue pointerValue];
			AudioQueueFreeBuffer(mQueue, buf);
		}
	}

	if(mQueue)
		AudioQueueDispose(mQueue, TRUE);

	[mAvailableBuffers release];
	[mBuffers release];
}


@end
