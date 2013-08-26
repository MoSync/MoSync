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
	mSearchBar(NULL),
	mDebugLabel(NULL)
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
	mSearchBar->setPlaceholder("search");
	mSearchBar->setBackgroundColor(0x0000FF); //Test setBackgroundColor on SearchBar.
	mMainLayout->addChild(mSearchBar);

    mDebugLabel = new Label();
    mDebugLabel->setText("Listening to events...");
    mDebugLabel->fillSpaceHorizontally();
    mMainLayout->addChild(mDebugLabel);
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
        mDebugLabel->setText("Search button click event received");
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
        mDebugLabel->setText("Cancel button click event received");
        mSearchBar->hideKeyboard();
    }
}
