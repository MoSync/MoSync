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
		mCameraWidget->bindToCurrentCamera();
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
