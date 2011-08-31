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
 * @author Emma Tresanszki
 *
 * This file contains the moblet that manages the application.
 */

#include <MAUtil/String.h>

// The header file of the class.
#include "Moblet.h"
#include "maapi.h"
#include <maxtoa.h>

namespace Test
{

VideoMoblet *VideoMoblet::mInstance = NULL;

/**
 * Constructor.
 */
VideoMoblet::VideoMoblet():mMainScreen(NULL)
{
	mMainScreen = new MainScreen();
	mMainScreen->show();

}

/**
 * Destructor.
 */
VideoMoblet::~VideoMoblet()
{
	delete mMainScreen;
}

/**
 * Get the single instance of this Moblet.
 * @return The Moblet instance.
 */
VideoMoblet *VideoMoblet::getInstance()
{
	if( NULL == mInstance ) {
		mInstance = new VideoMoblet();
	}

	return mInstance;
}

/**
 * This method is called when the application is closed.
 */
void VideoMoblet::closeEvent()
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
void VideoMoblet::keyPressEvent(int keyCode, int nativeCode)
{
    // Close the application if the back key is pressed.
    if(MAK_BACK == keyCode)
    {
        closeEvent();
    }
}

} // namespace Test
