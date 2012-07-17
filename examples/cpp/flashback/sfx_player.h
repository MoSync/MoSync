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

#ifndef __SFX_PLAYER_H__
#define __SFX_PLAYER_H__

#include "intern.h"

struct Mixer;

struct SfxPlayer {
	enum {
		NUM_SAMPLES = 5,
		NUM_CHANNELS = 3,
		FRAC_BITS = 12,
		PAULA_FREQ = 3546897
	};

	struct Module {
		const uint8 *sampleData[NUM_SAMPLES];
		const uint8 *moduleData;
	};

	struct SampleInfo {
		uint16 len;
		uint16 vol;
		uint16 loopPos;
		uint16 loopLen;
		int freq;
		int pos;
		const uint8 *data;

		int8 getPCM(int offset) const {
			if (offset < 0) {
				offset = 0;
			} else if (offset >= (int)len) {
				offset = len - 1;
			}
			return (int8)data[offset];
		}
	};

	static const uint8 _musicData68[];
	static const uint8 _musicData70[];
	static const uint8 _musicData72[];
	static const uint8 _musicData73[];
	static const uint8 _musicData74[];
	static const uint8 _musicData75[];
	static const uint8 _musicDataSample1[];
	static const uint8 _musicDataSample2[]; // tick
	static const uint8 _musicDataSample3[]; // bell
	static const uint8 _musicDataSample4[];
	static const uint8 _musicDataSample5[];
	static const uint8 _musicDataSample6[];
	static const uint8 _musicDataSample7[];
	static const uint8 _musicDataSample8[];
	static const Module _module68;
	static const Module _module70;
	static const Module _module72;
	static const Module _module73;
	static const Module _module74;
	static const Module _module75;
	static const uint16 _periodTable[];

	const Module *_mod;
	bool _playing;
	int _samplesLeft;
	uint16 _curOrder;
	uint16 _numOrders;
	uint16 _orderDelay;
	const uint8 *_modData;
	SampleInfo _samples[NUM_CHANNELS];
	Mixer *_mix;

	SfxPlayer(Mixer *mixer);

	void play(uint8 num);
	void stop();
	void playSample(int channel, const uint8 *sampleData, uint16 period);
	void handleTick();
	bool mix(int8 *buf, int len);
	void mixSamples(int8 *buf, int samplesLen);

	static bool mixCallback(void *param, int8 *buf, int len);
};

#endif // __SFX_PLAYER_H__
