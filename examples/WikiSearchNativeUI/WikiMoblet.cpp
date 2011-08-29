/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file WikiMoblet.cpp
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 *
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
    if(MAK_BACK == keyCode)
    {
        closeEvent();
    }
}

} // namespace WikiNativeUI
