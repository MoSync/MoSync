/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "systemstub.h"
#include "util.h"
#include "event.h"

#include <conprint.h>
#include <ma.h>
#include <MAUtil/FrameBuffer.h>
#include <MAUtil/Vector.h>
#include <IX_AUDIOBUFFER.h>

struct MoSyncStub : SystemStub {
	enum {
		SCREEN_W = 320,
		SCREEN_H = 200,
		SOUND_SAMPLE_RATE = 22050
	};

	struct Timer {
		uint32 delay;
		TimerCallback callback;
		void *param;

		int id;

		int curTime;
	};

	/*
	MAUtil::Vector<Timer> timers;
	*/
	Timer timer;
	bool timerActive;

	int curSample;
	int samplesAtNextTick;

	int timeOfStart;
#define BUFFERSIZE (2048)
	unsigned char buffer[BUFFERSIZE];
	int currentOrientation;
	AudioCallback callback;
	void *param;

	virtual ~MoSyncStub() {}
	virtual void init(const char *title);
	virtual void destroy();
	virtual void setPalette(uint8 s, uint8 n, const uint8 *buf);
	virtual void copyRect(uint16 x, uint16 y, uint16 w, uint16 h, const uint8 *buf, uint32 pitch);
	virtual void processEvents();
	virtual void sleep(uint32 duration);
	virtual uint32 getTimeStamp();
	virtual void startAudio(AudioCallback callback, void *param);
	virtual void stopAudio();
	virtual uint32 getOutputSampleRate();
	virtual void *addTimer(uint32 delay, TimerCallback callback, void *param);
	virtual void removeTimer(void *timerId);
	virtual void *createMutex();
	virtual void destroyMutex(void *mutex);
	virtual void lockMutex(void *mutex);
	virtual void unlockMutex(void *mutex);

	int processTimers();
	void handleKeys(int key, int pressed);

};

SystemStub *SystemStub_MoSync_create() {
	return new MoSyncStub();
}

void MoSyncStub::init(const char *title) {
	memset(&_pi, 0, sizeof(_pi));
	currentOrientation = 0;
	FrameBuffer_init(SCREEN_W, SCREEN_H, currentOrientation, FLAG_4BPP|FLAG_RGB666);
	timerActive = false;
	timeOfStart = maGetMilliSecondCount();
}

void MoSyncStub::destroy() {
	FrameBuffer_close();
}

void MoSyncStub::setPalette(uint8 s, uint8 n, const uint8 *buf) {
	assert(s + n <= 16);
	FrameBuffer_setPalette(s, s*3, n, buf, FLAG_RGB666);
}

void MoSyncStub::copyRect(uint16 x, uint16 y, uint16 w, uint16 h, const uint8 *buf, uint32 pitch) {
	FrameBuffer_copyRect(x, y, w, h, x, y, buf, pitch);
	maResetBacklight();
	maUpdateScreen();
}

void MoSyncStub::handleKeys(int key, int pressed) {
	if(key == MAK_UP || key == MAK_DOWN || key == MAK_LEFT || key == MAK_RIGHT) {
		key = FrameBuffer_getArrowKeyForOrientation(key);
	}

	int dirMask = 0;
	switch(key) {
		case MAK_LEFT: dirMask = PlayerInput::DIR_LEFT; break;
		case MAK_UP: dirMask = PlayerInput::DIR_UP; break;
		case MAK_RIGHT: dirMask = PlayerInput::DIR_RIGHT; break;
		case MAK_DOWN: dirMask = PlayerInput::DIR_DOWN; break;

		case MAK_5:
		case MAK_FIRE: _pi.button = pressed; break;
		case MAK_POUND: if(pressed)_pi.code = true; break;
		case MAK_STAR: if(pressed)_pi.pause = true; break;
		case MAK_SOFTLEFT: if(pressed)_pi.save = true; break;
		case MAK_SOFTRIGHT: if(pressed)_pi.load = true; break;

		case MAK_0: if(pressed)_pi.quit = true; break;

		case MAK_1:
			{
				if(!pressed) return;
				currentOrientation++;
				FrameBuffer_setOrientation(currentOrientation);
			}
			break;
	}

	if(pressed)_pi.dirMask |= dirMask;
	else _pi.dirMask &= ~dirMask;
}

int MoSyncStub::processTimers() {
	return 0;
}

void MoSyncStub::processEvents() {
	MAEvent e;
	while(maGetEvent(&e) != 0) {
		switch(e.type) {
			case EVENT_TYPE_SCREEN_CHANGED: {
				FrameBuffer_close();
				FrameBuffer_init(SCREEN_W, SCREEN_H, currentOrientation, FLAG_4BPP|FLAG_RGB666);
			}
			break;
			case EVENT_TYPE_KEY_PRESSED: handleKeys(e.key, 1); break;
			case EVENT_TYPE_KEY_RELEASED: handleKeys(e.key, 0); break;
			case EVENT_TYPE_CLOSE: _pi.quit = true; break;
			case EVENT_TYPE_POINTER_PRESSED: handleKeys(MAK_FIRE, 1); break;
			case EVENT_TYPE_POINTER_RELEASED: handleKeys(MAK_FIRE, 0); break;
			case EVENT_TYPE_AUDIOBUFFER_FILL:
				uint8 *b = buffer;
				int i = 0;

				while(i<BUFFERSIZE) {
					int numSamplesToNextTick = samplesAtNextTick-(i+curSample);
					int numSamplesToEndOfBuffer = BUFFERSIZE-i;
					if(timerActive&&numSamplesToNextTick<=numSamplesToEndOfBuffer) {
						callback(param, b, numSamplesToNextTick);
						i+=numSamplesToNextTick;
						b+=numSamplesToNextTick;
						timer.delay = timer.callback(timer.delay, timer.param);
						numSamplesToNextTick = (timer.delay*getOutputSampleRate()/1000);
						samplesAtNextTick+=numSamplesToNextTick;
					} else {
						callback(param, b, numSamplesToEndOfBuffer);
						break;
					}
				}

				curSample += BUFFERSIZE;
				maAudioBufferReady();

				break;
		}
	}

	_pi.lastChar = '1';
}

void MoSyncStub::sleep(uint32 duration) {
	uint32 startTime = getTimeStamp();
	do {
		uint32 curTime = getTimeStamp() - startTime;
		if(curTime>=duration) break;
		maWait(duration-curTime);
	} while(true);
}

uint32 MoSyncStub::getTimeStamp() {
	return maGetMilliSecondCount() - timeOfStart;
}

void MoSyncStub::startAudio(AudioCallback callbck, void *prm) {
	this->callback = callbck;
	this->param = prm;

	MAAudioBufferInfo ai;
	ai.buffer = buffer;
	ai.bufferSize = BUFFERSIZE;
	ai.fmt = AUDIO_FMT_S8;
	ai.numChannels = 1;
	ai.sampleRate = SOUND_SAMPLE_RATE;

	maAudioBufferInit(&ai);
}

void MoSyncStub::stopAudio() {
	maAudioBufferClose();
}

uint32 MoSyncStub::getOutputSampleRate() {
	return SOUND_SAMPLE_RATE;
}

// I know only one timer is used...
void *MoSyncStub::addTimer(uint32 delay, TimerCallback callbck, void *prm) {
	timer.callback = callbck;
	timer.delay = delay;
	timer.param = prm;
	timerActive = true;
	samplesAtNextTick = (timer.delay*getOutputSampleRate()/1000);
	curSample = 0;
	return (void*)1;
}

void MoSyncStub::removeTimer(void *timerId) {
	timerActive = false;
	samplesAtNextTick = 0;
}

void *MoSyncStub::createMutex() {
	return NULL;
}

void MoSyncStub::destroyMutex(void *mutex) {
}

void MoSyncStub::lockMutex(void *mutex) {
}

void MoSyncStub::unlockMutex(void *mutex) {
}
