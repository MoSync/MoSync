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
#include <helpers/helpers.h>
#include <gsm_amr/amr_nb/dec/include/decoder_gsm_amr.h>
#include <gsm_amr/amr_wb/dec/include/decoder_amr_wb.h>
#include <FileStream.h>

#include "audio.h"
#include "base_errors.h"
#include "sdl_errors.h"
using namespace MoSyncError;

static void initChannelAmr(Channel* chan, SDL_RWops* rwops);

Channel gChannels[MAX_SOUND_CHANNELS];
Sound_AudioInfo gAudioInfo;
int gSoundVolume = 100;

static void convertFromMono(short *src, short *dst,
	int srcSampleRate, int dstSampleRate, int numSrcSamples, int numDstChannels)
{
	const int delta = (srcSampleRate<<16)/(dstSampleRate);
	int samplePos = 0;
	while((samplePos>>16) < numSrcSamples) {
		int sample = src[samplePos>>16];
		for(int i=0; i<numDstChannels; i++)
			*dst++ = sample;
		samplePos+=delta;
	}
}

static int readData(Channel *chan)
{
	if (chan->stopped)           
		chan->decodedBytes = 0;

	if(chan->sample) {	//SDL_sound
		Sound_Sample *sample = chan->sample;

		// don't need to decode more data just yet
		if (chan->decodedBytes  > 0)   
			return chan->decodedBytes;

		// there's more
		if (!(sample->flags & (SOUND_SAMPLEFLAG_ERROR | SOUND_SAMPLEFLAG_EOF)))
		{
			chan->decodedBytes = Sound_Decode(sample);
			chan->decodedPtr = (Uint8*)sample->buffer;

			return readData(chan);  
		}

		if (chan->numLoops==1) return(0);
		else if(chan->numLoops>1) { chan->numLoops--;}

		if(!Sound_Rewind(sample))
		{
			LOG("%s\n", Sound_GetError());
			BIG_PHAT_ERROR(SDLERR_SOUND_LOOP_FAILED);
		}
	} else {	//AMR
		//parameters->frame_type is Frame_Type_3GPP
		//set on a per-frame basis.

		//read frame header (1 byte)
		uint8* inputBuffer = chan->amr.parameters->pInputBuffer;
		int res = chan->amr.rwops->read(chan->amr.rwops, inputBuffer, 1, 1);
		if(res != 1) {
			LOGA("Couldn't read AMR frame header, stopping playback\n");
			return 0;
		}
		chan->amr.parameters->frame_type = (inputBuffer[0] & 0x78) >> 3;
		int frameLen = chan->amr.frameSizes[chan->amr.parameters->frame_type];
		LOGA("Frame type %i, length %i bytes\n", chan->amr.parameters->frame_type, frameLen);

		//read the rest of the frame
		res = chan->amr.rwops->read(chan->amr.rwops, inputBuffer, frameLen - 1, 1);
		if(res != 1) {
			LOGA("Couldn't read AMR frame body, stopping playback\n");
			return 0;
		}

		//decode the frame
		res = chan->amr.decoder->ExecuteL(chan->amr.parameters);
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

		chan->decodedPtr = (uint8*)chan->amr.parameters->pOutputBuffer;

#define SOUND_DUMP
#ifdef SOUND_DUMP
		Base::WriteFileStream file8("amr.pcm", true);
		file8.write(chan->amr.parameters->pOutputBuffer, chan->amr.frameLength * sizeof(short));
#endif

		//up-sample to 44.1 kHz.
#if 0
		res = SDL_ConvertAudio(&chan->amr.acvt);
		if(res != 0) {
			LOG("SDL_ConvertAudio %i\n", res);
			return 0;
		}
		chan->decodedBytes = int(chan->amr.acvt.len * chan->amr.acvt.len_ratio);
#else
		convertFromMono(chan->amr.parameters->pOutputBuffer, chan->amr.buffer,
			chan->amr.sampleRate, gAudioInfo.rate, chan->amr.frameLength, gAudioInfo.channels);
		chan->decodedPtr = (uint8*)chan->amr.buffer;
		chan->decodedBytes = ((chan->amr.frameLength * gAudioInfo.rate) /
			chan->amr.sampleRate) * sizeof(short) * gAudioInfo.channels;
#endif

		LOGA("decodedBytes: %i\n", chan->decodedBytes);

#ifdef SOUND_DUMP
		Base::WriteFileStream file4("sdl.pcm", true);
		file4.write(chan->decodedPtr, chan->decodedBytes);
#endif
		return chan->decodedBytes;
	}

	return(readData(chan));
}

static Sint32 tempBuffer[DEFAULT_AUDIOBUF_SAMPLES*2];

static void soundCallback(void *userdata, Uint8 *buf,int len)
{
//	Channel *channels = (Channel*) userdata;
	Sint16 *src, *dst;
	Sint32 *mixDst;
	int bytesWritten, copySize, i, j; 

	memset(tempBuffer, 0, DEFAULT_AUDIOBUF_SAMPLES*2*sizeof(Uint32));

	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		bytesWritten = 0;
		copySize = 0;

		if(!gChannels[i].active||gChannels[i].stopped)
			continue;

		while(bytesWritten<len)
		{
			if (!readData(&gChannels[i])) 
			{
				// all data have been read
				gChannels[i].stopped = 1;
				//TODO: add_sound_event
				break;
			} 

			copySize = len - bytesWritten;
			if(copySize > gChannels[i].decodedBytes)
			{
				copySize = gChannels[i].decodedBytes;
			}

			if (copySize > 0)
			{
				src = (Sint16*)gChannels[i].decodedPtr; // hard coded for 16 bit sound
				mixDst = (Sint32*)&tempBuffer[(bytesWritten>>1)];
				for(j = 0; j < copySize>>1; j++)
				{
					mixDst[j] += (((Sint32)src[j])*gChannels[i].volume)>>16;
				}
				bytesWritten += copySize;
				gChannels[i].decodedPtr += copySize;
				gChannels[i].decodedBytes -= copySize;
				gChannels[i].samplesRead += copySize>>1;
			} 
		}
	}
	
	dst = (Sint16*) buf;

	for(i = 0; i < len>>1; i++)
	{
		Sint32 sample = tempBuffer[i];
		if(sample < -32768) sample = -32768;
		if(sample > 32767) sample = 32767;
		dst[i] = (Sint16) sample;
	}
}

int openAudio()
{
	int i, res;

	SDL_Init(SDL_INIT_AUDIO);
	TEST_Z(Sound_Init());

	SDL_AudioSpec *desired, *obtained;
	desired = (SDL_AudioSpec*) malloc(sizeof(SDL_AudioSpec));
	obtained = (SDL_AudioSpec*) malloc(sizeof(SDL_AudioSpec));

	desired->freq=MY_SAMPLE_RATE;
	desired->format=AUDIO_S16SYS;
	desired->channels=2;
	desired->samples=DEFAULT_AUDIOBUF_SAMPLES;
	desired->callback=soundCallback;
	desired->userdata=gChannels;

	res = SDL_OpenAudio(desired, obtained);
	if (res < 0)
	{
		LOG(SDL_GetError());
		//BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
		return res;
	}

	if(desired->samples != obtained->samples)
	{
		//BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
		return res;
	}

	gAudioInfo.channels = obtained->channels;
	gAudioInfo.format = obtained->format;
	gAudioInfo.rate = obtained->freq;

	LOGA("Channels: %i\nFormat: 0x%x\nRate: %i Hz\n",
		gAudioInfo.channels, gAudioInfo.format, gAudioInfo.rate);

	free(desired);
	free(obtained);

	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		//  gChannels[i].sample = NULL;
		gChannels[i].active = false;
		gChannels[i].sample = NULL;
		gChannels[i].stopped = 1;
		gChannels[i].numLoops = 1;
		gChannels[i].decodedPtr = NULL;
		gChannels[i].decodedBytes = 0;
	}

	SDL_PauseAudio(0);

#if 0
	const Sound_DecoderInfo** i;
	for(i = Sound_AvailableDecoders(); *i != NULL; i++) {
		LOG("Supported sound format: [%s], which is [%s]. Author: [%s]. URL: [%s]\n",
			*(*i)->extensions, (*i)->description, (*i)->author, (*i)->url);
	}
#endif

	return 1;
}

void closeAudio()
{
	SDL_LockAudio(); 
	SDL_CloseAudio();

	/** 
		\bug Sound_Quit crashes, because of deadlock. 
		my guess it has something to do with Sound_FreeSample that is
		executed on all the loaded samples according to the api docs.
	*/

	/*
	if(!Sound_Quit())
	{
		LOG(Sound_GetError()); 
		Sound_ClearError();
	}
	*/
}

#define SDL_SOUND_READ(rwops, dst, len) \
	MYASSERT(rwops->read(rwops, dst, len, 1) == 1, SDLERR_SOUND_LOAD_FAILED)

static void initChannelAmr(Channel* chan, SDL_RWops* rwops) {
	//read and parse AMR file header.
	enum CodecType {
		NB, WB, NBMC, WBMC
	} codecType;

	do {
		char header[32];
		int pos = 0;

		SDL_SOUND_READ(rwops, header + pos, 6);
		pos += 6;
		if(memcmp(header, "#!AMR\n", pos) == 0) {
			codecType = NB;
			break;
		}

		SDL_SOUND_READ(rwops, header + pos, 3);
		pos += 3;
		if(memcmp(header, "#!AMR-WB\n", pos) == 0) {
			codecType = WB;
			break;
		}

		SDL_SOUND_READ(rwops, header + pos, 3);
		pos += 3;
		if(memcmp(header, "#!AMR_MC1.0\n", pos) == 0) {
			codecType = NBMC;
			break;
		}

		SDL_SOUND_READ(rwops, header + pos, 3);
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
		chan->amr.decoder = CDecoder_AMR_NB::NewL();
		chan->amr.sampleRate = AMR_NB_SAMPLE_RATE;
		chan->amr.frameLength = AMR_NB_FRAME_SAMPLES;
		chan->amr.frameSizes = sAmrNbFrameSizes;
		break;
	case WB:
		chan->amr.decoder = CDecoder_AMR_WB::NewL();
		chan->amr.sampleRate = AMR_WB_SAMPLE_RATE;
		chan->amr.frameLength = AMR_WB_FRAME_SAMPLES;
		chan->amr.frameSizes = sAmrWbFrameSizes;
		break;
	default:
		LOG("Unsupported AMR type!\n");
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	}

	chan->amr.parameters = new tPVAmrDecoderExternal;
	ZERO_OBJECT(*chan->amr.parameters);
	int res = chan->amr.decoder->StartL(chan->amr.parameters, true, true);
	if(res != 0) {
		LOG("StartL %i\n", res);
		BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
	}
	//chan->amr.parameters->pOutputBuffer = chan->amr.buffer;
	chan->amr.parameters->input_format = WMF;
	chan->amr.rwops = rwops;

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
}

int initChannel(int chan, SDL_RWops* rwops, bool isAmr)
{
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==true)
	{
		freeChannel(chan);
	}

	if(isAmr) {
		initChannelAmr(&gChannels[chan], rwops);
	} else {	//use SDL_sound
		gChannels[chan].sample = Sound_NewSample(rwops, NULL, &gAudioInfo, DEFAULT_DECODEBUF_BYTES);

		if(!gChannels[chan].sample)
		{
			LOG("SDL_sound says: %s\n", Sound_GetError());
			BIG_PHAT_ERROR(SDLERR_SOUND_LOAD_FAILED);
			//return 0;
		}

		const Sound_DecoderInfo *info = gChannels[chan].sample->decoder;
		LOG("\nusing decoder:\n%s\nn", info->description);
		LOG("%s\n\n", info->author);
	}

	gChannels[chan].active = true;
	gChannels[chan].numLoops = 1;
	setVolume(chan, gSoundVolume);
	return 1;
}

void freeChannel(int chan)
{
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active == false) return; //BIG_PHAT_ERROR?
	gChannels[chan].stopped = true;
	gChannels[chan].active = false;
	if(gChannels[chan].sample!=NULL)
	{
		Sound_FreeSample(gChannels[chan].sample);
		gChannels[chan].sample = NULL;
	}
	SAFE_DELETE(gChannels[chan].amr.decoder);
	SAFE_DELETE(gChannels[chan].amr.parameters);
}

void stopChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	gChannels[chan].stopped = true;	
}

void startChannel(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	gChannels[chan].stopped = false;	
}

void setPosChannel(int chan, int ms)
{
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	if(!Sound_Seek(gChannels[chan].sample, ms))
	{
		// out of bounds or something..
	}
}

int getPosChannel(int chan)
{
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return gChannels[chan].samplesRead;
}

int getNumChannels() {
	return MAX_SOUND_CHANNELS;
}

int getVolume(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	//if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return (gChannels[chan].volume*100)>>16;
}

void setVolume(int chan, int volume) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	//if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	if(volume<0) volume = 0;
	if(volume>100) volume = 100;
	gSoundVolume = volume;
	gChannels[chan].volume = (volume*0xffff)/100;
}

int soundIsPlaying(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) return 0;
	return gChannels[chan].stopped==0;
}

int getLength(int chan) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	return -1;
}

void setLoopCount(int chan, int count) {
	if((chan<0)||(chan>=MAX_SOUND_CHANNELS)) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INVALID);
	if(gChannels[chan].active==false) BIG_PHAT_ERROR(ERR_SOUND_CHANNEL_INACTIVE);
	MYASSERT(count != 0, ERR_SOUND_LOOP_COUNT_INVALID);

	gChannels[chan].numLoops = count;
}

void rewindChannel(int chan){
	if(chan < 32) if(gChannels[chan].sample) if(!Sound_Rewind(gChannels[chan].sample)) {
		// Could not rewind sample.
	}
}
