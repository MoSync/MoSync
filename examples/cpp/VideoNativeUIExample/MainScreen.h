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
 * @file MainScreen.h
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * Here the application screens are created and connected to the main tab screen.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include "SettingsScreen.h"
#include "VideoScreen.h"

using namespace NativeUI;

class MainScreen :
	public TabScreen,
	public TabScreenListener,
	public ListViewListener
{
public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:
	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewSection The section object that contains the selected item.
	 * Will be null for default type list views.
	 * @param listViewItem The item object that was clicked.
	 */
	virtual void listViewItemClicked(
		ListView *listView,
		ListViewSection *listViewSection,
		ListViewItem *listViewItem);

    /**
     * This method is called when a tab screen has changed to a new tab.
     * @param tabScreen The tab screen object that generated the event.
     * @param tabScreenIndex The index of the new tab.
     */
    virtual void tabScreenTabChanged(
        TabScreen* tabScreen,
        const int tabScreenIndex);

private:
	/**
	 * Index of the currently visible tab.
	 */
	int mCurrentTabIndex;

	/**
	 * The video screen.
	 */
	VideoScreen* mVideoScreen;

	/**
	 * The settings screen.
	 */
	SettingsScreen* mSettingsScreen;

};

#endif /* MAINSCREEN_H_ */
