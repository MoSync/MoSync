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
	mProgressBar(NULL),
	mSetProgressValueButton(NULL),
	mGetProgressValueButton(NULL),
	mIncreaseValueButton(NULL)
{
	createMainLayout();

	mSetProgressValueButton->addButtonListener(this);
    mGetProgressValueButton->addButtonListener(this);
	mIncreaseValueButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mSetProgressValueButton->removeButtonListener(this);
    mIncreaseValueButton->removeButtonListener(this);
    mGetProgressValueButton->removeButtonListener(this);
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
        mProgressBar->setProgress(value);
    }
    else if (button == mGetProgressValueButton)
    {
        int value = mProgressBar->getProgress();
        printf("progress value = %d", value);
    }
    else if (button == mIncreaseValueButton)
    {
        mProgressBar->increaseProgress(10);
    }
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mProgressBar = new ProgressBar();
	mProgressBar->fillSpaceHorizontally();
	mProgressBar->setMaximumValue(100);
	mMainLayout->addChild(mProgressBar);

	mEditBox = new EditBox();
	mEditBox->setText("30");
	mMainLayout->addChild(mEditBox);

	mSetProgressValueButton = new Button();
	mSetProgressValueButton->setText("set progress");
	mMainLayout->addChild(mSetProgressValueButton);

	mGetProgressValueButton = new Button();
	mGetProgressValueButton->setText("get progress");
	mMainLayout->addChild(mGetProgressValueButton);

	mIncreaseValueButton = new Button();
	mIncreaseValueButton->setText("increase value with 10");
	mMainLayout->addChild(mIncreaseValueButton);
}
