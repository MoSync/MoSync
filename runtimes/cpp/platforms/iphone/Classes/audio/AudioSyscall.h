/* Copyright (C) 2010 Mobile Sorcery AB

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

#ifndef _AUDIO_SYSCALL_H_
#define _AUDIO_SYSCALL_H_

#include <config_platform.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_AUDIO.h>

void MAAudioInit();
void MAAudioClose();

MAAudioData maAudioDataCreateFromResource(const char* mime, MAHandle data, int offset, int length, int flags);
MAAudioData maAudioDataCreateFromFile(const char* mime, const char* filename, int flags);

int maAudioDataDestroy(MAAudioData audioData);
MAAudioInstance maAudioInstanceCreate(MAAudioData audioData);
int maAudioInstanceDestroy(MAAudioInstance audioInstance);

int maAudioGetLength(MAAudioData audio);
int maAudioSetNumberOfLoops(MAAudioInstance audio, int loops);
int maAudioPlay(MAAudioInstance audio);
int maAudioSetPosition(MAAudioInstance audio, int milliseconds);
int maAudioGetPosition(MAAudioInstance audio);
int maAudioSetVolume(MAAudioInstance audio, float volume);
int maAudioStop(MAAudioInstance audio);

#endif
