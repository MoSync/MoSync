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