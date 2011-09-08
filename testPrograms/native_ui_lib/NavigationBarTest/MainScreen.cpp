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
	Screen(),
	mMainLayout(NULL),
	mNavigationBar(NULL)
{
	createMainLayout();

	mNavigationBar->addNavigationBarListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mNavigationBar->removeNavigationBarListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mNavigationBar = new NavigationBar();
	mNavigationBar->setBackButtonTitle("Back");
	mNavigationBar->setTitle("My title");
	mNavigationBar->fillSpaceHorizontally();
	mMainLayout->addChild(mNavigationBar);
}

/**
 * This method is called when the back button from the navigation bar
 * was clicked.
 * @param navigationBar The navigation bar object that generated the
 * event.
 */
void MainScreen::navigationBarBackButtonClicked(
    NavigationBar* navigationBar)
{
    if (navigationBar == mNavigationBar)
    {
        printf("mNavigationBar back button pressed.");
    }
}
