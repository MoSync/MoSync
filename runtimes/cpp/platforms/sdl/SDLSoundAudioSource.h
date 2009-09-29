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

#ifndef _SDL_SOUND_AUDIO_SOURCE_H_
#define _SDL_SOUND_AUDIO_SOURCE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_sound.h>

#include "AudioSource.h"

class SDLSoundAudioSource : public AudioSource {
public:
	SDLSoundAudioSource(SDL_RWops *rwops);
	~SDLSoundAudioSource();

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

	Sound_Sample *mSample;
	SDL_RWops *mRWops;
	Sound_AudioInfo mAudioInfo;

	//Sint16 *mDecodedPtr;
	//int mDecodedSamples;
};

#endif /* _SDL_SOUND_AUDIO_SOURCE_H_ */
