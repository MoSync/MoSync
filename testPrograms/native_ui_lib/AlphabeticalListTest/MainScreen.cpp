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
 * MainScreen.cpp
 *
 *  Created on: Aug 7, 2012
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
