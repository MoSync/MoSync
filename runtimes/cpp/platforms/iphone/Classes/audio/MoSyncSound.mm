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
