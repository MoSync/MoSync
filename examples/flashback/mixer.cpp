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

#include "mixer.h"
#include "systemstub.h"


Mixer::Mixer(SystemStub *stub)
	: _stub(stub) {
}

void Mixer::init() {
	memset(_channels, 0, sizeof(_channels));
	_premixHook = 0;
	_mutex = _stub->createMutex();
	_stub->startAudio(Mixer::mixCallback, this);
}

void Mixer::free() {
	stopAll();
	_stub->stopAudio();
	_stub->destroyMutex(_mutex);
}

void Mixer::setPremixHook(PremixHook premixHook, void *userData) {
	debug(DBG_SND, "Mixer::setPremixHook()");
	MutexStack(_stub, _mutex);
	_premixHook = premixHook;
	_premixHookData = userData;
}

void Mixer::play(const MixerChunk *mc, uint16 freq, uint8 volume) {
	debug(DBG_SND, "Mixer::play(%d, %d)", freq, volume);
	MutexStack(_stub, _mutex);
	MixerChannel *ch = 0;
	for (int i = 0; i < NUM_CHANNELS; ++i) {
		MixerChannel *cur = &_channels[i];
		if (cur->active) {
			if (cur->chunk.data == mc->data) {
				cur->chunkPos = 0;
				return;
			}
		} else {
			ch = cur;
			break;
		}
	}
	if (ch) {
		ch->active = true;
		ch->volume = volume;
		ch->chunk = *mc;
		ch->chunkPos = 0;
		ch->chunkInc = (freq << FRAC_BITS) / _stub->getOutputSampleRate();
	}
}

uint32 Mixer::getSampleRate() const {
	return _stub->getOutputSampleRate();
}

void Mixer::stopAll() {
	debug(DBG_SND, "Mixer::stopAll()");
	MutexStack(_stub, _mutex);
	for (uint8 i = 0; i < NUM_CHANNELS; ++i) {
		_channels[i].active = false;
	}
}

void Mixer::mix(int8 *buf, int len) {
	MutexStack(_stub, _mutex);
	memset(buf, 0, len);
	if (_premixHook) {
		if (!_premixHook(_premixHookData, buf, len)) {
			_premixHook = 0;
			_premixHookData = 0;
		}
	}
	for (uint8 i = 0; i < NUM_CHANNELS; ++i) {
		MixerChannel *ch = &_channels[i];
		if (ch->active) {
			for (int pos = 0; pos < len; ++pos) {
				if ((ch->chunkPos >> FRAC_BITS) >= (ch->chunk.len - 1)) {
					ch->active = false;
					break;
				}
				int out = resampleLinear(&ch->chunk, ch->chunkPos, ch->chunkInc, FRAC_BITS);
				addclamp(buf[pos], out * ch->volume / Mixer::MAX_VOLUME);
				ch->chunkPos += ch->chunkInc;
			}
		}
	}
}

void Mixer::addclamp(int8& a, int b) {
	int add = a + b;
	if (add < -128) {
		add = -128;
	} else if (add > 127) {
		add = 127;
	}
	a = add;
}

void Mixer::mixCallback(void *param, uint8 *buf, int len) {
	((Mixer *)param)->mix((int8 *)buf, len);
}
