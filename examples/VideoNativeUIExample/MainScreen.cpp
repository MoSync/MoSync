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
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * Here the application screens are created and connected to the main tab screen.
 */

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen():
TabScreen()
{
	mVideoScreen = new VideoScreen();
	mSettingsScreen = new SettingsScreen();

	this->addTab(mVideoScreen);
	this->addTab(mSettingsScreen);

	// At startup display the Video screen.
	this->setActiveTab(0);

	// Load the default url at application startup.
	mVideoScreen->loadDefaultUrl(mSettingsScreen->getDefaultUrl());

	addTabScreenListener(this);

	// Add listener for the list view from SettingsView.
	mSettingsScreen->getSettingsListView()->addListViewListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	removeTabScreenListener(this);
}

/**
 * This method is called when a list view item is clicked.
 * @param listView The list view object that generated the event.
 * @param listViewItem The ListViewItem object that was clicked.
 */
void MainScreen::listViewItemClicked(
	ListView* listView, ListViewItem* listViewItem)
{
	if ( listView == mSettingsScreen->getSettingsListView()
		&&
		listViewItem == mSettingsScreen->getResetListViewItem() )
	{
		// Reload the default source in VideoView.
		mVideoScreen->loadDefaultUrl(mSettingsScreen->getDefaultUrl());

		this->setActiveTab(0);
	}
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
    if (tabScreen == this)
    {
		if ( tabScreenIndex == 0 )
		{
			// Each time the VideoScreen is shown,show or hide the duration
			// label accordingly to the settings screen.
			mVideoScreen->showDurationLabel(
				mSettingsScreen->getDurationSettingValue());
		}
    }
}
