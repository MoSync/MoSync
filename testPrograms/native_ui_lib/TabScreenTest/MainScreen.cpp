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
 * @author Bogdan Iusco.
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	TabScreen(),
	mMainLayout(NULL)
{
    Screen* firstScreen = new Screen();
    firstScreen->setTitle("1st screen");
    addMainLayout(firstScreen, 0xFF0000);
    this->addTab(firstScreen);

    Screen* secondScreen = new Screen();
    secondScreen->setTitle("2nd screen");
    addMainLayout(secondScreen, 0x123456);
    this->addTab(secondScreen);

    this->addTabScreenListener(this);
    this->setActiveTab(1);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    this->removeTabScreenListener(this);
	delete mMainLayout;
}

void MainScreen::addMainLayout(Screen* screen, int color)
{
    VerticalLayout* layout = new VerticalLayout();
    layout->setBackgroundColor(color);
    screen->setMainWidget(layout);
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
