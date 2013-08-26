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
