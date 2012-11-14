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
 * MainScreen.cpp
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"
#include "ListScreen.h"
#include "SettingsScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	TabScreen(),
	mMainLayout(NULL)
{
	createMainLayout();

    this->addTabScreenListener(this);
    this->setActiveTab(0);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
    mListScreen = new ListScreen();
    mListScreen->setTitle("List screen");
    this->addTab(mListScreen);

    mSettingsScreen = new SettingsScreen();
    mSettingsScreen->setTitle("Settings screen");
    mSettingsScreen->setListView(mListScreen->getAlphabeticalListView());
    this->addTab(mSettingsScreen);

    mListScreen->addListScreenListener(mSettingsScreen);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    this->removeTabScreenListener(this);
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
        printf("tab changed : %d", tabScreenIndex);
        printf("getActiveTabIndex() = %d", this->getActiveTabIndex());
    }
}
