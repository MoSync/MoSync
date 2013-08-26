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

#include "Stream.h"
#include <windows.h>
#include "SoundServer.h"

#define MAX_SOUND_CHANNELS 32
#define DEFAULT_DECODEBUF_BYTES 16384
#define DEFAULT_DECODEBUF_SAMPLES (DEFAULT_DECODEBUF_BYTES>>2)
#define DEFAULT_AUDIOBUF_MS 500
#define DEFAULT_AUDIOBUF_SAMPLES ((DEFAULT_AUDIOBUF_MS * 44100) / 1000) * 2

enum MimeType {
	MT_MIDI,
	MT_MP3,
	MT_WAVE,
	MT_AU,
	MT_UNKNOWN
};
class Channel
{
public:
	Base::Stream *sound;
	int soundDataStart, 
		soundDataEnd;
	int sampleRate, 
		bitsPerSample,
		bytesPerSample, 
		numChannels,
		numSampleBytes;
	int currentSampleByte,
		samplePosition,
		sampleRateDelta;
	bool hasUnsignedSamples;
	bool littleEndian; // false = big endinan

	MimeType st;
	int samplesRead;
	bool active;
	int volume;
	int numLoops;
	int stopped;
	int decodedBytes;
	int decodedSamples;
	byte decodedBuffer[DEFAULT_DECODEBUF_BYTES];
	byte *decodedPtr;
};

bool openAudio(int sampleRate, int bitDepth, int numChannels, int bufferSize=DEFAULT_AUDIOBUF_MS, USER_CALLBACK callBack=NULL);
void closeAudio();
bool initChannel(int chan, Base::Stream *sound, MimeType st);
void freeChannel(int chan);
void stopChannel(int chan);
void startChannel(int chan);
void setPosChannel(int chan, int ms);
int getPosChannel(int chan);
int getNumChannels();
int getVolume(int chan);
void setVolume(int chan, int volume);
bool soundIsPlaying(int chan);
int	getLength(int chan);
void setLoopCount(int chan, int count);
void rewindChannel(int chan);

#endif // AUDIO_H