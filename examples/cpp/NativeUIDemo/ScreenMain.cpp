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
