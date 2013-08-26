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

#include "BufferAudioSource.h"
#include "config_platform.h"
#include <helpers/helpers.h>
#include "Syscall.h"
#include <helpers/CPP_IX_AUDIOBUFFER.h>
#include "ThreadPool.h"

using namespace Base;

BufferAudioSource::BufferAudioSource(const MAAudioBufferInfo *i, BufferRequestCallback callback) {
	mSem = new MoSyncSemaphore();
	info.sampleRate = i->sampleRate;
	switch(i->fmt) {
				case AUDIO_FMT_S16: 
					info.fmt = AudioSource::FMT_S16;
					info.bytesPerSample = 2;
					info.bitDepth = 16;
					break;
				case AUDIO_FMT_U16: 
					info.fmt = AudioSource::FMT_U16;
					info.bytesPerSample = 2;
					info.bitDepth = 16;
					break;
				case AUDIO_FMT_S8: 
					info.fmt = AudioSource::FMT_S8;
					info.bytesPerSample = 1;
					info.bitDepth = 8;
					break;
				case AUDIO_FMT_U8: 
					info.fmt = AudioSource::FMT_U8;
					info.bytesPerSample = 1;
					info.bitDepth = 8;
					break;
	}
	info.numChannels = i->numChannels;
	info.bufferSize = i->bufferSize;
	mBuffer = SYSCALL_THIS->GetValidatedMemRange((int)(size_t)i->buffer, info.bufferSize);
	mCallback = callback;
}

BufferAudioSource::~BufferAudioSource() {
	delete mSem;
}

int BufferAudioSource::init() {
	return 0;
}

void BufferAudioSource::close() {
	ready();
}

void BufferAudioSource::ready() {
	mSem->post();
}

int BufferAudioSource::fillBuffer() {
	mCallback();
	/*
	MAEvent audioEvent;
	audioEvent.type = EVENT_TYPE_AUDIOBUFFER_FILL;
	gEventFifo.put(audioEvent);
	*/
	mSem->wait();
	return info.bufferSize/(info.bytesPerSample*info.numChannels);
}

const void* BufferAudioSource::getBuffer() const {
	return mBuffer;
}

void BufferAudioSource::setPosition(int ms) {
}

int BufferAudioSource::getPosition() const {
	return 0;
}

int BufferAudioSource::getLength() const {
	return 0;
}

void BufferAudioSource::setNumLoops(int i) {
}

int BufferAudioSource::getNumLoops() {
	return 0;
}
