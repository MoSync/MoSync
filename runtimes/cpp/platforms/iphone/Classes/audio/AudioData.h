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
#import <helpers/cpp_defs.h>

@interface AudioData :  NSObject {
    NSString* mMimeType;
    MAHandle mAudioData;
    int mAudioDataOffset;
    int mAudioDataLength;
    int mFlags;
    NSData* mData;
    NSString* mFilename;
}

/**
 * Init function.
 */
-(id) initWithMime:(NSString*)mime audio:(MAHandle)audioData offset:(int)offset length:(int)length andFlags:(int)flags error:(int*)error;
-(id) initWithMime:(NSString*)mime filename:(NSString*)filename andFlags:(int)flags error:(int*)error;

-(NSData*) getData;
-(NSString*) getFilename;

@end
