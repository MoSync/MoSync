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

#ifndef AUDIO_H
#define AUDIO_H

//#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_sound.h>

#define MAX_SOUND_CHANNELS 32
#define DEFAULT_DECODEBUF_BYTES 16384
#define DEFAULT_AUDIOBUF_SAMPLES 1024*4

class CDecoder_AMRInterface;
struct tPVAmrDecoderExternal;
#define AMR_NB_FRAME_SAMPLES 160
#define AMR_WB_FRAME_SAMPLES 320
#define MY_SAMPLE_RATE 44100
#define AMR_NB_SAMPLE_RATE 8000
#define AMR_WB_SAMPLE_RATE 16000
#define MY_WB_CONVERSION_RATE ((MY_SAMPLE_RATE / AMR_WB_SAMPLE_RATE) + 1) * 2	//stereo

struct AMR_Sample {
	CDecoder_AMRInterface* decoder;
	tPVAmrDecoderExternal* parameters;
	SDL_RWops* rwops;
	//SDL_AudioCVT acvt;
	int sampleRate;	//samples per second
	int frameLength;	//samples per frame
	const int* frameSizes;	//bytes per encoded frame
	short buffer[AMR_WB_FRAME_SAMPLES * MY_WB_CONVERSION_RATE];
};

class Channel
{
public:
	Sound_Sample *sample;
	AMR_Sample amr;
	int samplesRead;
	/*
	Mix_Chunk *sample;
	*/
	bool active;
	int volume;
	int numLoops;
	int stopped;
	int decodedBytes;
	Uint8 *decodedPtr;

	int getTime()
	{
		return samplesRead*1000/sample->actual.rate;
	}
};

int openAudio();
void closeAudio();
int initChannel(int chan, SDL_RWops* rwops, bool isAmr);
void freeChannel(int chan);
void stopChannel(int chan);
void startChannel(int chan);
void setPosChannel(int chan, int ms);
int getPosChannel(int chan);
int getNumChannels();
int getVolume(int chan);
void setVolume(int chan, int volume);
int soundIsPlaying(int chan);
int	getLength(int chan);
void setLoopCount(int chan, int count);
void rewindChannel(int chan);

#endif // AUDIO_H
