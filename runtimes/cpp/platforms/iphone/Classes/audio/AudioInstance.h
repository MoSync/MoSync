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

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

@class AudioData;

@interface AudioInstance :  NSObject {
    AVAudioPlayer* mAudioPlayer;
    AudioData* mAudioData;
}

/**
 * Init function.
 */
-(id) initWithAudioData:(AudioData*)audioData error:(int*)error;

-(void) play;
-(void) stop;
-(void) setPosition:(int)millis;
-(int) getPosition;
-(void) setNumberOfLoops:(int)numLoops;
-(void) setVolume:(float)volume;
-(int) getLength;

@end
