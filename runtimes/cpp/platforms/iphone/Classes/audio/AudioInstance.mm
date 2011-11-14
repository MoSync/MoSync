/* Copyright (C) 2011 Mobile Sorcery AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import "AudioInstance.h"
#import "AudioData.h"
#include <helpers/CPP_IX_AUDIO.h>

@implementation AudioInstance

/**
 * Init function.
 */
-(id) initWithAudioData:(AudioData*)audioData error:(int*)errorCode
{
	mAudioData = audioData;

	NSData* data = [mAudioData getData];
	NSError* error;

	if(errorCode)
		*errorCode = MA_AUDIO_ERR_OK;

	if(data != nil)
	{
		mAudioPlayer = [[AVAudioPlayer alloc] initWithData:data error:&error];
	} else {
		NSString* filename = [mAudioData getFilename];

		if(filename)
		{
			NSURL *fileURL = [[NSURL alloc] initFileURLWithPath:filename];
			mAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:&error];
			[fileURL release];
		} else {
			if(errorCode)
				*errorCode = MA_AUDIO_ERR_INVALID_DATA;
		}
	}

	[mAudioPlayer prepareToPlay];

	return [super init];
}

-(void) play
{
	[mAudioPlayer play];
}

-(void) stop
{
	[mAudioPlayer stop];
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

@end
