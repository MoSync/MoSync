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
