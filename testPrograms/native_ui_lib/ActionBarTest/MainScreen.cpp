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
 * @file MainScreen.cpp
 * @author emma
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	StackScreen(),
	mMainLayout(NULL)
{
    mFirstScreen = new FirstScreen();
    this->push(mFirstScreen);

    addStackScreenListener(this);
}

/**
 * This method is called when a screen has been popped from a stack
 * screen.
 * @param stackScreen The stack screen object that generated the event.
 * @param fromScreen The screen that was popped from the stack screen.
 * @param toScreen The screen that will be shown.
 */
void MainScreen::stackScreenScreenPopped(
    StackScreen* stackScreen,
    Screen* fromScreen,
    Screen* toScreen)
{
}


/**
 * Handle keypresses.
 */
void MainScreen::handleKeyPress(int keyCode)
{
	if (this->getStackSize() <= 1)
	{
		maExit(0);
	}
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	removeStackScreenListener(this);
}
