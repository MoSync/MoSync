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
