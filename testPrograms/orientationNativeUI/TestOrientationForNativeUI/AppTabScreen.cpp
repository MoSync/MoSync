/*
Copyright (C) 2012 MoSync AB

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
 * @file AppTabScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Application's tab screen.
 */

#include <conprint.h>

#include "AppTabScreen.h"
#include "FirstScreen.h"
#include "SecondScreen.h"

namespace OrientationTest
{
	/**
	 * Constructor.
	 */
	AppTabScreen::AppTabScreen():
		TabScreen(),
		mFirstScreen(NULL),
		mSecondScreen(NULL),
		mVisibleTabScreen(TAB_SCREEN_ONE)
	{
		mFirstScreen = new FirstScreen();
		mSecondScreen = new SecondScreen();

		this->addTab(mFirstScreen);
		this->addTab(mSecondScreen);
		this->addTabScreenListener(this);
	}

	/**
	 * Destructor.
	 */
	AppTabScreen::~AppTabScreen()
	{
		this->removeTabScreenListener(this);
	}

	/**
	 * This method is called when a tab screen has changed to a new tab.
	 * @param tabScreen The tab screen object that generated the event.
	 * @param tabScreenIndex The index of the new tab.
	 */
	void AppTabScreen::tabScreenTabChanged(
		TabScreen* tabScreen,
		const int tabScreenIndex)
	{
		if (tabScreenIndex == 0)
		{
			mVisibleTabScreen = TAB_SCREEN_ONE;
		}
		else if (tabScreenIndex == 1)
		{
			mVisibleTabScreen = TAB_SCREEN_TWO;
		}
	}

	/**
	 * Called just before the screen begins rotating.
	 */
	void AppTabScreen::orientationWillChange()
	{
		if (mVisibleTabScreen == TAB_SCREEN_ONE)
		{
			mFirstScreen->orientationWillChange();
		}
		else if (mVisibleTabScreen == TAB_SCREEN_TWO)
		{
			mSecondScreen->orientationWillChange();
		}
	}

	/**
	 * Called after the screen orientation has changed.
	 * Available only on iOS and Windows Phone 7.1 platforms.
	 */
	void AppTabScreen::orientationDidChange()
	{
		if (mVisibleTabScreen == TAB_SCREEN_ONE)
		{
			mFirstScreen->orientationDidChange();
		}
		else if (mVisibleTabScreen == TAB_SCREEN_TWO)
		{
			mSecondScreen->orientationDidChange();
		}
	}

} //namespace OrientationTest
