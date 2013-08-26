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
