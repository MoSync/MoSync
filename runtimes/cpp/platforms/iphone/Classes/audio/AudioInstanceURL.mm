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
