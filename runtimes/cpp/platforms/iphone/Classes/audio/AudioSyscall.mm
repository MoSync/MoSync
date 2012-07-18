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

#include "config_platform.h"
#import "AudioSyscall.h"
#import "AudioInstance.h"
#import "AudioInstanceURL.h"
#import "AudioInstanceDynamic.h"
#import "AudioData.h"
#import "iphone_helpers.h"

//***************************************************************************
// Variables
//***************************************************************************

static NSMutableArray* sAudioData;
static NSMutableArray* sAudioInstances;

//***************************************************************************
// Initialization
//***************************************************************************

void MAAudioInit()
{
    //   256 / ( 44.1khz) ~  5.8ms
    float latency = 0.005; //in seconds gives a 256 sample buffer
    //on an infinitely fast computer, we'd make sure we got a buffer of size 1.
    // but it will take too much CPU. :-)  some latency is inevitable.
    OSStatus status = AudioSessionSetProperty(
											  kAudioSessionProperty_PreferredHardwareIOBufferDuration,
											  sizeof(latency),&latency
											  );

	sAudioData = [[NSMutableArray alloc] init];
	sAudioInstances = [[NSMutableArray alloc] init];
}

void MAAudioClose()
{
    [sAudioData release];
    [sAudioInstances release];
}

//***************************************************************************
// Ioctl implementations
//***************************************************************************

MAAudioData maAudioDataCreateFromResource(const char* mime, MAHandle data, int offset, int length, int flags)
{
    NSString* mimeType = @"";
    if(mime != NULL)
        mimeType = [[NSString alloc] initWithCString:mime encoding:NSASCIIStringEncoding];

    int error;
    AudioData* audioData = [[AudioData alloc] initWithMime:mimeType audio:data offset:offset length:length andFlags:flags error:&error];

    if(error != MA_AUDIO_ERR_OK)
        return error;

    [sAudioData addObject:audioData];
    return [sAudioData count] - 1;
}

MAAudioData maAudioDataCreateFromURL(const char* mime, const char* path, int flags)
{
    NSString* mimeType = @"";
    if(mime != NULL)
        mimeType = [[NSString alloc] initWithCString:mime encoding:NSASCIIStringEncoding];

    if(path == NULL) return MA_AUDIO_ERR_INVALID_FILENAME;
    NSString* filename = [[NSString alloc] initWithCString:path encoding:NSASCIIStringEncoding];

    int error;
    AudioData* audioData = [[AudioData alloc] initWithMime:mimeType filename:filename andFlags:flags error:&error];

    if(error != MA_AUDIO_ERR_OK)
        return error;

    [sAudioData addObject:audioData];
    return [sAudioData count] - 1;
}

int maAudioDataDestroy(MAAudioData audioData)
{
	AudioData* data = (AudioData*) [sAudioData objectAtIndex:audioData];
    if(!data)
        return MA_AUDIO_ERR_INVALID_DATA;

	[data release];
	[sAudioData replaceObjectAtIndex:audioData withObject:[NSNull null]];

    return MA_AUDIO_ERR_OK;
}

MAAudioInstance maAudioInstanceCreate(MAAudioData audioData)
{
    if(audioData < 0 || audioData >= [sAudioData count])
        return MA_AUDIO_ERR_INVALID_DATA;

    AudioData* data = (AudioData*) [sAudioData objectAtIndex:audioData];
    if(!data)
        return MA_AUDIO_ERR_INVALID_DATA;

    int error = MA_AUDIO_ERR_OK;

	MAAudioInstance handle = [sAudioInstances count];

    id<AudioInstance> audioInstance = nil;


	NSString* filename = [data getFilename];
	if(filename!=nil && ([filename hasPrefix:@"/"] || [filename hasPrefix:@"file://"]))
	{
		audioInstance = [[AudioInstanceStatic alloc] initWithAudioData:data andHandle:handle error:&error];
	}
	else
	{
		if(filename != nil)
		{
			audioInstance = [[AudioInstanceURL alloc] initWithAudioData:data andHandle:handle error:&error];
		}
		else
		{
			audioInstance = [[AudioInstanceStatic alloc] initWithAudioData:data andHandle:handle error:&error];
		}
	}

    if(error != MA_AUDIO_ERR_OK)
        return error;

    if(audioInstance == nil)
        return MA_AUDIO_ERR_GENERIC;

    [sAudioInstances addObject:audioInstance];

    return handle;
}

static id<AudioInstance> getAudioInstance(int audio)
{
    if(audio < 0 || audio >= [sAudioInstances count])
        return nil;

    return [sAudioInstances objectAtIndex:audio];
}

int maAudioInstanceDestroy(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance stop];
    [instance release];
    [sAudioInstances replaceObjectAtIndex:audio withObject:[NSNull null]];

    return MA_AUDIO_ERR_OK;
}

int maAudioGetLength(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    return [instance getLength];
}

int maAudioSetNumberOfLoops(MAAudioInstance audio, int numLoops)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setNumberOfLoops:numLoops];

    return MA_AUDIO_ERR_OK;
}

int maAudioPlay(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    if(![instance play])
		return MA_AUDIO_ERR_GENERIC;

    return MA_AUDIO_ERR_OK;
}

int maAudioPrepare(MAAudioInstance audio, int async)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;
	if([instance isPreparing] || [instance isPrepared])
		return MA_AUDIO_ERR_ALREADY_PREPARED;

	if(![instance prepare:(async==1)])
		return MA_AUDIO_ERR_PREPARE_FAILED;

	return MA_AUDIO_ERR_OK;
}

int maAudioSetPosition(MAAudioInstance audio, int milliseconds)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setPosition:milliseconds];

    return MA_AUDIO_ERR_OK;
}

int maAudioGetPosition(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    return [instance getPosition];
}

int maAudioSetVolume(MAAudioInstance audio, float volume)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setVolume:volume];

    return MA_AUDIO_ERR_OK;
}

int maAudioStop(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance stop];

    return MA_AUDIO_ERR_OK;
}

int maAudioPause(MAAudioInstance audio)
{
    id<AudioInstance> instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance pause];

    return MA_AUDIO_ERR_OK;
}

MAAudioInstance maAudioInstanceCreateDynamic(int sampleRate, int numChannels, int bufferSize)
{
    int error = MA_AUDIO_ERR_OK;

	MAAudioInstance handle = [sAudioInstances count];

    AudioInstanceDynamic* audioInstance = [[AudioInstanceDynamic alloc] initWithSampleRate:sampleRate numChannels:numChannels andBufferSize:bufferSize error:&error];

    if(error != MA_AUDIO_ERR_OK)
        return error;

    if(audioInstance == nil)
        return MA_AUDIO_ERR_GENERIC;

    [sAudioInstances addObject:audioInstance];

    return handle;
}

int maAudioGetPendingBufferCount(MAAudioInstance instance)
{
    id<AudioInstance> ainstance = getAudioInstance(instance);
    if(!ainstance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;
	if([ainstance class] != [AudioInstanceDynamic class])
		return MA_AUDIO_ERR_INVALID_INSTANCE;
	AudioInstanceDynamic* aid = (AudioInstanceDynamic*)ainstance;

	return [aid getPendingBufferCount];
}

int maAudioSubmitBuffer(MAAudioInstance instance, const void* ptr, int numBytes)
{
    id<AudioInstance> ainstance = getAudioInstance(instance);
    if(!ainstance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;
	if([ainstance class] != [AudioInstanceDynamic class])
        return MA_AUDIO_ERR_INVALID_INSTANCE;
	AudioInstanceDynamic* aid = (AudioInstanceDynamic*)ainstance;

	return [aid submitBufferData:ptr ofNumBytes:numBytes];
}
