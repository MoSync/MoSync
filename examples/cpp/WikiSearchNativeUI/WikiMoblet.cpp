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
 * @file WikiMoblet.cpp
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 */

// The header file of the class.
#include "WikiMoblet.h"

namespace WikiNativeUI
{

WikiMoblet *WikiMoblet::mInstance = NULL;

/*
 * ctor
 */
WikiMoblet::WikiMoblet():mMainScreen(NULL)
{
	mMainScreen = new HomeScreen();
	mMainScreen->showScreen();
}

/**
 * Get the single instance of this Moblet.
 * @return The Moblet instance.
 */
WikiMoblet *WikiMoblet::getInstance()
{
	if( NULL == mInstance ) {
		mInstance = new WikiMoblet();
	}

	return mInstance;
}

/**
 * This method is called when the application is closed.
 */
void WikiMoblet::closeEvent()
{
	// Deallocate the main screen.
	delete mMainScreen;
	mMainScreen = NULL;

	// Exit the app.
	close();
}

/**
 * Method called when a key is pressed.
 */
void WikiMoblet::keyPressEvent(int keyCode, int nativeCode)
{
    // Close the application if the back key is pressed.
//    if(MAK_BACK == keyCode)
//    {
//        closeEvent();
//    }
}

} // namespace WikiNativeUI
