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
	StackScreen(),
	mMainLayout(NULL)
{
    mFirstScreen = new FirstScreen();
    this->push(mFirstScreen);

    mSecondScreen = new SecondScreen();
//    this->addTab(mSecondScreen);

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
