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

#include "MAHeaders.h"

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
	mMainLayout->setBackgroundColor(0xC1FFC1);
	Screen::setMainWidget(mMainLayout);

	mNavigationBar = new NavigationBar();
	mNavigationBar->setBackButtonTitle("Back");
	mNavigationBar->setTitle("My title");
	mNavigationBar->setTitleFontColor(0x104E8B);
//	mNavigationBar->setTitleFontSize(20);
	mNavigationBar->setIcon(RES_IMAGE);
//	mNavigationBar->fillSpaceHorizontally();
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
