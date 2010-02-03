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

#include "config_platform.h"
#include "SDLSoundAudioSource.h"
#include "helpers/helpers.h"

#define DEFAULT_DECODEBUF_BYTES 16384
#define DEFAULT_AUDIOBUF_SAMPLES 1024*4

static void skipID3(SDL_RWops *rwops);

SDLSoundAudioSource::SDLSoundAudioSource(SDL_RWops *rwops) :
mNumLoops(1), mRWops(rwops)
{
}

int SDLSoundAudioSource::init() {
	//dump info
	const Sound_DecoderInfo** i;
	for (i = Sound_AvailableDecoders(); *i != NULL; i++) {
		//const Sound_DecoderInfo* di = *i;
		LOG("Supported sound format: [%s], which is [%s].\n",
			(*i)->extensions[0], (*i)->description);
	}

	skipID3(mRWops);

	mAudioInfo.rate=44100;
	mAudioInfo.format=AUDIO_S16;
	mAudioInfo.channels=2;
	mSample = Sound_NewSample(mRWops, NULL, NULL, DEFAULT_DECODEBUF_BYTES);
	if(!mSample) {
		LOG("Sound_GetError() = %s\n", Sound_GetError());
		return -1;
	}
	mAudioInfo = mSample->actual;

	info.sampleRate = mAudioInfo.rate;
	
 	if(mAudioInfo.format == AUDIO_U16) {
		info.fmt = AudioSource::FMT_U16;
		info.bitDepth = 16;
	} else if(mAudioInfo.format == AUDIO_S16) {
		info.fmt = AudioSource::FMT_S16;
		info.bitDepth = 16;
	} else if(mAudioInfo.format == AUDIO_U8) {
		info.fmt = AudioSource::FMT_U8;
		info.bitDepth = 8;
	}  else if(mAudioInfo.format == AUDIO_S8) {
		info.fmt = AudioSource::FMT_S8;
		info.bitDepth = 8;
	}

	info.numChannels = mAudioInfo.channels;
	info.bufferSize = DEFAULT_DECODEBUF_BYTES;
	info.canSeek = false;

	return 0;
}

SDLSoundAudioSource::~SDLSoundAudioSource() {
	Sound_FreeSample(mSample);
}


int SDLSoundAudioSource::fillBuffer()
{
	if(mSample) {	//SDL_sound
		MutexHandler mutex(&mMutex);
		Sound_Sample *sample = mSample;

		// don't need to decode more data just yet
		/*
		if (mDecodedSamples  > 0)   
			return mDecodedSamples;
		*/

		// there's more
		if (!(sample->flags & (SOUND_SAMPLEFLAG_ERROR | SOUND_SAMPLEFLAG_EOF)))
		{
			int decodedSamples = Sound_Decode(sample) / ((info.bitDepth>>3)*info.numChannels);
			//mDecodedPtr = (Sint16*)sample->buffer;	
			return decodedSamples;  
		}

		if (mNumLoops==1) {
			return 0;
		} else if(mNumLoops>1) { mNumLoops--;}

		if(!Sound_Rewind(sample))
		{
			//LOG("%s\n", Sound_GetError());
			//BIG_PHAT_ERROR(SDLERR_SOUND_LOOP_FAILED);
			// rewind failed.
		}
	}

	return 0;
}

const void* SDLSoundAudioSource::getBuffer() const {
	return mSample->buffer;
}

void SDLSoundAudioSource::setPosition(int ms) {
	MutexHandler mutex(&mMutex);
	Sound_Seek(mSample, ms);
}

int SDLSoundAudioSource::getPosition() const {
	return 0;
}

int SDLSoundAudioSource::getLength() const {
	return 0;
}

void SDLSoundAudioSource::setNumLoops(int i) {
	mNumLoops = i;
}

int SDLSoundAudioSource::getNumLoops() {
	return mNumLoops;
}


static bool isID3v1(const char* buf) {
	return memcmp(buf, "TAG", 3) == 0;
}

static bool isID3v1ext(const char* buf) {
	return memcmp(buf, "TAG+", 4) == 0;
}

static bool isID3v2(const char* buf) {
	return memcmp(buf, "ID3", 3) == 0;
}

static int sizeID3v2(const char* buf) {
	int size = 0;
	for(int i=6; i<10; i++) {
		DEBUG_ASSERT((buf[i] & 0x80) == 0);
		size |= int(buf[i]) << ((9 - i) * 7);
	}
	return size;
}

static void skipID3(SDL_RWops *rwops) {
	static const int ID3V2_HEADER_LEN = 10;
	static const int ID3V1_LEN = 128;
	static const int ID3V1_EXT_LEN = 227 + ID3V1_LEN;

	//get length of file
	int len = rwops->seek(rwops, 0, SEEK_END);
	DEBUG_ASSERT(len > ID3V2_HEADER_LEN);

	//read a header's worth of data
	rwops->seek(rwops, 0, SEEK_SET);
	char buf[ID3V2_HEADER_LEN];
	int res = rwops->read(rwops, buf, ID3V2_HEADER_LEN, 1);
	DEBUG_ASSERT(res == 1);

	int pos;
	if(isID3v1ext(buf)) {
		pos = ID3V1_EXT_LEN;
	} else if(isID3v1(buf)) {
		pos = ID3V1_LEN;
	} else if(isID3v2(buf)) {
		pos = sizeID3v2(buf) + ID3V2_HEADER_LEN;
	} else {
		pos = 0;
	}
	res = rwops->seek(rwops, pos, SEEK_SET);
	DEBUG_ASSERT(res == pos);
}
