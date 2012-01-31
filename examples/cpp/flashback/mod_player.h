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

#ifndef __MOD_PLAYER_H__
#define __MOD_PLAYER_H__

#include "intern.h"

struct File;
struct Mixer;

struct ModPlayer {
	enum {
		NUM_SAMPLES = 31,
		NUM_TRACKS = 4,
		NUM_PATTERNS = 128,
		FRAC_BITS = 12,
		PAULA_FREQ = 3546897
	};

	struct SampleInfo {
		char name[23];
		uint16 len;
		uint8 fineTune;
		uint8 volume;
		uint16 repeatPos;
		uint16 repeatLen;
		int8 *data;

		int8 getPCM(int offset) const {
			if (offset < 0) {
				offset = 0;
			} else if (offset >= (int)len) {
				offset = len - 1;
			}
			return data[offset];
		}
	};

	struct ModuleInfo {
		char songName[21];
		SampleInfo samples[NUM_SAMPLES];
		uint8 numPatterns;
		uint8 patternOrderTable[NUM_PATTERNS];
		uint8 *patternsTable;
	};

	struct Track {
		SampleInfo *sample;
		uint8 volume;
		int pos;
		int freq;
		uint16 period;
		uint16 periodIndex;
		uint16 effectData;
		int vibratoSpeed;
		int vibratoAmp;
		int vibratoPos;
		int portamento;
		int portamentoSpeed;
		int retriggerCounter;
		int delayCounter;
		int cutCounter;
	};

	static const int8 _sineWaveTable[];
	static const uint16 _periodTable[];
	static const char *_modulesFiles[][2];
	static const int _modulesFilesCount;

	ModuleInfo _modInfo;
	uint8 _currentPatternOrder;
	uint8 _currentPatternPos;
	uint8 _currentTick;
	uint8 _songSpeed;
	uint8 _songTempo;
	int _patternDelay;
	int _patternLoopPos;
	int _patternLoopCount;
	int _samplesLeft;
	uint8 _songNum;
	bool _introSongHack;
	bool _playing;
	Track _tracks[NUM_TRACKS];
	Mixer *_mix;
	const char *_dataPath;

	ModPlayer(Mixer *mixer, const char *dataPath);

	uint16 findPeriod(uint16 period, uint8 fineTune) const;
	void load(File *f);
	void unload();
	void play(uint8 num);
	void stop();
	void handleNote(int trackNum, uint32 noteData);
	void handleTick();
	void applyVolumeSlide(int trackNum, int amount);
	void applyVibrato(int trackNum);
	void applyPortamento(int trackNum);
	void handleEffect(int trackNum, bool tick);
	void mixSamples(int8 *buf, int len);
	bool mix(int8 *buf, int len);

	static bool mixCallback(void *param, int8 *buf, int len);
};

#endif // __MOD_PLAYER_H__
