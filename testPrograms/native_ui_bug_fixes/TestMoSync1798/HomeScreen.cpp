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
 * @file HomeScreen.cpp
 * @author Bogdan Iusco.
 * @brief Application's home screen class.
 * Contain a button that shows the tab screen.
 */

#define SHOW_TAB_SCREEN_BUTTON_TEXT "Show tab screen"
#define SCREEN_TITLE "Home"

#include "HomeScreen.h"
#include "HomeScreenListener.h"
#include "AppTabScreen.h"

/**
 * Contructor.
 * @param listener Listener for this class.
 */
HomeScreen::HomeScreen(HomeScreenListener* listener) :
	Screen(),
	mShowTabScreenButton(NULL),
	mListener(listener)
{
	this->createUI();
	this->setTitle(SCREEN_TITLE);

	mShowTabScreenButton->addButtonListener(this);
}

/**
 * Destructor.
 */
HomeScreen::~HomeScreen()
{
	mShowTabScreenButton->removeButtonListener(this);
}

/**
 * Create screen's widgets.
 */
void HomeScreen::createUI()
{
	VerticalLayout* mainLayout = new VerticalLayout();
	this->setMainWidget(mainLayout);

	mShowTabScreenButton = new Button();
	mShowTabScreenButton->setText(SHOW_TAB_SCREEN_BUTTON_TEXT);
	mShowTabScreenButton->fillSpaceHorizontally();
	mainLayout->addChild(mShowTabScreenButton);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void HomeScreen::buttonClicked(Widget* button)
{
	if (button == mShowTabScreenButton)
	{
		mListener->showTabScreen();
	}
}
