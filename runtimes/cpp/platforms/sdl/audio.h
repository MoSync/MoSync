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
