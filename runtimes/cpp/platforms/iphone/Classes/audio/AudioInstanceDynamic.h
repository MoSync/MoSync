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
