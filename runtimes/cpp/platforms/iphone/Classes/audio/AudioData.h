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
