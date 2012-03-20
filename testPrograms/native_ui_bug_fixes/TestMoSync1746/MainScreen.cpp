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

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mLayout(NULL),
	mLabel(NULL)
{
	createMainLayout();

}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x0000FF);
	Screen::setMainWidget(mMainLayout);

	mLabel = new Label();
	mLabel->setText("MainLayout is blue");
	mMainLayout->addChild(mLabel);

	mLayout = new VerticalLayout();
	mLayout->setBackgroundColor(0xEE0000);

	Label* info = new Label();
	info->setText("The child layout is scrollable with multiple labels.");
	mLayout->addChild(info);
	for (int i=0; i < 30; i++)
	{
		Label* label = new Label();
		label->setText("Label " + MAUtil::integerToString(i));
		mLayout->addChild(label);
	}

	mLayout->setScrollable(true);
	mMainLayout->addChild(mLayout);
//	mLayout->setScrollable(true);
}
