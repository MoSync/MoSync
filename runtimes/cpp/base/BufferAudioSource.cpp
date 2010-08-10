/* Copyright (C) 2009 Mobile Sorcery AB

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

#include "BufferAudioSource.h"
#include "config_platform.h"
#include <helpers/helpers.h>
#include "Syscall.h"
#include <helpers/CPP_IX_AUDIOBUFFER.h>
#include "ThreadPool.h"

using namespace Base;

BufferAudioSource::BufferAudioSource(MAAudioBufferInfo *i, BufferRequestCallback callback) {
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
					info.bytesPerSample = 2;
					info.bitDepth = 8;
					break;
	}
	info.numChannels = i->numChannels;
	info.bufferSize = i->bufferSize;
	mBuffer = SYSCALL_THIS->GetValidatedMemRange((int)i->buffer, info.bufferSize);
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
