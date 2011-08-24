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

#include "MAHeaders.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mImageButton(NULL)
{
	createMainLayout();
	mImageButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mImageButton->removeButtonListener(this);

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mImageButton = new ImageButton();
	mImageButton->wrapContentHorizontally();
	mMainLayout->addChild(mImageButton);

	int result = mImageButton->setImage(RES_IMAGE);
	mImageButton->setText("click me");
	printf("result setImage = %d", result);

}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * Only for iphone platform
 * @param button The button object that generated the event.
 */
void MainScreen::buttonPressed(Widget* button)
{
    if (button == mImageButton)
    {
        printf("button pressed event for mImageButton");
    }
}

/**
 * This method is called when there is an touch-up event for
 * a button.
 * Only for iphone platform
 * @param button The button object that generated the event.
 */
void MainScreen::buttonReleased(Widget* button)
{
    if (button == mImageButton)
    {
        printf("button released event for mImageButton");
    }
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mImageButton)
    {
        printf("button clicked event for mImageButton");
    }
}
