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
 * Constructor.
 */
SettingsScreen::SettingsScreen()
{
	mCurrentCamera = 0;
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	delete mScreen;
}

/**
 * This method implements a button event listener.
 * Button click events are sent here.
 */
void SettingsScreen::buttonClicked(Widget* button)
{
	if (mFlashModeButton == button)
	{
		if (mFlashModeIndex < 3)
		{
			mFlashModeIndex++;
		}
		else
		{
			mFlashModeIndex = 0;
		}
		char buffer[128];
		sprintf(buffer, "Flash Mode: %s", getModeForIndex(mFlashModeIndex));
		mFlashModeButton->setText(buffer);
	}
	else if (mSwapCameraButton == button)
	{
		if (mCurrentCamera < mNumCameras - 1)
		{
			mCurrentCamera++;
		}
		else
		{
			mCurrentCamera = 0;
		}

		if (mCurrentCamera == 0)
		{
			// On devices like Nexus 7, with only one camera that
			// is facing front, this is not true.
			mSwapCameraButton->setText("Camera Selected: Back");
		}
		else
		{
			mSwapCameraButton->setText("Camera Selected: Front");
		}
	}
	else if (mOKButton == button)
	{
		// Hide settings screen.
		mStackScreen->pop();
	}
}

/**
 * Initialize the screen.
 */
void SettingsScreen::initializeUI(StackScreen* stackScreen)
{
	mStackScreen = stackScreen;
	mNumCameras = maCameraNumber();
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

	// Create a button for the flash mode.
	mFlashModeButton = new Button();
	mFlashModeButton->fillSpaceHorizontally();
	mFlashModeButton->wrapContentVertically();
	char buffer[128];
	sprintf(buffer, "Flash Mode: %s", MA_CAMERA_FLASH_OFF);
	mFlashModeButton->setText(buffer);
	mFlashModeButton->addButtonListener(this);
	mMainLayoutWidget->addChild(mFlashModeButton);

	// Create button to switch camera.
	mSwapCameraButton = new Button();
	mSwapCameraButton->fillSpaceHorizontally();
	mSwapCameraButton->wrapContentVertically();
	mSwapCameraButton->setText("Camera Selected: Back");
	mSwapCameraButton->addButtonListener(this);
	mMainLayoutWidget->addChild(mSwapCameraButton);

	// Only enable switch camera button if we have
	// more than one camera.
	if (mNumCameras == 1)
	{
		mSwapCameraButton->setEnabled(false);
		mSwapCameraButton->setFontColor(0x969696);
	}

	// This button exits the settigs screen.
	mOKButton = new Button();
	mOKButton->fillSpaceHorizontally();
	mOKButton->wrapContentVertically();
	mOKButton->setText("Back to Main Screen");
	mOKButton->addButtonListener(this);
	mMainLayoutWidget->addChild(mOKButton);
}

/**
 * Set camera properties.
 */
void SettingsScreen::setFlashSupported(bool flashSupported)
{
	mFlashSupported = flashSupported;

	// If the current camera does not support flash
	// we disable the flash button.
	if (!mFlashSupported)
	{
		mFlashModeButton->setEnabled(false);
		mFlashModeButton->setFontColor(0x969696);
	}
	else
	{
		mFlashModeButton->setEnabled(true);
		mFlashModeButton->setFontColor(0x000000);
	}
}

/**
 * Pushes the settings screen to display in front.
 */
void SettingsScreen::pushSettingsScreen()
{
	mStackScreen->push(mScreen);
}

/**
 * A wrapper for iterating over flash modes
 */
const char* SettingsScreen::getModeForIndex(int index)
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
 * Returns the selected camera index.
 */
int SettingsScreen::getCurrentCamera()
{
	return mCurrentCamera;
}

/**
 * Returns the selected camera index.
 */
void SettingsScreen::setCurrentCamera(int cameraIndex)
{
	mCurrentCamera = cameraIndex;
}

/**
 * Returns the selected flash mode
 */
const char* SettingsScreen::getFlashMode()
{
	return getModeForIndex(mFlashModeIndex);
}
