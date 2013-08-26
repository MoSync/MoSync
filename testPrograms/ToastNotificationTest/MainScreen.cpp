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
	mToastShortDuration->setState(true);
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
	mMainLayout->setBackgroundColor(0x4400FF);

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
	if (state)
	{
		if (checkBox == mToastShortDuration)
			mToastLongDuration->setState(false);
		else
			mToastShortDuration->setState(false);
	}
	else
	{
		if ( !mToastLongDuration->isChecked() &&
				!mToastShortDuration->isChecked())
		{
			mToastShortDuration->setState(true);
		}
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android, Windows Phone.
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
