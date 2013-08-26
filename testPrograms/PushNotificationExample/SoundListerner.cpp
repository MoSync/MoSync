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
 * @file SoundListener.cpp
 * @author Bogdan Iusco.
 */
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
	Environment::getEnvironment().removeTimer(this);
}

/**
 * Start playing sound.
 */
void SoundListener::start()
{
	maSoundPlay(RES_SOUND, 0, maGetDataSize(RES_SOUND));
	Environment::getEnvironment().addTimer(this, 182000, 0);
}

void SoundListener::runTimerEvent()
{
	printf("SoundListener::runTimerEvent");
	maSoundPlay(RES_SOUND, 0, maGetDataSize(RES_SOUND));
}
