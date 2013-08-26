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
 * @file SettingsScreen.cpp
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */
#include "SettingsScreen.h"

#include <mastdlib.h>
#include <mavsprintf.h>

#include "WidgetUtil.h"

using namespace NativeUI;

/**
 * This method implements a button event listener.
 * Button click events are sent here.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	if(mFlashModeButton == button)
	{
		if(flashModeIndex < 3)
		{
			flashModeIndex++;
		}
		else
		{
			flashModeIndex = 0;
		}
		char buffer[128];
		sprintf(buffer, "Flash Mode: %s", getModeForIndex(flashModeIndex));
		mFlashModeButton->setText(buffer);
	}
	else if(mSwapCameraButton == button)
	{
		if(currentCamera < numCameras-1)
		{
			currentCamera++;
		}
		else
		{
			currentCamera = 0;
		}

		if(currentCamera == 0)
		{
			mSwapCameraButton->setText("Camera Selected: Back");
		}
		else
		{
			mSwapCameraButton->setText("Camera Selected: Front");
		}
	}
	else if(mOKButton == button)
	{
		mStackScreen->pop();
	}
}

/**
 * Lazy initialization
 */
void SettingsScreen::initialize(StackScreen* stackScreen)
{
	mStackScreen = stackScreen;
	numCameras = maCameraNumber();
	createUI();
}

/**
 * Creates the required view and components
 */
void SettingsScreen::createUI()
{
	// Create a Native UI screen. As the screen is a member
	// variable (also called instance variable) we have
	// prefixed the variable name with "m".
	mScreen = new Screen();

	mScreen->setTitle("Settings");

	// Create the screen's main layout widget.
	mMainLayoutWidget = new VerticalLayout;

	// Make the layout fill the entire screen.
	mMainLayoutWidget->fillSpaceHorizontally();
	mMainLayoutWidget->fillSpaceVertically();

	// Add the layout as the root of the screen's widget tree.
	mScreen->setMainWidget(mMainLayoutWidget);

	mFlashModeButton = new Button();

	mFlashModeButton->fillSpaceHorizontally();
	mFlashModeButton->wrapContentVertically();
	char buffer[128];
	sprintf(buffer, "Flash Mode: %s", MA_CAMERA_FLASH_OFF);
	mFlashModeButton->setText(buffer);
	mFlashModeButton->addButtonListener(this);

	mMainLayoutWidget->addChild(mFlashModeButton);

	mSwapCameraButton = new Button();

	mSwapCameraButton->fillSpaceHorizontally();
	mSwapCameraButton->wrapContentVertically();
	mSwapCameraButton->setText("Camera Selected: Back");
	mSwapCameraButton->addButtonListener(this);

	mMainLayoutWidget->addChild(mSwapCameraButton);

	//Only enable switch camera buttons if we have more than one camera
	if(numCameras == 1)
	{
		mSwapCameraButton->setEnabled(false);
		mSwapCameraButton->setFontColor(0x969696);
	}

	//If the phone does not support flash disable the flash button
	if(flashSupported == false)
	{
		mFlashModeButton->setEnabled(false);
		mFlashModeButton->setFontColor(0x969696);
	}


	//The button that finishes the setting operation
	mOKButton = new Button();

	mOKButton->fillSpaceHorizontally();
	mOKButton->wrapContentVertically();
	mOKButton->setText("Back to Main Screen");
	mOKButton->addButtonListener(this);

	mMainLayoutWidget->addChild(mOKButton);
}


/**
 * pushes the settings screen to the device screen
 */
void SettingsScreen::pushSettingsScreen()
{
	mStackScreen->push(mScreen);
}

/**
 * A wrapper for iterating over flash modes
 */
const char* SettingsScreen::getModeForIndex( int index)
{
	switch(index)
	{
		case 0:
			return MA_CAMERA_FLASH_OFF;
		case 1:
			return MA_CAMERA_FLASH_ON;
		case 2:
			return MA_CAMERA_FLASH_AUTO;
		case 3:
			return MA_CAMERA_FLASH_TORCH;
		default:
			return MA_CAMERA_FLASH_OFF;
	}
}

/**
 * returns the selected camera index
 */
int SettingsScreen::getCurrentCamera()
{
	return currentCamera;
}

/**
 * Returns the selected flash mode
 */
const char * SettingsScreen::getFLashMode()
{
	return getModeForIndex(flashModeIndex);
}
