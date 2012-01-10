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
 *         ovidiu
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
	mProgressBar(NULL),
	mProgressBarValue(NULL),
	mSetProgressValueButton(NULL),
	mSetMaximumValueButton(NULL),
	mIncreaseValueButton(NULL)
{
	createMainLayout();

	mSetProgressValueButton->addButtonListener(this);
	mSetMaximumValueButton->addButtonListener(this);
	mIncreaseValueButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSetProgressValueButton->removeButtonListener(this);
    mIncreaseValueButton->removeButtonListener(this);
    mSetMaximumValueButton->removeButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mSetProgressValueButton)
    {
        MAUtil::String stringValue = mEditBox->getText();
        int value = atoi(stringValue.c_str());

        // set and get Progress value
        mProgressBar->setProgress(value);
        mProgressBarValue->setText(MAUtil::integerToString(mProgressBar->getProgress()));

        return;
    }

    if (button == mSetMaximumValueButton)
    {
		MAUtil::String stringValue = mEditBox->getText();
		int value = atoi(stringValue.c_str());

		// set Max value
		mProgressBar->setMaximumValue(value);

		mSetMaximumValueButton->setText("set max (" + stringValue + ")");
        printf("progress max value = %d", value);
        return;
    }

    if (button == mIncreaseValueButton)
    {
		// set IncrementProgress
        mProgressBar->increaseProgress(10);

        mProgressBarValue->setText(MAUtil::integerToString(mProgressBar->getProgress()));
    }
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Label* label = new Label();
	label->setText("Value of the ProgressBar");
	mMainLayout->addChild(label);

	mProgressBarValue = new Label();
	mMainLayout->addChild(mProgressBarValue);

	// the ProgressBar
	mProgressBar = new ProgressBar();
	mProgressBar->fillSpaceHorizontally();
	mMainLayout->addChild(mProgressBar);

	// check the default Progress value
	mProgressBarValue->setText(MAUtil::integerToString(mProgressBar->getProgress()));

	label = new Label();
	label->setText("Use the text box to 'set progress' or 'set max'");
	mMainLayout->addChild(label);

	mEditBox = new EditBox();
	mEditBox->setPlaceholder("your value...");
	mMainLayout->addChild(mEditBox);

	mSetProgressValueButton = new Button();
	mSetProgressValueButton->setText("set progress");
	mMainLayout->addChild(mSetProgressValueButton);

	mSetMaximumValueButton = new Button();
	mSetMaximumValueButton->setText("set max");
	mMainLayout->addChild(mSetMaximumValueButton);

	mIncreaseValueButton = new Button();
	mIncreaseValueButton->setText("increase value with 10");
	mMainLayout->addChild(mIncreaseValueButton);
}
