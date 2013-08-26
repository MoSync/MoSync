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
 * @file ScreenMain.h
 * @author Mikael Kindborg
 *
 * This is the main entry point of the user interface
 * of the application. Here the application screens
 * are created and connected to the main tab screen.
 */

#ifndef SCREEN_MAIN_H_
#define SCREEN_MAIN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * This is the main screen of the application.
 */
class ScreenMain : public TabScreen
{
public:
	/**
	 * Constructor.
	 */
	ScreenMain();

	/**
	 * Destructor.
	 */
	virtual ~ScreenMain();

	/**
	 * Handle keypresses.
	 */
	virtual void handleKeyPress(int keyCode) = 0;

	/**
	 * Handle pointer presses.
	 */
	virtual void handlePointerPressed(MAPoint2d point) = 0;

	/**
	 * Handle pointer moves.
	 */
	virtual void handlePointerMoved(MAPoint2d point) = 0;

	/**
	 * Handle pointer releases.
	 */
	virtual void handlePointerReleased(MAPoint2d point) = 0;

	/**
	 * Create UI with three tabs
	 */
	static ScreenMain* createThreeTabUI();
};

#endif
