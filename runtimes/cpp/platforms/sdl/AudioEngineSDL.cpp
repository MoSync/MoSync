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


		SDL_LockAudio();
	   	 SDL_UnlockAudio();
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

		if(audioSource->init() !=0) {
			delete audioSource;
			return NULL;
		}
		else
			return audioSource;
	}
};
