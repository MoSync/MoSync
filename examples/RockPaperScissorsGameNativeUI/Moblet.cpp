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
