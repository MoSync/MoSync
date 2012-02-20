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
 * @author emma
 */
#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <mastring.h>
#include <MAUtil/String.h>

#include "MainScreen.h"

using namespace MAUtil;


/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mRatingBar(NULL)
{
	createMainLayout();

	mRatingBar->addRatingBarListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mRatingBar->removeRatingBarListener(this);
}

/**
 * This method is called when the value of the rating bar was changed.
 * @param ratingBar The rating bar object that generated the event.
 * @param value The new value of the rating bar.
 * @param romUser True if the value was changed by the user, false if it was
 * set programmaticaly.
 */
void MainScreen::nratingChanged(
    RatingBar* ratingBar,
    float value,
    bool fromUser)
{

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x220099);
	Screen::setMainWidget(mMainLayout);

	Label* info1 = new Label();
	info1->setText("Pick yoru rating");
	mMainLayout->addChild(info1);

	mRatingBar = new RatingBar();
	mMainLayout->addChild(mRatingBar);
}
