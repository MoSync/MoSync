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

class ScreenMainWithFourTabs : public ScreenMain
{
public:
	ScreenMainWithFourTabs() : ScreenMain()
	{
		mTabScreen = new TabScreen();

		Screen* colorScreen = ScreenColorList::create();
		Screen* webScreen = ScreenWebView::create();
		Screen* imageScreen = ScreenImageSwiper::create();
		Screen* cubeScreen = ScreenSpinningCube::create();
		mSettingsScreen = ScreenSettings::create();

		mTabScreen->addTab(colorScreen);
		mTabScreen->addTab(webScreen);
		mTabScreen->addTab(imageScreen);
		mTabScreen->addTab(cubeScreen);
	}

	virtual ~ScreenMainWithFourTabs()
	{
		// TODO: Is this safe?
		delete mTabScreen;
		delete mSettingsScreen;
	}

	void show()
	{
		mVisibleScreen = mTabScreen;
		mTabScreen->show();
	}

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
			if (mVisibleScreen == mTabScreen)
			{
				// TODO: Later change to close();
				//close();
				maExit(0);
			}
			else if (mVisibleScreen == mSettingsScreen)
			{
				show();
			}
		}
	}

private:
	TabScreen* mTabScreen;
	Screen* mSettingsScreen;
	Screen* mVisibleScreen;
};

class ScreenMainWithFiveTabs : public ScreenMain
{
public:
	ScreenMainWithFiveTabs() : ScreenMain()
	{
		mTabScreen = new TabScreen();

		Screen* colorScreen = ScreenColorList::create();
		Screen* webScreen = ScreenWebView::create();
		Screen* imageScreen = ScreenImageSwiper::create();
		Screen* cubeScreen = ScreenSpinningCube::create();
		Screen* settingsScreen = ScreenSettings::create();

		mTabScreen->addTab(colorScreen);
		mTabScreen->addTab(webScreen);
		mTabScreen->addTab(imageScreen);
		mTabScreen->addTab(cubeScreen);
		mTabScreen->addTab(settingsScreen);
	}

	virtual ~ScreenMainWithFiveTabs()
	{
		// TODO: Is this safe?
		delete mTabScreen;
	}

	void show()
	{
		mTabScreen->show();
	}

	void handleKeyPress(int keyCode)
	{
		if (MAK_BACK == keyCode)
		{
			maExit(0);
		}
	}

private:
	TabScreen* mTabScreen;
};

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
