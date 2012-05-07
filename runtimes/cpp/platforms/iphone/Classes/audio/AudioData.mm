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

#include <config_platform.h>
#import "AudioData.h"
#include "Stream.h"
#include "Syscall.h"
//#include <helpers/CPP_IX_AUDIO.h>

using namespace Base;

@implementation AudioData

/**
 * Init function.
 */
-(id) initWithMime:(NSString*)mime audio:(MAHandle)audioData offset:(int)offset length:(int)length andFlags:(int)flags error:(int*)error;
{
	id ret = [super init];
	if(ret)
	{
		mMimeType = mime;
		mAudioData = audioData;
		mFilename = nil;
		mAudioDataOffset = offset;
		mAudioDataLength = length;
		mFlags = flags;

		Stream *res = gSyscall->resources.get_RT_BINARY(audioData);
		MYASSERT(res->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
		Stream* src = res->createLimitedCopy(length);
		MYASSERT(src, ERR_DATA_ACCESS_FAILED);

		mData = NULL;

		int encodedSize = length;
		if(!src->ptrc()) {
			byte* sound = new byte[encodedSize];
			src->read(sound, encodedSize);
			mData = [[NSData alloc] initWithBytesNoCopy:sound length:encodedSize freeWhenDone:YES];
		} else {
			byte* sound = &(((byte*)src->ptrc())[offset]);
			mData = [[NSData alloc] initWithBytesNoCopy:sound length:encodedSize freeWhenDone:NO];
		}
	}

	if(!ret || !mData) {
		if(error)
			*error = MA_AUDIO_ERR_OUT_OF_MEMORY;
	} else {
		if(error)
			*error = MA_AUDIO_ERR_OK;
	}

	return ret;
}

-(id) initWithMime:(NSString*)mime filename:(NSString*)filename andFlags:(int)flags error:(int*)error
{
	id ret = [super init];
	if(ret)
	{
		mMimeType = mime;

		if([filename hasPrefix:@"/"]==NO && [filename hasPrefix:@"file://"]==NO && !(flags & MA_AUDIO_DATA_STREAM))
		{
			NSURL* url = [[NSURL alloc] initWithString:filename];
			NSData *data =[[NSData alloc] initWithContentsOfURL:url];
			mData = data;
			[url release];
			mFlags = flags;
			mAudioData = nil;
			mFilename = nil;
			if(error)
				*error = MA_AUDIO_ERR_OK;
			return ret;
		}

		mFilename = filename;
		mFlags = flags;
		mAudioData = nil;
	}

	if(!filename) {
		if(error)
			*error = MA_AUDIO_ERR_INVALID_FILENAME;
	}
	else if(!ret) {
		if(error)
			*error = MA_AUDIO_ERR_OUT_OF_MEMORY;
	} else {
		if(error)
			*error = MA_AUDIO_ERR_OK;
	}

	return ret;
}

-(NSData*) getData
{
	return mData;
}

-(NSString*) getFilename
{
	return mFilename;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
	[super dealloc];
	if(mData)
		[mData release];
	if(mFilename)
		[mFilename release];
	if(mMimeType)
		[mMimeType release];
}

@end
