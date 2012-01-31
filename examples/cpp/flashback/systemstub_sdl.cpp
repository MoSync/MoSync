/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2007 Gregory Montoir
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <ma.h>
#include "scaler.h"
#include "systemstub.h"

#include <MAUtil/FrameBuffer.h>
#include <IX_AUDIOBUFFER.h>

struct MoSyncStub : SystemStub {
	enum {
		MAX_BLIT_RECTS = 200,
		SOUND_SAMPLE_RATE = 11025,
		JOYSTICK_COMMIT_VALUE = 3200
	};

	uint8 *_offscreen;
	uint8 _overscanColor;
	int orientation;
	uint16 _screenW, _screenH;
	MARect _blitRects[MAX_BLIT_RECTS];
	uint16 _numBlitRects;

#define BUFFERSIZE (2048)
	unsigned char buffer[BUFFERSIZE];
	AudioCallback audioCallback;
	void *audioParam;

	virtual ~MoSyncStub() {}
	virtual void init(const char *title, uint16 w, uint16 h);
	virtual void destroy();
	virtual void setPalette(const uint8 *pal, uint16 n);
	virtual void setPaletteEntry(uint8 i, const Color *c);
	virtual void getPaletteEntry(uint8 i, Color *c);
	virtual void setOverscanColor(uint8 i);
	virtual void copyRect(int16 x, int16 y, uint16 w, uint16 h, const uint8 *buf, uint32 pitch);
	virtual void updateScreen(uint8 shakeOffset);
	virtual void processEvents();
	virtual void sleep(uint32 duration);
	virtual uint32 getTimeStamp();
	virtual void startAudio(AudioCallback callback, void *param);
	virtual void stopAudio();
	virtual uint32 getOutputSampleRate();
	virtual void *createMutex();
	virtual void destroyMutex(void *mutex);
	virtual void lockMutex(void *mutex);
	virtual void unlockMutex(void *mutex);

	void prepareGfxMode();
	void cleanupGfxMode();
	void switchGfxMode(bool fullscreen, uint8 scaler);
	void forceGfxRedraw();
	void drawRect(MARect *rect, uint8 color);

	void blitRect(MARect *br, int dx, int dy);
};

SystemStub *MoSyncStub_create() {
	return new MoSyncStub();
}


void MoSyncStub::init(const char *title, uint16 w, uint16 h) {
	memset(&_pi, 0, sizeof(_pi));
	_screenW = w;
	_screenH = h;
	_numBlitRects = 0;

	_offscreen = 0;
	cleanupGfxMode();
	int size_offscreen = (w + 2) * (h + 2);
	_offscreen = (uint8 *)malloc(size_offscreen);
	if (!_offscreen) {
		error("SystemStub_SDL::init() Unable to allocate offscreen buffer");
	}

	memset(_offscreen, 0, size_offscreen);

	orientation = ORIENTATION_0;
	FrameBuffer_init(w, h, orientation, FLAG_RGB666);
	audioCallback = 0;
}

void MoSyncStub::destroy() {
	FrameBuffer_close();
}

void MoSyncStub::setPalette(const uint8 *pal, uint16 n) {
	assert(n <= 256);
	FrameBuffer_setPalette(0, 0, n, pal, 0);
}

void MoSyncStub::setPaletteEntry(uint8 i, const Color *c) {
	FrameBuffer_setPaletteEntry(i, c->r, c->g, c->b, FLAG_RGB666);
}

void MoSyncStub::getPaletteEntry(uint8 i, Color *c) {

	byte cr[3];
	FrameBuffer_getPaletteEntry(i, cr, FLAG_RGB666);
	c->r = cr[0];
	c->g = cr[1];
	c->b = cr[2];
}

void MoSyncStub::setOverscanColor(uint8 i) {
	_overscanColor = i;
}

void MoSyncStub::copyRect(int16 x, int16 y, uint16 w, uint16 h, const uint8 *buf, uint32 pitch) {
	if (_numBlitRects >= MAX_BLIT_RECTS) {
		warning("SystemStub_SDL::copyRect() Too many blit rects, you may experience graphical glitches");
	} else {
		// extend the dirty region by 1 pixel for scalers accessing 'outer' pixels
//		--x;
//		--y;
//		w += 2;
//		h += 2;

		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}
		if (x + w > _screenW) {
			w = _screenW - x;
		}
		if (y + h > _screenH) {
			h = _screenH - y;
		}

		MARect *br = &_blitRects[_numBlitRects];

		br->left = _pi.mirrorMode ? _screenW - (x + w) : x;
		br->top = y;
		br->width = w;
		br->height = h;
		++_numBlitRects;

		uint8 *p = (uint8 *)_offscreen + (br->top ) * _screenW + (br->left);
		buf += y * pitch + x;

		if (_pi.mirrorMode) {
			while (h--) {
				for (int i = 0; i < w; ++i) {
					p[i] = buf[w - 1 - i];
				}
				p += _screenW;
				buf += pitch;
			}
		} else {
			while (h--) {
				for (int i = 0; i < w; ++i) {
					p[i] = buf[i];
				}
				p += _screenW;
				buf += pitch;
			}
		}

		if (_pi.dbgMask & PlayerInput::DF_DBLOCKS) {
			drawRect(br, 0xE7);
		}
	}
}

void MoSyncStub::blitRect(MARect *br, int dx, int dy) {
	int x = br->left;
	int y = br->top;
	int w = br->width;
	int h = br->height;

	FrameBuffer_copyRect(x, y, w, h, dx, dy, _offscreen, _screenW);
}

void setMoSyncColor(int i) {
	Color c;
	FrameBuffer_getPaletteEntry(i, (byte*)&c, FLAG_RGB666);
	c.r<<=2; c.g<<=2; c.b<<=2;
	maSetColor((c.r<<16) | (c.g<<8) | (c.b));
}

void MoSyncStub::updateScreen(uint8 shakeOffset) {
	if (shakeOffset == 0) {
		for (int i = 0; i < _numBlitRects; ++i) {
			blitRect(&_blitRects[i], _blitRects[i].left, _blitRects[i].top);
		}
	} else {

		MARect bsr, bdr;
		bdr.left = 0;
		bdr.top = 0;
		bdr.width = _screenW;
		bdr.height = shakeOffset;

		bsr.left = 0;
		bsr.top = 0;
		bsr.width = _screenW;
		bsr.height = (_screenH - shakeOffset);
		blitRect(&bsr, 0, shakeOffset);
	}
	_numBlitRects = 0;

	maUpdateScreen();
	maResetBacklight();
}

void MoSyncStub::processEvents() {
	MAEvent ev;
	while (maGetEvent(&ev)) {
		switch (ev.type) {
		case EVENT_TYPE_CLOSE:
			_pi.quit = true;
			maExit(0);
			break;
		case EVENT_TYPE_KEY_RELEASED:
			if(ev.key == MAK_UP || ev.key == MAK_DOWN || ev.key == MAK_LEFT || ev.key == MAK_RIGHT) {
				ev.key = FrameBuffer_getArrowKeyForOrientation(ev.key);
			}
			switch (ev.key) {
			case MAK_LEFT:
				_pi.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case MAK_RIGHT:
				_pi.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case MAK_UP:
				_pi.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case MAK_DOWN:
				_pi.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case MAK_SOFTLEFT:
				_pi.space = false;
				break;
			case MAK_SOFTRIGHT:
				_pi.shift = false;
				break;
			case MAK_FIRE:
				_pi.enter = false;
				break;
			case MAK_POUND:
				_pi.escape = false;
				break;
			case MAK_4:
				orientation++;
				FrameBuffer_setOrientation(orientation);
				forceGfxRedraw();
				updateScreen(0);
				break;
			default:
				break;
			}
			break;
		case EVENT_TYPE_KEY_PRESSED:
			if(ev.key == MAK_UP || ev.key == MAK_DOWN || ev.key == MAK_LEFT || ev.key == MAK_RIGHT) {
				ev.key = FrameBuffer_getArrowKeyForOrientation(ev.key);
			}

			_pi.lastChar = ev.key;
			switch (ev.key) {
			case MAK_1:
					_pi.save = true;
				break;
			case MAK_3:
					_pi.load = true;
				break;
			case MAK_LEFT:
				_pi.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case MAK_RIGHT:
				_pi.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case MAK_UP:
				_pi.dirMask |= PlayerInput::DIR_UP;
				break;
			case MAK_DOWN:
				_pi.dirMask |= PlayerInput::DIR_DOWN;
				break;

			case MAK_STAR:
				_pi.backspace = true;
				break;

			case MAK_SOFTLEFT:
				_pi.space = true;
				break;
			case MAK_SOFTRIGHT:
				_pi.shift = true;
				break;
			case MAK_FIRE:
				_pi.enter = true;
				break;
			case MAK_POUND:
				_pi.escape = true;
				break;
			default:
				break;
			}
			break;
		case EVENT_TYPE_AUDIOBUFFER_FILL:
		{
			uint8 *b = buffer;
			if(audioCallback) {
				audioCallback(audioParam, buffer, BUFFERSIZE);
			}
			maAudioBufferReady();
		}
			break;
		default:
			break;
		}
	}
}

void MoSyncStub::sleep(uint32 duration) {
	int startTime = maGetMilliSecondCount();
	int dur = (int)duration;
	do {
		int curTime = maGetMilliSecondCount() - startTime;
		if(curTime>=dur) break;
		maWait(dur-curTime);
	} while(true);

	processEvents();
}

uint32 MoSyncStub::getTimeStamp() {
	return maGetMilliSecondCount();
}

void MoSyncStub::startAudio(AudioCallback callback, void *param) {
	audioCallback = callback;
	audioParam = param;

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

void *MoSyncStub::createMutex() {
	return NULL;
}

void MoSyncStub::destroyMutex(void *mutex) {
}

void MoSyncStub::lockMutex(void *mutex) {
}

void MoSyncStub::unlockMutex(void *mutex) {
}

void MoSyncStub::prepareGfxMode() {
	forceGfxRedraw();
}

void MoSyncStub::cleanupGfxMode() {
	if (_offscreen) {
		free(_offscreen);
		_offscreen = 0;
	}

	maFrameBufferClose();
}

void MoSyncStub::switchGfxMode(bool fullscreen, uint8 scaler) {
}

void MoSyncStub::forceGfxRedraw() {
	_numBlitRects = 1;
	_blitRects[0].left = 0;
	_blitRects[0].top = 0;
	_blitRects[0].width = _screenW;
	_blitRects[0].height = _screenH;
}

void MoSyncStub::drawRect(MARect *rect, uint8 color) {
	int x1 = rect->left;
	int y1 = rect->top;
	int x2 = rect->left + rect->width - 1;
	int y2 = rect->top + rect->height - 1;
	assert(x1 >= 0 && x2 < _screenW && y1 >= 0 && y2 < _screenH);

	setMoSyncColor(color);
	maLine(x1, y1, x2, y1);
	maLine(x2, y1, x2, y2);
	maLine(x2, y2, x1, y2);
	maLine(x1, y2, x1, y1);
}
