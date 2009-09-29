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

#include <MAUtil/Moblet.h>
#include <IX_AUDIOBUFFER.h>
#include <conprint.h>
#include <madmath.h>

using namespace MAUtil;

typedef short s16;

#define WAVE_HZ 440
#define SAMPLE_RATE 11025
#define CHAN 1
#define SECS 1
#define SAMPLES_PER_WAVE (SAMPLE_RATE / WAVE_HZ)
#define WAVES_PER_BUFFER (WAVE_HZ * SECS)
#define SAMPLES (SAMPLES_PER_WAVE * WAVES_PER_BUFFER)
#define SHRT_MAX 32767

s16 mBuffer[SAMPLES * CHAN];

class MyMoblet : public Moblet {
	MAAudioBufferInfo mInfo;
	bool mRunning;
public:
	MyMoblet() {
		printf("Hello Synth!\n");

		//generate one full sine wave...
		double angle = 0;
		for(int i=0; i<SAMPLES_PER_WAVE; i++, angle += ((M_PI * 2) / SAMPLES_PER_WAVE)) {
			mBuffer[i] = (s16) (sin(angle) * SHRT_MAX);
		}
		//then replicate it across the buffer.
		for(int i=1; i<WAVES_PER_BUFFER; i++) {
			memcpy(mBuffer + (SAMPLES_PER_WAVE * i), mBuffer, SAMPLES_PER_WAVE * sizeof(s16));
		}

		Handle data = maCreatePlaceholder();
		maCreateData(data, sizeof(mBuffer));
		maWriteData(data, mBuffer, 0, sizeof(mBuffer));
		Handle store = maOpenStore("sine.pcm", MAS_CREATE_IF_NECESSARY);
		maWriteStore(store, data);
		maCloseStore(store, 0);

		//set the info
		mInfo.fmt = AUDIO_FMT_S16;
		mInfo.sampleRate = SAMPLE_RATE;
		mInfo.numChannels = CHAN;
		mInfo.bufferSize = sizeof(mBuffer);
		mInfo.buffer = mBuffer;

		int res = maAudioBufferInit(&mInfo);
		printf("I %i\n", res);
		mRunning = true;
		//res = maAudioBufferReady();
		//printf("R %i\n", res);
	}

	void customEvent(const EVENT& event) {
		if(event.type != EVENT_TYPE_AUDIOBUFFER_FILL)
			return;
		printf("F %i\n", event.state);
		int res = maAudioBufferReady();
		printf("R %i\n", res);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
		if(keyCode == MAK_5) {
			if(mRunning) {
				int res = maAudioBufferClose();
				printf("C %i\n", res);
				mRunning = false;
			} else {
				int res = maAudioBufferInit(&mInfo);
				printf("I %i\n", res);
				mRunning = true;
			}
		}
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
