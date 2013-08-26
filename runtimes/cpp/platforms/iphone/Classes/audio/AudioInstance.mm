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

#import "AudioInstance.h"
#import "AudioData.h"
//#include <helpers/CPP_IX_AUDIO.h>
#include "Platform.h"

@implementation AudioInstanceStatic

/**
 * Init function.
 */
-(id) initWithAudioData:(AudioData*)audioData andHandle:(int)handle error:(int*)errorCode
{
	mAudioData = audioData;
	mHandle = handle;

	NSData* data = [mAudioData getData];
	NSError* error = nil;

	if(errorCode)
		*errorCode = MA_AUDIO_ERR_OK;

	if(data != nil)
	{
		mAudioPlayer = [[AVAudioPlayer alloc] initWithData:data error:&error];
	} else {
		NSString* filename = [mAudioData getFilename];

		if(filename)
		{
			NSURL* url = nil;
			if([filename hasPrefix:@"/"] || [filename hasPrefix:@"file://"])
			{
				url = [[NSURL alloc] initFileURLWithPath:filename];
			} else {
				url = [[NSURL alloc] initWithString:filename];
			}

			mAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];

			mAudioPlayer.delegate = self;

			if(error != nil)
			{
				*errorCode = MA_AUDIO_ERR_INVALID_INSTANCE;
			}

			[url release];
		} else {
			if(errorCode)
				*errorCode = MA_AUDIO_ERR_INVALID_DATA;
		}
	}

	mPrepared = NO;
	mIsPreparing = NO;

	return [super init];
}

-(BOOL) play
{
	mPrepared = true; // play implicitly calls prepareToPlay if it isn't already prepared
	return [mAudioPlayer play];
}

-(BOOL) isPrepared
{
	return mPrepared;
}

-(BOOL) isPreparing
{
	return mIsPreparing;
}

-(void) prepareThread
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	BOOL res = [mAudioPlayer prepareToPlay];

	mIsPreparing = false;

	MAEvent event;
	event.type = EVENT_TYPE_AUDIO_PREPARED;
	if(res == NO)
		event.audioInstance = MA_AUDIO_ERR_PREPARE_FAILED;
	else
		event.audioInstance = mHandle;
	Base::gEventQueue.put(event);

	[pool release];
}

-(BOOL) prepare:(BOOL)async
{
	if(async == NO)
	{
		mPrepared = true;
		return [mAudioPlayer prepareToPlay];
	} else {
		mIsPreparing = true;
		[NSThread detachNewThreadSelector:@selector(prepareThread) toTarget:self withObject:nil];
	}

	return true;
}


-(void) pause
{
	[mAudioPlayer pause];
}


-(void) stop
{
	mPrepared = false;
	[mAudioPlayer stop];
	[mAudioPlayer setCurrentTime:0.0];
}

-(void) setPosition:(int)millis
{
	[mAudioPlayer setCurrentTime:(double)millis*0.001];
}

-(int) getPosition
{
	return mAudioPlayer.currentTime*1000.0;
}

-(void) setNumberOfLoops:(int)numLoops
{
	[mAudioPlayer setNumberOfLoops:numLoops];
}

-(void) setVolume:(float)volume
{
	mAudioPlayer.volume = volume;
}

-(int) getLength
{
	return mAudioPlayer.duration*1000.0;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
	[super dealloc];
	if(mAudioPlayer)
		[mAudioPlayer release];
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
	MAEvent event;
	event.type = EVENT_TYPE_AUDIO_COMPLETED;

	if(flag == YES)
	{
		event.audioInstance = mHandle;
	} else {
		event.audioInstance = MA_AUDIO_ERR_GENERIC;
	}

	Base::gEventQueue.put(event);
}

@end
