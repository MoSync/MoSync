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
	mActivityIndicator(NULL),
	mShow(NULL),
	mHide(NULL)
{
	createMainLayout();

	mShow->addButtonListener(this);
    mHide->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mShow->removeButtonListener(this);
    mHide->removeButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mShow)
    {
        mActivityIndicator->show();
    }
    else if (button == mHide)
    {
        mActivityIndicator->hide();
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
	label->setText("Show/hide Activity Indicator");
	mMainLayout->addChild(label);

	mShow = new Button();
	mShow->setText("show ActivityIndicator");
	mMainLayout->addChild(mShow);

	mActivityIndicator = new ActivityIndicator();
	mMainLayout->addChild(mActivityIndicator);

	mHide = new Button();
	mHide->setText("hide ActivityIndicator");
	mMainLayout->addChild(mHide);

}
