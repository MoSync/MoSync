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

#include "config_platform.h"
#import "AudioSyscall.h"
#import "AudioInstance.h"
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

MAAudioData maAudioDataCreateFromFile(const char* mime, const char* path, int flags)
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
    return MA_AUDIO_ERR_OK;
}

MAAudioInstance maAudioInstanceCreate(MAAudioData audioData)
{
    if(audioData < 0 || audioData >= [sAudioData count])
        return MA_AUDIO_ERR_INVALID_DATA;

    AudioData* data = (AudioData*) [sAudioData objectAtIndex:audioData];
    if(!data)
        return MA_AUDIO_ERR_INVALID_DATA;

    int error;
    AudioInstance* audioInstance = [[AudioInstance alloc] initWithAudioData:data error:&error];

    if(error != MA_AUDIO_ERR_OK)
        return error;

    if(audioInstance == nil)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [sAudioInstances addObject:audioInstance];

    return [sAudioInstances count] - 1;
}

static AudioInstance* getAudioInstance(int audio)
{
    if(audio < 0 || audio >= [sAudioInstances count])
        return nil;

    return [sAudioInstances objectAtIndex:audio];
}

int maAudioInstanceDestroy(MAAudioInstance audio)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance stop];
    [instance release];

    return MA_AUDIO_ERR_OK;
}

int maAudioGetLength(MAAudioInstance audio)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    return [instance getLength];
}

int maAudioSetNumberOfLoops(MAAudioInstance audio, int numLoops)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setNumberOfLoops:numLoops];

    return MA_AUDIO_ERR_OK;
}

int maAudioPlay(MAAudioInstance audio)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance play];

    return MA_AUDIO_ERR_OK;
}

int maAudioSetPosition(MAAudioInstance audio, int milliseconds)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setPosition:milliseconds];

    return MA_AUDIO_ERR_OK;
}

int maAudioGetPosition(MAAudioInstance audio)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    return [instance getPosition];
}

int maAudioSetVolume(MAAudioInstance audio, float volume)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance setVolume:volume];

    return MA_AUDIO_ERR_OK;
}

int maAudioStop(MAAudioInstance audio)
{
    AudioInstance* instance = getAudioInstance(audio);
    if(!instance)
        return MA_AUDIO_ERR_INVALID_INSTANCE;

    [instance stop];

    return MA_AUDIO_ERR_OK;
}
