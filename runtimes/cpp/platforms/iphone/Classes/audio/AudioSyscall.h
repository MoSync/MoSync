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

#ifndef _AUDIO_SYSCALL_H_
#define _AUDIO_SYSCALL_H_

#include <config_platform.h>
#include <helpers/cpp_defs.h>
//#include <helpers/CPP_IX_AUDIO.h>

void MAAudioInit();
void MAAudioClose();

MAAudioData maAudioDataCreateFromResource(const char* mime, MAHandle data, int offset, int length, int flags);
MAAudioData maAudioDataCreateFromURL(const char* mime, const char* url, int flags);
int maAudioDataDestroy(MAAudioData audioData);
MAAudioInstance maAudioInstanceCreate(MAAudioData audioData);
int maAudioInstanceDestroy(MAAudioInstance audioInstance);
int maAudioGetLength(MAAudioData audio);
int maAudioSetNumberOfLoops(MAAudioInstance audio, int loops);
int maAudioPlay(MAAudioInstance audio);
int maAudioPrepare(MAAudioInstance audio, int async);
int maAudioSetPosition(MAAudioInstance audio, int milliseconds);
int maAudioGetPosition(MAAudioInstance audio);
int maAudioSetVolume(MAAudioInstance audio, float volume);
int maAudioStop(MAAudioInstance audio);
int maAudioPause(MAAudioInstance audio);

MAAudioInstance maAudioInstanceCreateDynamic(int sampleRate, int numChannels, int bufferSize);
int maAudioGetPendingBufferCount(MAAudioInstance instance);
int maAudioSubmitBuffer(MAAudioInstance instance, const void* ptr, int numBytes);

#endif
