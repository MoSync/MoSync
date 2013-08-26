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
 * @file Moblet.h
 *
 * This file contains the moblet that manages the application.
 *
 * @author Emma Tresanszki
 */

#ifndef NATIVEUIMOBLET_H_
#define NATIVEUIMOBLET_H_

#include <maapi.h>

// Include the library for the event manager,
// which manages the application main loop.
#include <MAUtil/Moblet.h>

// Include library for string handling.
#include <MAUtil/String.h>

// The header for the main screen.
#include "PlayScreen.h"

namespace RockPaperScissors
{

/**
 * The Moblet that manages the application and handles events.
 */
class RockPaperScissorsMoblet : public MAUtil::Moblet
{
public:
	/**
	 * Constructor
	 */
	RockPaperScissorsMoblet();

	/**
	 * Get the single instance of this Moblet.
	 * Returns a reference to the moblet instance, so that we can access it from outside its class.
	 * @return The Moblet instance.
	 */
	static RockPaperScissorsMoblet* getInstance();

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent() GCCATTRIB(noreturn);

private:
	/** The screen of the application. **/
	PlayScreen* mMainScreen;
};

} // namespace RockPaperScissors

#endif  /* NATIVEUIMOBLET_H_ */
