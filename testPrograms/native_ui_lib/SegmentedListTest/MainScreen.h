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
 * MainScreen.h
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */
#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

// Forward declaration of the ListScreen and SettingsScreen classes
class ListScreen;
class SettingsScreen;

class MainScreen:
	public TabScreen,
	public TabScreenListener
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
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

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
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * The screen containing the alphabetical list.
	 */
	ListScreen *mListScreen;

	/**
	 * The screen containing the alphabetical list settings.
	 */
	SettingsScreen *mSettingsScreen;
};

#endif /* MAINSCREEN_H_ */
