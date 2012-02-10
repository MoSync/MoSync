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

#include "ScreenMain.h"
#include "ScreenColorList.h"
#include "ScreenWebView.h"
#include "ScreenImageSwiper.h"
#include "ImageScreen.h"
#include "Util.h"

// Indexes for the tab screens.
#define COLOR_TAB		0
#define WEB_TAB			1
#define SWIPER_TAB		2

/**
 * This is a main screen with three tabs.
 */
class ScreenMainWithThreeTabs :
	public ScreenMain,
	public TabScreenListener
{
public:
	ScreenMainWithThreeTabs() : ScreenMain()
	{
		if(getPlatform() != WINDOWSPHONE7)
		{
			// Create child screens.
			mColorScreen = new ScreenColorList();
			mWebScreen = new ScreenWebView();
			mImageScreen = new ScreenImageSwiper();

			// Add them as tabs.
			this->addTab(mColorScreen);
			this->addTab(mWebScreen);
			this->addTab(mImageScreen);
		}
		else
		{
			mColorScreen = new ScreenColorList();
			mWebScreen = new ScreenWebView();
			mScreenImageShow = new ImageScreenWithButtons();

			// Add them as tabs.
			this->addTab(mColorScreen);
			this->addTab(mWebScreen);
			this->addTab(mScreenImageShow);
		}
	}

	/**
	 * This method is called when there is an event for this widget.
	 * @param widgetEventData The data for the widget event.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (MAW_EVENT_TAB_CHANGED == widgetEventData->eventType)
		{
			// Here we save the current tab index.
			mCurrentTabIndex = widgetEventData->tabIndex;
		}
	}

	/**
	 * This method is called when a key is pressed.
	 */
	void handleKeyPress(int keyCode)
	{
		// The MAK_BACK key code is sent on Android when
		//  the back key is pressed.
		if (MAK_BACK == keyCode)
		{
			// Is this the color screen (first tab)?
			if (COLOR_TAB == mCurrentTabIndex)
			{
				// Exit only if this is the top screen in the stack.
				// If there are more that one screen on the stack,
				// we will instead go back to the previous screen.
				// This behaviour is built into the stack screen
				// widget, and can be turned on/off with the property
				// MAW_STACK_SCREEN_BACK_BUTTON_ENABLED.
				if (mColorScreen->getStackSize() <= 1)
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
	}

	/**
	 * Handle pointer presses.
	 */
	void handlePointerPressed(MAPoint2d point)
	{
		if (SWIPER_TAB == getActiveTabIndex())
		{
			mImageScreen->handlePointerPressed(point);
		}
	}

	/**
	 * Handle pointer moves.
	 */
	void handlePointerMoved(MAPoint2d point)
	{
		if (SWIPER_TAB == getActiveTabIndex())
		{
			mImageScreen->handlePointerMoved(point);
		}
	}

	/**
	 * Handle pointer releases.
	 */
	void handlePointerReleased(MAPoint2d point)
	{
		if (SWIPER_TAB == getActiveTabIndex())
		{
			mImageScreen->handlePointerReleased(point);
		}
	}

    /**
     * This method is called when a tab screen has changed to a new tab.
     * @param tabScreen The tab screen object that generated the event.
     * @param tabScreenIndex The index of the new tab.
     */
    void tabScreenTabChanged(
        TabScreen* tabScreen,
        const int tabScreenIndex)
    {
		// Here we save the current tab index.
		mCurrentTabIndex = tabScreenIndex;
    }

private:
	/**
	 * Index of the currently visible tab.
	 */
	int mCurrentTabIndex;

	/**
	 * The color screen.
	 */
	ScreenColorList* mColorScreen;

	/**
	 * The image screen.
	 */
	ScreenImageSwiper* mImageScreen;

	/**
	 * The web screen.
	 */
	ScreenWebView* mWebScreen;

	 /**
	  * The image view for WP7
	  */
	ImageScreenWithButtons* mScreenImageShow;
};

/**
 * Create the main screen with three tabs.
 */
ScreenMain* ScreenMain::createThreeTabUI()
{
	return new ScreenMainWithThreeTabs();
}

/**
 * Constructor.
 */
ScreenMain::ScreenMain():
	TabScreen()
{
}

/**
 * Destructor.
 */
ScreenMain::~ScreenMain()
{
}
