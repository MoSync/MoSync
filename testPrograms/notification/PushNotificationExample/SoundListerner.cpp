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

/**
 * @file SoundListener.h
 * @author Bogdan Iusco
 * @date 10 Nov 2011
 *
 * @brief Plays a sound repeatedly.
 */

// Sound length in milliseconds.
#define SOUND_LENGTH 182000

#include "SoundListener.h"

#include <conprint.h>
#include <mastdlib.h>

#include "MAHeaders.h"

/**
 * Constructor.
 */
SoundListener::SoundListener()
{

}

/**
 * Destructor.
 */
SoundListener::~SoundListener()
{
    MAUtil::Environment::getEnvironment().removeTimer(this);
}

/**
 * Start playing sound.
 */
void SoundListener::start()
{
    maSoundPlay(RES_SOUND, 0, maGetDataSize(RES_SOUND));
    MAUtil::Environment::getEnvironment().addTimer(this, SOUND_LENGTH, 0);
}

/**
 * Timer callback method.
 */
void SoundListener::runTimerEvent()
{
    maSoundPlay(RES_SOUND, 0, maGetDataSize(RES_SOUND));
}
