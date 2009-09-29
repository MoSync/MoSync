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