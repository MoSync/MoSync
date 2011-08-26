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
	mImage(NULL)
{
	createMainLayout();

}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	delete mMainLayout;
}

/**
 * This method is called when there is an event for this widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
void MainScreen::handleWidgetEvent(Widget* widget,
		MAWidgetEventData* widgetEventData)
{

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	mImage = new Image();
	mMainLayout->addChild(mImage);

	int result = mImage->setImage(RES_IMAGE);
	printf("result setImage = %d", result);
	mImage->fillSpaceHorizontally();
	mImage->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);

}
