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
 * @file WikiMoblet.h
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 */

#ifndef WIKIMOBLET_H_
#define WIKIMOBLET_H_

// Include the syscalls library.
#include <maapi.h>

// Include the library for the event manager,
// which manages the application main loop.
#include <MAUtil/Moblet.h>

// Include the utility functions.
#include "Util.h"

// The default screen, that is displayed when application starts
#include "HomeScreen.h"

namespace WikiNativeUI
{

/**
 * The Moblet that will manage the application and handle events
 */
class WikiMoblet : public MAUtil::Moblet
{
	/*
	 * ctor
	 */
	WikiMoblet();

public:
	/**
	 * Get the single instance of this Moblet.
	 * @return The Moblet instance.
	 */
	static WikiMoblet *getInstance();

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent() GCCATTRIB(noreturn);

private:
	static WikiMoblet *mInstance;

	/** The main screen of the application. **/
	HomeScreen* mMainScreen;
};

}// namespace WikiNativeUI

#endif /* WIKIMOBLET_H_ */
