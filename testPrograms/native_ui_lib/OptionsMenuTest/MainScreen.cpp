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
