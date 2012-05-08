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

#import "AudioInstance.h"

@interface AudioInstanceURL :  NSObject <AudioInstance> {
	AVPlayer* mPlayer;
    AudioData* mAudioData;
	int mHandle;
	BOOL mPrepared;
	BOOL mIsPreparing;
	int mNumLoops;
	int mCurrentLoop;
}

-(id) initWithAudioData:(AudioData*)audioData andHandle:(int)handle error:(int*)error;

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
- (void) dealloc;
@end
