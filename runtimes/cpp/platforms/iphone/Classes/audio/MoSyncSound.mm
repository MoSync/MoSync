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

#import "MoSyncSound.h"

#import <AVFoundation/AVFoundation.h>

static AVAudioPlayer* sSoundPlayer = NULL;

SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size))
{
	Base::Stream *res = Base::gSyscall->resources.get_RT_BINARY(sound_res);
	MYASSERT(res->seek(Base::Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
	Base::Stream* src = res->createLimitedCopy(size);
	MYASSERT(src, ERR_DATA_ACCESS_FAILED);

	byte b;
	do {
		if(!src->readByte(b))
			BIG_PHAT_ERROR(ERR_MIME_READ_FAILED);
	} while(b);

	int pos;
	src->tell(pos);

	NSData *data = NULL;

	int encodedSize = size - pos + offset;
	if(!src->ptrc()) {
		byte* sound = new byte[encodedSize];
		src->read(sound, encodedSize);
		data = [NSData dataWithBytesNoCopy:sound length:encodedSize];

	} else {
		byte* sound = &(((byte*)src->ptrc())[pos]);
		data = [NSData dataWithBytesNoCopy:sound length:encodedSize freeWhenDone:NO];
	}

	if(sSoundPlayer) {
		if(sSoundPlayer.playing==YES)
			[sSoundPlayer stop];
		[sSoundPlayer release];
	}

	NSError *err;
	sSoundPlayer = [[AVAudioPlayer alloc] initWithData:data error:&err];
	[data release];
	if(!sSoundPlayer) {
		// check err
		return -1;
	}
	[sSoundPlayer prepareToPlay];
	[sSoundPlayer play];

	return 0;
}

SYSCALL(void, maSoundStop())
{
	[sSoundPlayer stop];
	sSoundPlayer.currentTime = 0;
}

SYSCALL(int, maSoundIsPlaying())
{
	return sSoundPlayer.playing==YES;
}

SYSCALL(void, maSoundSetVolume(int vol))
{
	sSoundPlayer.volume = (float)vol/100.0f;
}

SYSCALL(int, maSoundGetVolume())
{
	return (int)(sSoundPlayer.volume*100.0f);
}
