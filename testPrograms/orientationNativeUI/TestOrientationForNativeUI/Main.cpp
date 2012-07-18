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
 * @file Main.cpp
 * @author Bogdan Iusco
 *
 * This is the main entry point for the application.
 */

#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class

#include "AppTabScreen.h"

using namespace MAUtil;
using namespace OrientationTest;

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
		mTabScreen = new AppTabScreen();

		// Show the screen.
		mTabScreen->show();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete mTabScreen;
	}

	/**
	 * This method is called when the application is closed.
	 */
	void NativeUIMoblet::closeEvent()
	{
		// Deallocate the main screen.
		delete mTabScreen;
		mTabScreen = NULL;

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
	AppTabScreen* mTabScreen;
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
