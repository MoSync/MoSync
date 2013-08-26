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
	TabScreen(),
	mMainLayout(NULL)
{
    Screen* firstScreen = new Screen();
    firstScreen->setTitle("1st screen");
    addMainLayout(firstScreen, 0x0000FF);
    this->addTab(firstScreen);

    Screen* secondScreen = new Screen();
    addMainLayout(secondScreen, 0xFCD116);
    this->addTab(secondScreen);
    // we set the title after the screen has been added as a
    // tab screen child to check if the property is being set right
    secondScreen->setTitle("2nd screen");

    this->addTabScreenListener(this);
    this->setActiveTab(1);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    this->removeTabScreenListener(this);
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
