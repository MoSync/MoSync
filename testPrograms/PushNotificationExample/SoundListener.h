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
 * @author Bogdan Iusco.
 */

#ifndef SOUNDLISTENER_H_
#define SOUNDLISTENER_H_

#include <MAUtil/Environment.h>
#include <maapi.h>

using namespace MAUtil;

class SoundListener: public TimerListener
{

public:
	/**
	 * Constructor.
	 */
	SoundListener();

	/**
	 * Destructor.
	 */
	~SoundListener();

	/**
	 * Start playing sound.
	 */
	virtual void start();

private:
	virtual void runTimerEvent();
};


#endif /* SOUNDLISTENER_H_ */
