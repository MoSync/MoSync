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
 * @file NativeUIExample.cpp
 * @author Mikael Kindborg
 *
 * This is the main entry point for the example application
 * that demonstrates NativeUI on MoSync.
 */

#include <maprofile.h>
#include <ma.h>
#include <mastring.h>
#include <MAUtil/String.h>
#include <MAUtil/Moblet.h>
#include "UIWrapper/Widgets.h"
#include "ScreenMain.h"
#include <conprint.h>

using namespace MAUtil;
using namespace MoSync::UI;

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
		// Create the main user interface.
		if (isAndroid())
		{
			mMainScreen = ScreenMain::createFourTabUI();
		}
		else
		{
			mMainScreen = ScreenMain::createFiveTabUI();
		}

		// Show the screen.
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
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Let the screen handle the keypress.
		mMainScreen->handleKeyPress(keyCode);
	}

	/**
	 * Detects if the current platform is Android.
	 * @return true if the platform is Android.
	 */
	bool isAndroid()
	{
		if (NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	ScreenMain* mMainScreen;
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
