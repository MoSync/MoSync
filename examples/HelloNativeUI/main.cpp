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
 * @file main.cpp
 * @author Mattias Frånberg and Chris Hughes
 *
 * This application provides a very basic example of how to work
 * with Native UI to position and and manipulate graphical user
 * interface elements. The code is very well commented so that you
 * can see what's happening at each step. The application uses one
 * screen and a some widgets to provide a password entry box and
 * some control buttons.
 *
 * NOTE: This example application has the same functionality as
 * our example application "HelloMAUI". Compare the two examples
 * to see how we do similar things in MAUI and in Native UI.
 *
 *
 */
#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class
#include <conprint.h>			// lprintfln for logging

#include <NativeUI/Widgets.h>	// Include all widgets

#include "MainScreen.h"			// Main UI screen

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet for the  application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * Constructor that creates the UI.
	 */
	NativeUIMoblet()
	{
		// Create the main user interface screen.
		mMainScreen = new MainScreen();

		// Display the NativeUI screen.
		// Note: This would hide any previously visible screen.
		mMainScreen->show();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete mMainScreen;
	}

	/**
	 * This method is called when the application is closed.
	 */
	void NativeUIMoblet::closeEvent()
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
	void NativeUIMoblet::keyPressEvent(int keyCode, int nativeCode)
	{
	    // Close the application if the back key is pressed.
	    if(MAK_BACK == keyCode)
	    {
	        closeEvent();
	    }
	}

private:
	MainScreen* mMainScreen;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create a moblet.
	NativeUIMoblet* moblet = new NativeUIMoblet();

	// Run the moblet event loop.
	Moblet::run(moblet);

	// Deallocate objects.
	delete moblet;

	return 0;
}
