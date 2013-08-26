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

		this->addScreenListener(this);
	}

	/**
	 * Destructor.
	 */
	AppTabScreen::~AppTabScreen()
	{
		this->removeTabScreenListener(this);
		this->removeScreenListener(this);
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
	 * Called after the screen orientation has changed.
	 * Available only on iOS and Windows Phone 7.1 platforms.
	 */
	void AppTabScreen::orientationDidChange()
	{
		printf("TabScreen orientationDidChange");
	}

	/**
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 * @param screenOrientation The new screen orientation.
	 */
	void AppTabScreen::orientationChanged(Screen* screen, int screenOrientation)
	{
		printf("Tab screen orientationChanged to %d" + screenOrientation);
	}
} //namespace OrientationTest
