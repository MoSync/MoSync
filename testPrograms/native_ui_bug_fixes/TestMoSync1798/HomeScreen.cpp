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
