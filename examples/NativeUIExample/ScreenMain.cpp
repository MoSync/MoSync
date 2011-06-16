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
 * @file ScreenMain.cpp
 * @author Mikael Kindborg
 *
 * This is the main entry point of the user interface
 * of the application. Here the application screens
 * are created and connected to the main tab screen.
 */

#include <maprofile.h>
#include "ScreenMain.h"
#include "ScreenColorList.h"
#include "ScreenWebView.h"
#include "ScreenImageSwiper.h"
#include "ScreenSpinningCube.h"
#include "ScreenSettings.h"

/**
 * TODO: Comment this class.
 */
class ScreenMainWithThreeTabs : public ScreenMain
{
public:
	ScreenMainWithThreeTabs() : ScreenMain()
	{
		mColorScreen = ScreenColorList::create();
		Screen* webScreen = ScreenWebView::create();
		Screen* imageScreen = ScreenImageSwiper::create();

		this->addTab(mColorScreen);
		this->addTab(webScreen);
		this->addTab(imageScreen);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * @param widgetEventData The data for the widget event.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (MAW_EVENT_TAB_CHANGED == widgetEventData->eventType)
		{
			mCurrentTabIndex = widgetEventData->tabIndex;
		}
	}

	/**
	 * TODO: Comment this code.
	 */
	void handleKeyPress(int keyCode)
	{
		// The MAK_BACK key code is sent on Android when the back
		// is pressed.
		if (MAK_BACK == keyCode)
		{
			// Is this the color screen (first tab)?
			if (0 == mCurrentTabIndex)
			{
				// Exit only is this is the top screen in the stack.
				// If there are more that one screen on the stack,
				// we will instead go back to tre previous screen.
				// This behaviour is built into the stack screen
				// widget, and can be turned on/off with the property
				// MAW_STACK_SCREEN_BACK_BUTTON_ENABLED.
				if (mColorScreen->getStackSize() <= 1)
				{
					// TODO: Later change to close();
					//close();
					maExit(0);
				}
			}
			else
			{
				// Otherwise, always exit when back key is pressed.
				// TODO: Later change to close();
				//close();
				maExit(0);
			}
		}
	}

private:
	int mCurrentTabIndex;
	StackScreen* mColorScreen;
};

/**
 * TODO: Comment this class.
 */
class ScreenMainWithFourTabs : public ScreenMain
{
public:
	ScreenMainWithFourTabs() : ScreenMain()
	{
		mColorScreen = ScreenColorList::create();
		Screen* webScreen = ScreenWebView::create();
		Screen* imageScreen = ScreenImageSwiper::create();
		Screen* cubeScreen = ScreenSpinningCube::create();
		mSettingsScreen = ScreenSettings::create();

		this->addTab(mColorScreen);
		this->addTab(webScreen);
		this->addTab(imageScreen);
		this->addTab(cubeScreen);
	}

	virtual ~ScreenMainWithFourTabs()
	{
		delete mSettingsScreen;
	}

	void show()
	{
		mVisibleScreen = this;
		ScreenMain::show();
	}

	/**
	 * TODO: Comment this code.
	 */
	void handleKeyPress(int keyCode)
	{
		if (MAK_MENU == keyCode)
		{
			// Show settings screen.
			mVisibleScreen = mSettingsScreen;
			mSettingsScreen->show();
		}
		else if (MAK_BACK == keyCode)
		{
			if (mVisibleScreen == this)
			{
				if (mColorScreen->getStackSize() <= 1)
				{
					// TODO: Later change to close();
					//close();
					maExit(0);
				}
			}
			else if (mVisibleScreen == mSettingsScreen)
			{
				this->show();
			}
		}
	}

private:
	StackScreen* mColorScreen;
	Screen* mSettingsScreen;
	Screen* mVisibleScreen;
};

/**
 * TODO: Comment this class.
 */
class ScreenMainWithFiveTabs : public ScreenMain
{
public:
	ScreenMainWithFiveTabs() : ScreenMain()
	{
		Screen* colorScreen = ScreenColorList::create();
		Screen* webScreen = ScreenWebView::create();
		Screen* imageScreen = ScreenImageSwiper::create();
		Screen* cubeScreen = ScreenSpinningCube::create();
		Screen* settingsScreen = ScreenSettings::create();

		this->addTab(colorScreen);
		this->addTab(webScreen);
		this->addTab(imageScreen);
		this->addTab(cubeScreen);
		this->addTab(settingsScreen);
	}

	virtual ~ScreenMainWithFiveTabs()
	{
	}

	void handleKeyPress(int keyCode)
	{
		if (MAK_BACK == keyCode)
		{
			// TODO: How does this work on iOS?
			maExit(0);
		}
	}
};

/**
 * Create the main screen with three tabs.
 */
ScreenMain* ScreenMain::createThreeTabUI()
{
	return new ScreenMainWithThreeTabs();
}

/**
 * Create the main screen with four tabs.
 */
ScreenMain* ScreenMain::createFourTabUI()
{
	return new ScreenMainWithFourTabs();
}

/**
 * Create the main screen with five tabs.
 */
ScreenMain* ScreenMain::createFiveTabUI()
{
	return new ScreenMainWithFiveTabs();
}


/**
 * Constructor.
 */
ScreenMain::ScreenMain()
{
}

/**
 * Destructor.
 */
ScreenMain::~ScreenMain()
{
}
