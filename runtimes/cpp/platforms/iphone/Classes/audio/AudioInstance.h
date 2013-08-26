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
#import <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

@class AudioData;

@protocol AudioInstance <NSObject>
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
	-(void) release;
@end

@interface AudioInstanceStatic :  NSObject <AVAudioPlayerDelegate, AudioInstance> {
    AVAudioPlayer* mAudioPlayer;
    AudioData* mAudioData;
	int mHandle;
	BOOL mPrepared;
	BOOL mIsPreparing;
}

/**
 * Init function.
 */
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
