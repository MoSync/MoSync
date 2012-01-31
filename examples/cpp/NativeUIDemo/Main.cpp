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
 * @author Mikael Kindborg
 *
 * This is the main entry point for the example application
 * that demonstrates NativeUI on MoSync.
 * The application uses the NativeUI library for handling widgets.
 */

#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Moblet.h>		// Moblet class
#include <conprint.h>			// lprintfln for logging

#include <NativeUI/Widgets.h>// Include all widgets

#include "ScreenMain.h"			// Main UI screen
#include "Util.h"

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
		mMainScreen = ScreenMain::createThreeTabUI();

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
	 * This is a callback method declared in class Moblet.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Let the screen handle the keypress.
		mMainScreen->handleKeyPress(keyCode);
	}

	/**
	 * Called when the screen was touched.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		if(getPlatform() != 2) mMainScreen->handlePointerPressed(point);
	}

	/**
	 * Called when the pointer was moved on the screen.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerMoveEvent(MAPoint2d point)
	{
		if(getPlatform() != 2) mMainScreen->handlePointerMoved(point);
	}

	/**
	 * Called when the screen was released.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerReleaseEvent(MAPoint2d point)
	{
		mMainScreen->handlePointerReleased(point);
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
