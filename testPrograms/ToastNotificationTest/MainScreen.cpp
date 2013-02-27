/*
 Copyright (C) 2013 MoSync AB

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
 * @author emma
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>

#include "MainScreen.h"

#define HORIZONTAL_LAYOUT_HEIGHT 80

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mToastButton(NULL),
	mToastLongDuration(NULL),
	mToastMessage(NULL),
	mToastShortDuration(NULL)
{
	createMainLayout();
	mToastButton->addButtonListener(this);
	mToastLongDuration->addCheckBoxListener(this);
	mToastShortDuration->addCheckBoxListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mToastButton->removeButtonListener(this);
	mToastLongDuration->removeCheckBoxListener(this);
	mToastShortDuration->removeCheckBoxListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Label* infoLabel = new Label("Set toast message and duration, and click Display");
	mMainLayout->addChild(infoLabel);

	mToastMessage = new EditBox();
	mToastMessage->setPlaceholder("Enter message here..");
	mMainLayout->addChild(mToastMessage);

	Label* shortDurationLabel = new Label("Short duration");
	mToastShortDuration = new CheckBox();
	mToastShortDuration->setState(true);
	mMainLayout->addChild(shortDurationLabel);
	mMainLayout->addChild(mToastShortDuration);

	Label* longDurationLabel = new Label("Long duration");
	mToastLongDuration = new CheckBox();
	mToastLongDuration->setState(false);
	mMainLayout->addChild(longDurationLabel);
	mMainLayout->addChild(mToastLongDuration);

	mToastButton = new Button();
	mToastButton->setText("Display");
	mToastButton->fillSpaceHorizontally();
	mMainLayout->addChild(mToastButton);
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if ( state && checkBox == mToastShortDuration )
		mToastLongDuration->setState(false);
	else if( state && checkBox == mToastLongDuration )
		mToastShortDuration->setState(false);
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonPressed(Widget* button)
{
	if ( button == mToastButton )
	{
		int duration = (mToastShortDuration->isChecked() ? MA_TOAST_DURATION_SHORT : MA_TOAST_DURATION_LONG);
		maToast(mToastMessage->getText().c_str(), duration);
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if ( button == mToastButton )
	{
		int duration = (mToastShortDuration->isChecked() ? MA_TOAST_DURATION_SHORT : MA_TOAST_DURATION_LONG);
		maToast(mToastMessage->getText().c_str(), duration);
	}
}
