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
 * @file MainScreen.h
 * @author Emma Tresanszki
 *
 * The main screen of the application.
 * This is the main entry point of the user interface of the application.
 * Here the application screens are created and connected to the main tab screen.
 *
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
    /** from ListViewListener
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

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
