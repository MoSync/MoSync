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

/*
Program: SoundPlay
Author: Mikael Kindborg, MoSync AB

Purpose:

  This app rests that maSoundPlay works, in particular that
  sounds at offset greater than zero in a data resource can
  be played.

How to use:

  Start app.

  When app starts a voice should say "vit".

  Press & release key OR touch screen to play next sound.
  There are in total 2 sounds, a voice saying "mystiskt ljud"
  and a piece of music. These are repeated two times in a
  sequence testing different ways of playing the sound.

  When testing make sure to cycle through the sounds
  at least 4 times (touch screen four times),
  preferably 8 times.

  Press BACK key on Android to exit app.
*/

#include <MAUtil/Moblet.h>
#include <MAUtil/Downloader.h>

#include <conprint.h>

using namespace MAUtil;

// Sound resources, I don't bother with including MAHeaders.
#define SOUND1 1
#define SOUND2 2
#define SOUND3 3

// Keeps track of parameters used for playing sounds.
struct SoundData
{
	int mHandle;
	int mOffset;
	int mLength;
};

class MyMoblet : public Moblet
{
public:
	MyMoblet()
	{
		initializeSoundData();

		// Play sound in first resource.
		printf("Testing maSoundPlay\n");
		printf("You should now hear the word 'vit'\n");

		printf("Playing sound: 1 offset: 0\n");

		maSoundPlay(SOUND1, 0, maGetDataSize(SOUND1));

		printf("Touch screen or press key\n"
			"to play more sounds\n");
		printf("Repeat at least 4 times.\n"
			"You should hear the word 'mystiskt ljud'\n"
			"and music (same sounds repeated)\n");
	}

	virtual ~MyMoblet()
	{
		maDestroyPlaceholder(mAudioData);
	}

	void initializeSoundData()
	{
		// Indexes into mSoundData array.
		mCurrentSound = 0;
		mMaxSound = 3;

		// Allocate multi-sound data object.
		int sizeSound2 = maGetDataSize(SOUND2);
		int sizeSound3 = maGetDataSize(SOUND3);
		int offsetSound2 = 999;
		int offsetSound3 = offsetSound2 + sizeSound2;
		int totalSize = offsetSound3 + sizeSound3;

		mAudioData = maCreatePlaceholder();
		maCreateData(mAudioData, totalSize);

		MACopyData copyParams;

		// Copy first sound (SOUND2).
		copyParams.dst = mAudioData;
		copyParams.dstOffset = offsetSound2;
		copyParams.size = sizeSound2;
		copyParams.src = SOUND2;
		copyParams.srcOffset = 0;
		maCopyData(&copyParams);

		// Copy second sound (SOUND3).
		copyParams.dst = mAudioData;
		copyParams.dstOffset = offsetSound3;
		copyParams.size = sizeSound3;
		copyParams.src = SOUND3;
		copyParams.srcOffset = 0;
		maCopyData(&copyParams);

		// Set sound data parameters.
		mSoundData[0].mHandle = SOUND2;
		mSoundData[0].mOffset = 0;
		mSoundData[0].mLength = maGetDataSize(SOUND2);

		mSoundData[1].mHandle = SOUND3;
		mSoundData[1].mOffset = 0;
		mSoundData[1].mLength = maGetDataSize(SOUND3);

		mSoundData[2].mHandle = mAudioData;
		mSoundData[2].mOffset = offsetSound2;
		mSoundData[2].mLength = sizeSound2;

		mSoundData[3].mHandle = mAudioData;
		mSoundData[3].mOffset = offsetSound3;
		mSoundData[3].mLength = sizeSound3;
	}

	void keyReleaseEvent(int keyCode)
	{
		if (MAK_BACK == keyCode)
		{
			maExit(0);
		}
		else
		{
			playNextSound();
		}
	}

	void pointerReleaseEvent(MAPoint2d p)
	{
		playNextSound();
	}

	void playNextSound()
	{
		if (maSoundIsPlaying())
		{
			maSoundStop();
		}

		if (mCurrentSound > mMaxSound)
		{
			mCurrentSound = 0;
		}

		printf("Playing sound: %i offset: %i\n",
			mSoundData[mCurrentSound].mHandle,
			mSoundData[mCurrentSound].mOffset);

		maSoundPlay(
			mSoundData[mCurrentSound].mHandle,
			mSoundData[mCurrentSound].mOffset,
			mSoundData[mCurrentSound].mLength);

		++mCurrentSound;
	}

private:
	MAHandle mAudioData;
	SoundData mSoundData[4];
	int mCurrentSound;
	int mMaxSound;
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
