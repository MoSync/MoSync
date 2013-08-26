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
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * Here the application screens are created and connected to the main tab screen.
 */

#include "Util.h"
#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen():
TabScreen()
{
	getScreenSize();

	mVideoScreen = new VideoScreen();
	mSettingsScreen = new SettingsScreen();

	this->addTab(mVideoScreen);
	this->addTab(mSettingsScreen);

	// At startup display the Video screen.
	this->setActiveTab(0);

	// Load the default url at application startup.
	mVideoScreen->loadDefaultUrl(mSettingsScreen->getDefaultUrl());

	// Add listener for the list view from SettingsView.
	mSettingsScreen->getSettingsListView()->addListViewListener(this);

	addTabScreenListener(this);
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
 * @param listViewSection The section object that contains the selected item.
 * Will be null for default type list views.
 * @param listViewItem The item object that was clicked.
 */
void MainScreen::listViewItemClicked(
	ListView *listView,
	ListViewSection *listViewSection,
	ListViewItem *listViewItem)
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
