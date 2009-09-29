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

#include "AmrAudioSource.h"
#include <gsm_amr/amr_nb/dec/include/decoder_gsm_amr.h>
#include <gsm_amr/amr_wb/dec/include/decoder_amr_wb.h>

#include "config_platform.h"
#include "Stream.h"
#include "FileStream.h"
#include "base_errors.h"
#include "sdl_errors.h"
using namespace Base;
using namespace MoSyncError;

AmrAudioSource::AmrAudioSource(Base::Stream *stream) : mNumLoops(1), mStream(stream) {
}


/*
int AmrAudioSource::init() {

	return 0;
}
*/

AmrAudioSource::~AmrAudioSource() {
}


int AmrAudioSource::fillBuffer()
{
		//parameters->frame_type is Frame_Type_3GPP
		//set on a per-frame basis.

		MutexHandler mutex(&mMutex);
		int decodedBytes = 0;

		//read frame header (1 byte)
		uint8* inputBuffer = amr.parameters->pInputBuffer;
		int res = mStream->read(inputBuffer, 1);
		if(res != 1) {
			LOGA("Couldn't read AMR frame header, stopping playback\n");
			return 0;
		}
		amr.parameters->frame_type = (inputBuffer[0] & 0x78) >> 3;
		int frameLen = amr.frameSizes[amr.parameters->frame_type];
		LOGA("Frame type %i, length %i bytes\n", amr.parameters->frame_type, frameLen);

		//read the rest of the frame
		res = mStream->read(inputBuffer, frameLen - 1);
		if(res != 1) {
			LOGA("Couldn't read AMR frame body, stopping playback\n");
			return 0;
		}

		//decode the frame
		res = amr.decoder->ExecuteL(amr.parameters);
		LOGA("ExecuteL %i\n", res);
		/*if(res != frameLen - 1) {
			LOG("ExecuteL %i\n", res);
			return 0;
		}*/

#if 0	//sign hack
		for(int i=0; i<sFrameLength; i++) {
			chan->amr.parameters->pOutputBuffer[i] = -chan->amr.parameters->pOutputBuffer[i];
		}
#endif

		//chan->decodedPtr = (uint8*)chan->amr.parameters->pOutputBuffer;

		/*
#define SOUND_DUMP
#ifdef SOUND_DUMP
		Base::WriteFileStream file8("amr.pcm", true);
		file8.write(amr.parameters->pOutputBuffer, amr.frameLength * sizeof(short));
#endif
		*/

		//up-sample to 44.1 kHz.
#if 0
		res = SDL_ConvertAudio(&chan->amr.acvt);
		if(res != 0) {
			LOG("SDL_ConvertAudio %i\n", res);
			return 0;
		}
		chan->decodedBytes = int(chan->amr.acvt.len * chan->amr.acvt.len_ratio);
#else
		/*
		convertFromMono(chan->amr.parameters->pOutputBuffer, chan->amr.buffer,
			chan->amr.sampleRate, gAudioInfo.rate, chan->amr.frameLength, gAudioInfo.channels);
		chan->decodedPtr = (uint8*)chan->amr.buffer;
		*/
		/*
		chan->decodedBytes = ((chan->amr.frameLength * gAudioInfo.rate) /
			chan->amr.sampleRate) * sizeof(short) * gAudioInfo.channels;
		*/

#endif

		LOGA("decodedBytes: %i\n", chan->decodedBytes);

		/*
#ifdef SOUND_DUMP
		Base::WriteFileStream file4("sdl.pcm", true);
		file4.write(chan->decodedPtr, chan->decodedBytes);
#endif
		*/
		decodedBytes = amr.frameLength;

	return decodedBytes;
}

const void* AmrAudioSource::getBuffer() const {
	return (const void*)amr.parameters->pOutputBuffer;
}

void AmrAudioSource::setPosition(int ms) {
	mMutex.lock();

	mMutex.unlock();
}

int AmrAudioSource::getPosition() const {
	return 0;
}

int AmrAudioSource::getLength() const {
	return 0;
}

void AmrAudioSource::setNumLoops(int i) {
	mNumLoops = i;
}

int AmrAudioSource::getNumLoops() {
	return mNumLoops;
}

#define SDL_SOUND_READ(stream, dst, len) \
	MYASSERT(stream->read(dst, len) == 1, SDLERR_SOUND_LOAD_FAILED)

int AmrAudioSource::init() {
	//read and parse AMR file header.
	enum CodecType {
		NB, WB, NBMC, WBMC
	} codecType;

	do {
		char header[32];
		int pos = 0;

		SDL_SOUND_READ(mStream, header + pos, 6);
		pos += 6;
		if(memcmp(header, "#!AMR\n", pos) == 0) {
			codecType = NB;
			break;
		}

		SDL_SOUND_READ(mStream, header + pos, 3);
		pos += 3;
		if(memcmp(header, "#!AMR-WB\n", pos) == 0) {
			codecType = WB;
			break;
		}

		SDL_SOUND_READ(mStream, header + pos, 3);
		pos += 3;
		if(memcmp(header, "#!AMR_MC1.0\n", pos) == 0) {
			codecType = NBMC;
			break;
		}

		SDL_SOUND_READ(mStream, header + pos, 3);
		pos += 3;
		if(memcmp(header, "#!AMR-WB_MC1.0\n", pos) == 0) {
			codecType = NBMC;
			break;
		}

		LOG("Not a valid AMR file!\n");
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	} while(0);

	//see http://maveriik.blogspot.com/2007/10/amr-file-format.html
	static const int sAmrNbFrameSizes[] = { 13, 14, 16, 18, 20, 21, 27, 32,
		6,7,6,6, 1,1,1, 1 };

	//Wide-Band. See 3GPP TS 26.201. May be inconsistent with reality.
	static const int sAmrWbFrameSizes[] = { 18, 24, 33, 37, 41, 47, 51, 59, 61,
		6, 1,1,1,1, 1,1 };

	//set up decoder.
	switch(codecType) {
	case NB:
		amr.decoder = CDecoder_AMR_NB::NewL();
		amr.sampleRate = AMR_NB_SAMPLE_RATE;
		amr.frameLength = AMR_NB_FRAME_SAMPLES;
		amr.frameSizes = sAmrNbFrameSizes;
		break;
	case WB:
		amr.decoder = CDecoder_AMR_WB::NewL();
		amr.sampleRate = AMR_WB_SAMPLE_RATE;
		amr.frameLength = AMR_WB_FRAME_SAMPLES;
		amr.frameSizes = sAmrWbFrameSizes;
		break;
	default:
		LOG("Unsupported AMR type!\n");
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	}

	amr.parameters = new tPVAmrDecoderExternal;
	ZERO_OBJECT(*amr.parameters);
	int res = amr.decoder->StartL(amr.parameters, true, true);
	if(res != 0) {
		LOG("StartL %i\n", res);
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	}
	//chan->amr.parameters->pOutputBuffer = chan->amr.buffer;
	amr.parameters->input_format = WMF;
	//amr.rwops = rwops;

#if 0
	res = SDL_BuildAudioCVT(&chan->amr.acvt, AUDIO_S16, 1, 8000,
		gAudioInfo.format, gAudioInfo.channels, gAudioInfo.rate);
	if(res == -1) {
		LOG("Couldn't build converter!\n");
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	}
	chan->amr.acvt.buf = (Uint8*)chan->amr.buffer;
	chan->amr.acvt.len = AMR_FRAME_SAMPLES * sizeof(short);
#endif

#ifdef SOUND_DUMP
	Base::WriteFileStream file8("amr.pcm");	//truncate the file
	Base::WriteFileStream file4("sdl.pcm");	//truncate the file
#endif

	info.bitDepth = 16;
	info.fmt = AudioSource::FMT_S16;
	info.sampleRate = 8000;
	info.canSeek = true;
	info.bufferSize = AMR_BUFFER_SIZE*sizeof(short);
	info.numChannels = 1;
	
	return 0;
}
