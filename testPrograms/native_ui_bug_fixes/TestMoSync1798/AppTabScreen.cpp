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
 * @file AppTabScreen.cpp
 * @author Bogdan Iusco.
 * @brief Application's tab screen class.
 */

#define FIRST_SCREEN_TITLE "First screen"
#define SECOND_SCREEN_TITLE "Second screen"

#define FIRST_SCREEN_COLOR 0x123456
#define SECOND_SCREEN_COLOR 0xFF0000

#define TAB_SCREEN_TITLE "Tab Screen"

#include "AppTabScreen.h"

/**
 * Constructor.
 */
AppTabScreen::AppTabScreen():
	TabScreen()
{
	this->setTitle(TAB_SCREEN_TITLE);

	this->createFirstScreen();
	this->createSecondScreen();

	this->addTab(mFirstScreen);
	this->addTab(mSecondScreen);
}

/**
 * Destructor.
 */
AppTabScreen::~AppTabScreen()
{

}

/**
 * Create and initialize first screen.
 */
void AppTabScreen::createFirstScreen()
{
	mFirstScreen = new Screen();
	mFirstScreen->setTitle(FIRST_SCREEN_TITLE);
	VerticalLayout* layout = new VerticalLayout();
	layout->setBackgroundColor(FIRST_SCREEN_COLOR);
	mFirstScreen->setMainWidget(layout);
}

/**
 * Create and initialize second screen.
 */
void AppTabScreen::createSecondScreen()
{
	mSecondScreen = new Screen();
	mSecondScreen->setTitle(SECOND_SCREEN_TITLE);
	VerticalLayout* layout = new VerticalLayout();
	layout->setBackgroundColor(SECOND_SCREEN_COLOR);
	mSecondScreen->setMainWidget(layout);
}
