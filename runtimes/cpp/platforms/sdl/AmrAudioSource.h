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

#ifndef _AMR_AUDIO_SOURCE_H_
#define _AMR_AUDIO_SOURCE_H_

#include "AudioSource.h"

class CDecoder_AMRInterface;
struct tPVAmrDecoderExternal;
#define AMR_NB_FRAME_SAMPLES 160
#define AMR_WB_FRAME_SAMPLES 320
#define MY_SAMPLE_RATE 44100
#define AMR_NB_SAMPLE_RATE 8000
#define AMR_WB_SAMPLE_RATE 16000
#define MY_WB_CONVERSION_RATE ((MY_SAMPLE_RATE / AMR_WB_SAMPLE_RATE) + 1) * 2	//stereo
#define AMR_BUFFER_SIZE AMR_WB_FRAME_SAMPLES * MY_WB_CONVERSION_RATE

namespace Base {
	class Stream;
};

struct AMR_Sample {
	CDecoder_AMRInterface* decoder;
	tPVAmrDecoderExternal* parameters;
	//SDL_RWops* rwops;
	//SDL_AudioCVT acvt;
	int sampleRate;	//samples per second
	int frameLength;	//samples per frame
	const int* frameSizes;	//bytes per encoded frame
	short buffer[AMR_BUFFER_SIZE];
};

class AmrAudioSource : public AudioSource {
public:
	AmrAudioSource(Base::Stream *stream);
	virtual ~AmrAudioSource();

	int init();

	int fillBuffer();
	const void* getBuffer() const;

	void setPosition(int ms);
	int getPosition() const;
	int getLength() const;

	void setNumLoops(int i);
	int getNumLoops();

protected:
	int mNumLoops;

	Base::Stream *mStream;
	AMR_Sample amr;
};

#endif /* _AMR_AUDIO_SOURCE_H_ */
