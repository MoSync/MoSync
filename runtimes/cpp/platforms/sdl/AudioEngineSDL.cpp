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

#include <SDL/SDL_audio.h>
#include "config_platform.h"
#include "AudioEngine.h"
#include "AudioChannel.h"
#include "AudioSource.h"
#include "Stream.h"
#include "WaveAudioSource.h"
#include "AmrAudioSource.h"
#ifndef __NO_SDL_SOUND__
#include <SDL/SDL_sound.h>
#include "SDLSoundAudioSource.h"
#endif
#include "sdl_stream.h"

#define DEFAULT_AUDIOBUF_SAMPLES 1024*4
#define MY_SAMPLE_RATE 44100

#ifdef LINUX
#define stricmp(x, y) strcasecmp(x, y)
#endif

namespace AudioEngine {

	AudioChannel *gChannels[MAX_CHANNELS];
	SDL_AudioSpec gAudioSpec;

	static Sint32 gTempBuffer[DEFAULT_AUDIOBUF_SAMPLES*2];

	static void soundCallback(void *userdata, Uint8 *buf,int len) {
		//MutexHandler m(&gMutex);

		int numSamples = len>>2;
		memset(gTempBuffer, 0, DEFAULT_AUDIOBUF_SAMPLES*2*sizeof(Uint32));
		for(int i = 0; i < MAX_CHANNELS; i++) {
			AudioChannel *chnl = gChannels[i]; 
			if(chnl->isActive()) {
				chnl->mix(gTempBuffer, numSamples);
			}
		}

		Sint16* dst = (Sint16*)buf;
		for(int i = 0; i < numSamples<<1; i++) {
			int sample = gTempBuffer[i];
			if(sample<-32768) 
				sample = 32768;
			else if(sample>=32767) 
				sample = 32767;
			*dst++ = (Sint16) (gTempBuffer[i]);
		}
	}

	int getSampleRate() {
		return gAudioSpec.freq;
	}

	int init() {
#ifndef __NO_SDL_SOUND__
		if(Sound_Init() == 0) return -1;
#endif		
	
		SDL_AudioSpec desired;
		desired.freq=MY_SAMPLE_RATE;
		desired.format=AUDIO_S16SYS;
		desired.channels=2;
		desired.samples=DEFAULT_AUDIOBUF_SAMPLES;
		desired.callback=soundCallback;
		desired.userdata=0;

		int res = SDL_OpenAudio(&desired, &gAudioSpec);
		if(res < 0) return -1;

		for(int i = 0; i < MAX_CHANNELS; i++) {
			gChannels[i] = new AudioChannel(gAudioSpec.freq, NULL);
		}

		SDL_PauseAudio(0);
		//for(int i = 0; i < MAX_CHANNELS; i++) gChannels[i]->setActive(true);

		return 0;
	}

	int close() {
			
		for(int i = 0; i < MAX_CHANNELS; i++) {
			if(gChannels[i]) {
				AudioSource *src = gChannels[i]->getAudioSource();
				if(src)
					gChannels[i]->getAudioSource()->close();
			}
		}

		SDL_CloseAudio();
		for(int i = 0; i < MAX_CHANNELS; i++) {
			if(gChannels[i]) {
				AudioSource *src = gChannels[i]->getAudioSource();
				if(src) delete src;
				delete gChannels[i];
				gChannels[i] = NULL;
			}			
		}

		return 0;
	}

	AudioChannel* getChannel(int i) {
		return gChannels[i];
	}


	AudioSource* getAudioSource(const char *mimeType, Base::Stream *s)
	{
		AudioSource *audioSource;

		if ( strcmp(mimeType, "audio/3gpp") == 0 ||
			 strcmp(mimeType, "audio/amr") == 0 ||
			 strcmp(mimeType, "audio/amr-wb") == 0 )
		{
			audioSource = new AmrAudioSource(s);
		} 
		else if ( strcmp(mimeType, "audio/wav") == 0 ||
				  strcmp(mimeType, "audio/x-wav") == 0) 
		{
			audioSource = new WaveAudioSource(s);
		}		
#ifndef __NO_SDL_SOUND__		
		else 
		{
			SDL_RWops* rwops = Base::SDL_RWFromStream(s);
			if ( rwops == NULL )
				return NULL;

			audioSource = new SDLSoundAudioSource(rwops);
		}
#else
		else
		{
			return NULL;
		}
#endif		

		if(audioSource->init() !=0)
			return NULL;
		else
			return audioSource;
	}
};
