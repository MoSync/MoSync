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
 * @author Emma Tresanszki.
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
	mCameraWidget(NULL),
	mStartCamera(NULL),
	mStopCamera(NULL)
{
	createMainLayout();

	mStartCamera->addButtonListener(this);
	mStopCamera->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mStartCamera->removeButtonListener(this);
	mStopCamera->removeButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mStartCamera)
    {
		maCameraStart();
    }
    else if (button == mStopCamera)
    {
        maCameraStop();
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

	Label* nrCameras = new Label();
	nrCameras->setText(MAUtil::integerToString(maCameraNumber()) +" cameras");
	mMainLayout->addChild(nrCameras);

	if ( maCameraNumber() > 0)
	{
		// Select first camera
		maCameraSelect(0);
		mCameraWidget = new CameraPreview();
		maCameraSetPreview(mCameraWidget->getWidgetHandle());
		mMainLayout->addChild(mCameraWidget);

		mStartCamera = new Button();
		mStartCamera->setText("Start");
		mMainLayout->addChild(mStartCamera);

		mStopCamera = new Button();
		mStopCamera->setText("Stop");
		mMainLayout->addChild(mStopCamera);
	}
	else
	{
		maPanic(0,"No camera found");
	}

}
