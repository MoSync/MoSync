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

#import "AudioInstanceURL.h"
#import "AudioData.h"
//#include <helpers/CPP_IX_AUDIO.h>
#include "Platform.h"

@implementation AudioInstanceURL

/**
 * Init function.
 */
-(id) initWithAudioData:(AudioData*)audioData andHandle:(int)handle error:(int*)errorCode
{
	mAudioData = audioData;
	mHandle = handle;

	if(errorCode)
		*errorCode = MA_AUDIO_ERR_OK;

	NSString* filename = [mAudioData getFilename];
	if(errorCode)
	{
		if(filename == nil)
			return nil;
	}

	NSURL* url = [[NSURL alloc] initWithString:filename];
	mPlayer = [[AVPlayer alloc] initWithURL:url];
	[url release];

	mPlayer.actionAtItemEnd = AVPlayerActionAtItemEndNone;

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(playerItemDidReachEnd:)
												 name:AVPlayerItemDidPlayToEndTimeNotification
											   object:[mPlayer currentItem]];
	mPrepared = NO;
	mIsPreparing = NO;
	mNumLoops = 0;

	return [super init];
}

-(BOOL) play
{
	mPrepared = true; // play implicitly calls prepareToPlay if it isn't already prepared
	mCurrentLoop = 0;
	[mPlayer play];
	mCurrentLoop = 0;
	return true;
}

-(BOOL) isPrepared
{
	return mPrepared;
}

-(BOOL) isPreparing
{
	return mIsPreparing;
}

-(BOOL) prepare:(BOOL)async
{

	mIsPreparing = false;

	MAEvent event;
	event.type = EVENT_TYPE_AUDIO_PREPARED;
	event.audioInstance = mHandle;
	Base::gEventQueue.put(event);

	mPrepared = true;

	return true;
}


-(void) pause
{
	[mPlayer pause];
}


-(void) stop
{
	mPrepared = false;
	[mPlayer pause];
	[self setPosition:0];
}

-(void) setPosition:(int)millis
{
	CMTime newTime = CMTimeMakeWithSeconds((double)millis*0.001, 1);
	[mPlayer seekToTime:newTime];
}

-(int) getPosition
{
	return CMTimeGetSeconds(mPlayer.currentTime)*1000.0;
}

-(void) setNumberOfLoops:(int)numLoops
{
	//[mAudioPlayer setNumberOfLoops:numLoops];
	mNumLoops = numLoops;
}

-(void) setVolume:(float)volume
{
	id audioMix = [[AVAudioMix alloc] init];
	id volumeMixInput = [[AVMutableAudioMixInputParameters alloc] init];
	[volumeMixInput setVolume:volume atTime:kCMTimeZero];
	[volumeMixInput setTrackID:[[mPlayer.currentItem.asset.tracks objectAtIndex:0] trackID]];
	[audioMix setInputParameters:[NSArray arrayWithObject:volumeMixInput]];
	[mPlayer.currentItem setAudioMix:audioMix];
}

-(int) getLength
{
	return CMTimeGetSeconds(mPlayer.currentItem.duration)*1000.0;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
	[super dealloc];
	if(mPlayer)
		[mPlayer release];
}

- (void)playerItemDidReachEnd:(NSNotification *)notification {
	if(mNumLoops >= 0 && mCurrentLoop >= mNumLoops)
	{
		[mPlayer seekToTime:kCMTimeZero];
		[mPlayer pause];
	} else {
		[mPlayer seekToTime:kCMTimeZero];
		[mPlayer play];
		mCurrentLoop++;
	}

	MAEvent event;
	event.type = EVENT_TYPE_AUDIO_COMPLETED;
	event.audioInstance = mHandle;
	Base::gEventQueue.put(event);
}

@end
