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
 * @file Moblet.cpp
 *
 * This file contains the moblet that manages the application.
 *
 * @author Emma Tresanszki
 */

// Include the header file for this Moblet.
#include "Moblet.h"

namespace RockPaperScissors
{

/**
 * Private constructor.
 */
RockPaperScissorsMoblet::RockPaperScissorsMoblet() : mMainScreen(NULL)
{
	mMainScreen = new PlayScreen();
	mMainScreen->showScreen();
}

/**
 * Get the single instance of this Moblet.
 * Returns a reference to the moblet instance, so that we can access it from outside its class.
 * @return The Moblet instance.
 */
RockPaperScissorsMoblet* RockPaperScissorsMoblet::getInstance()
{
	// A Moblet is a kind of Environment, which is a
	// singleton. Here we cast the singleton instance
	// to the  type of our Moblet class.
	return (RockPaperScissorsMoblet*) &Environment::getEnvironment();
}

/**
 * Method called when a key is pressed.
 */
void RockPaperScissorsMoblet::keyPressEvent(int keyCode, int nativeCode)
{
    // Close the application if the back key is pressed.
    if(MAK_BACK == keyCode)
    {
        closeEvent();
    }
}

/**
 * This method is called when the application is closed.
 */
void RockPaperScissorsMoblet::closeEvent()
{
	// Deallocate the main screen.
	delete mMainScreen;
	mMainScreen = NULL;

	// Exit the app.
	close();
}

} // namespace RockPaperScissors
