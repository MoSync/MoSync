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

#ifndef _CUSTOMSCREEN_H_
#define _CUSTOMSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>
#include "Clock.h"

using namespace MAUI;

/**
 *  The screen class used for demonstrating how to
 *  do custom widgets in MAUI. It has two instances
 *  of the custom Clock class - one analogue and
 *  one digital.
 */
class CustomScreen : public Screen {
public:
	/**
	 * Constructor
	 * @param previous a pointer to the previous screen, in terms
	 * if navigation, to enable going back in the UI.
	 */
	CustomScreen(Screen *previous);
	/**
	 * Destructor
	 */
	~CustomScreen();
	/**
	 * Handles key presses when this screen is active, supporting
	 * navigating back to the previous screen.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);

private:
	Screen *previous;
	ListBox* listBox;
	Layout* mainLayout;

	Clock *anaClock;
	Clock *digClock;
	Widget *softKeys;
};

#endif	//_CUSTOMSCREEN_H_
