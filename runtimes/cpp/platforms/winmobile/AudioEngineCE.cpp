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

#include "AudioEngine.h"
#include "AudioChannel.h"
#include "AudioSource.h"

#include "WaveAudioSource.h"

#include "config_platform.h"
#include "Stream.h"

#include "SoundServer.h"

#define DEFAULT_AUDIOBUF_SAMPLES 1024*4
#define MY_SAMPLE_RATE 44100

namespace AudioEngine {

	AudioChannel *gChannels[MAX_CHANNELS];

	CSoundServer soundServer;

	static int gTempBuffer[DEFAULT_AUDIOBUF_SAMPLES*2];

	static void soundCallback(void *buf,long len) {
		int numSamples = len>>2;
		memset(gTempBuffer, 0, DEFAULT_AUDIOBUF_SAMPLES*2*sizeof(int));
		for(int i = 0; i < MAX_CHANNELS; i++) {
			AudioChannel *chnl = gChannels[i]; 
			if(chnl && chnl->isActive()) {
				chnl->mix(gTempBuffer, numSamples);
			}
		}

		short* dst = (short*)buf;
		for(int i = 0; i < numSamples<<1; i++) {
			int sample = gTempBuffer[i];
			if(sample<-32768) 
				sample = 32768;
			else if(sample>=32767) 
				sample = 32767;
			*dst++ = (short) (gTempBuffer[i]);
		}
	}

	int getSampleRate() {
		return MY_SAMPLE_RATE;
	}

	int init() {
		if(!soundServer.open(MY_SAMPLE_RATE,16,2,soundCallback,DEFAULT_AUDIOBUF_SAMPLES*2*sizeof(short))) {
			return -1;
		}
		
		for(int i = 0; i < MAX_CHANNELS; i++) {
			gChannels[i] = new AudioChannel(MY_SAMPLE_RATE, NULL);
		}

		return 0;
	}

	int close() {
		for(int i = 0; i < MAX_CHANNELS; i++) {
			if(gChannels[i]) {
				AudioSource *src = gChannels[i]->getAudioSource();
				if(src)
					src->close();
			}
		}

		soundServer.close();
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


	AudioSource* getAudioSource(const char *mimeType, Base::Stream *s) {
		AudioSource *audioSource = NULL;
		if(strcmp(mimeType, "audio/3gpp") == 0 ||
			strcmp(mimeType, "audio/amr") == 0 ||
			strcmp(mimeType, "audio/amr-wb") == 0) {
			//audioSource = new AmrAudioSource(s);
				return NULL;
		} else if(
			strcmp(mimeType, "audio/wav") == 0 ||
			strcmp(mimeType, "audio/x-wav") == 0) {
			audioSource = new WaveAudioSource(s);
		}
		if(audioSource->init() !=0)
			return NULL;
		else
			return audioSource;
	}
};