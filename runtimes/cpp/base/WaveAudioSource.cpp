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

#include "config_platform.h"
#include "WaveAudioSource.h"
#include "Stream.h"
#include "StreamHelpers.h"
#include <stdint.h>

using namespace Base;
using namespace MoSyncError;

WaveAudioSource::WaveAudioSource ( Base::Stream *stream )
: mStream(stream)
{
    mPos = 0;
    mNumLoops = 1;
}

WaveAudioSource::~WaveAudioSource()
{
}


int WaveAudioSource::fillBuffer()
{
	/*
	int pos;
	if(!mStream->tell(pos)) return 0;
	pos-=mSoundDataStart;
*/
	MutexHandler mutex(&mMutex);
	int pos = mPos;
	mStream->seek(Seek::Start, pos);

	int amountToRead = MIN(WAVE_AUDIO_BUFFER_SIZE,mLengthBytes-pos);
	if(amountToRead==0) {
		if(mNumLoops>1) {
			mNumLoops--;
			mStream->seek(Seek::Start, mSoundDataStart);
			mPos = 0;
			return fillBuffer();
		} else {
			return 0;
		}
	}
	bool res = mStream->read(mBuffer, amountToRead);
	if(!res) return 0;
	
	mPos += amountToRead;
	int samplesRead = amountToRead/(info.bytesPerSample*info.numChannels); 

	return samplesRead;
}

const void* WaveAudioSource::getBuffer() const
{
	return mBuffer;
}

void WaveAudioSource::setPosition(int ms)
{
	MutexHandler mutex(&mMutex);
	mPos = (ms*info.sampleRate/1000)*info.bytesPerSample*info.numChannels;
	if(mPos<0) mPos = 0;
	else if(mPos>mLengthBytes) mPos = mLengthBytes;
}

int WaveAudioSource::getPosition() const
{
	return 0;
}

int WaveAudioSource::getLength() const
{
	return 0;
}

void WaveAudioSource::setNumLoops(int i)
{
	mNumLoops = i;
}

int WaveAudioSource::getNumLoops()
{
	return mNumLoops;
}

/*
int WaveAudioSource::init() {
	return 0;
}
*/

int WaveAudioSource::init()
{
	struct chunk_hdr {
		char  id[4];
		uint32_t len;
	};

	chunk_hdr chnk;

	Stream *sound = mStream;

	sound->read(&chnk, sizeof(chunk_hdr));
	if(memcmp(chnk.id, "RIFF", 4) != 0) return false;
	sound->read(chnk.id, 4);
	if(memcmp(chnk.id, "WAVE", 4) != 0) return false;

	bool foundFMT = false, foundDATA = false;

	while(1) {
		if(!sound->read(chnk.id, 4)) break;
		chnk.len = readLEInt(sound);
		if(memcmp(chnk.id, "fmt ", 4) == 0) {
			foundFMT = true;

			struct fmt_header {
				short          wFormatTag;
				unsigned short wChannels;
				uint32_t  dwSamplesPerSec;
				uint32_t  dwAvgBytesPerSec;
				unsigned short wBlockAlign;
				unsigned short wBitsPerSample;
			};
			
			fmt_header fmt;
			fmt.wFormatTag			= readLEShort(sound);
			fmt.wChannels			= readLEUnsignedShort(sound);
			fmt.dwSamplesPerSec		= readLEUnsignedInt(sound);
			fmt.dwAvgBytesPerSec	= readLEUnsignedInt(sound);
			fmt.wBlockAlign			= readLEUnsignedShort(sound);
			fmt.wBitsPerSample		= readLEUnsignedShort(sound);

			if(fmt.wFormatTag != 1) return false; // unsupported compression
			
			info.bufferSize = WAVE_AUDIO_BUFFER_SIZE;
			info.numChannels = fmt.wChannels;
			info.sampleRate = fmt.dwSamplesPerSec;
			info.bitDepth = fmt.wBitsPerSample;
			info.bytesPerSample = fmt.wBlockAlign / fmt.wChannels;
			bool sign;
			// <= 8-bit pcm wave is unsigned / > 8-bit pcm wave is signed
			if(fmt.wBitsPerSample<9) sign = false;
			else sign = true;

			if(sign) {
				if(info.bytesPerSample==1) info.fmt = AudioSource::FMT_S8;
				else if(info.bytesPerSample==2) info.fmt = AudioSource::FMT_S16;
			} else {
				if(info.bytesPerSample==1) info.fmt = AudioSource::FMT_U8;
				else if(info.bytesPerSample==2) info.fmt = AudioSource::FMT_U16;
			}

		} else if(memcmp(chnk.id, "data", 4) == 0) {
			foundDATA = true;
			//channel->numSampleBytes = chnk.len/*(channel->bytesPerSample*channel->numChannels)*/;
			mLengthBytes = chnk.len;
			mLengthSamples = mLengthBytes/(info.bytesPerSample*info.numChannels);
			if(!sound->tell(mSoundDataStart)) return false;
			break;
		} else {
			sound->seek(Base::Seek::Current, chnk.len);
		}
	}

	// yes this is little endian

	return ((foundFMT&&foundDATA)==false)?-1:0; 
}
