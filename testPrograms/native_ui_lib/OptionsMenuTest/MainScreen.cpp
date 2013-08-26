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
	TabScreen(),
	mMainLayout(NULL)
{
    mFirstScreen = new FirstScreen();
    this->addTab(mFirstScreen);

    mSecondScreen = new SecondScreen();
    this->addTab(mSecondScreen);

    mThirdScreen = new ThirdScreen();
    this->addTab(mThirdScreen);

    addTabScreenListener(this);
}

/**
 * This method is called when a tab screen has changed to a new tab.
 * @param tabScreen The tab screen object that generated the event.
 * @param tabScreenIndex The index of the new tab.
 */
void MainScreen::tabScreenTabChanged(
    TabScreen* tabScreen,
    const int tabScreenIndex)
{
	if ( tabScreen == this )
	{
		mCurrentTabIndex = tabScreenIndex;
	}

}

/**
 * Handle keypresses.
 */
void MainScreen::handleKeyPress(int keyCode)
{
	// Is this the third tab?
	if (2 == mCurrentTabIndex)
	{
		// Exit only is this is the top screen in the stack.
		// If there are more that one screen on the stack,
		// we will instead go back to the previous screen.
		// This behaviour is built into the stack screen
		// widget, and can be turned on/off with the property
		// MAW_STACK_SCREEN_BACK_BUTTON_ENABLED.
		if (mThirdScreen->getStackSize() <= 1)
		{
			maExit(0);
		}
	}
	else
	{
		// Otherwise, always exit when back key is pressed.
		maExit(0);
	}
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	removeTabScreenListener(this);
}
