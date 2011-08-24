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
	mSearchBar(NULL)
{
	createMainLayout();

	mSearchBar->addSearchBarListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSearchBar->removeSearchBarListener(this);

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mSearchBar = new SearchBar();
	mSearchBar->fillSpaceHorizontally();
	mMainLayout->addChild(mSearchBar);
}

/**
 * This method is called when the search button is clicked.
 * @param searchBar The search bar object that generated the event.
 */
void MainScreen::searchBarSearchButtonClicked(SearchBar* searchBar)
{
    if (searchBar == mSearchBar)
    {
        printf("search bar - search button clicked");
        mSearchBar->hideKeyboard();
    }
}

/**
 * This method is called when the cancel button is clicked.
 * @param searchBar The search bar object that generated the event.
 */
void MainScreen::searchBarCancelButtonClicked(SearchBar* searchBar)
{
    if (searchBar == mSearchBar)
    {
        printf("search bar - cancel button clicked");
        mSearchBar->hideKeyboard();
    }
}
