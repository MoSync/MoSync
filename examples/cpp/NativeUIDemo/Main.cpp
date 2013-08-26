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
		if(getPlatform() != WINDOWSPHONE7) mMainScreen->handlePointerPressed(point);
	}

	/**
	 * Called when the pointer was moved on the screen.
	 * This is a callback method declared in class Moblet.
	 */
	void pointerMoveEvent(MAPoint2d point)
	{
		if(getPlatform() != WINDOWSPHONE7) mMainScreen->handlePointerMoved(point);
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
