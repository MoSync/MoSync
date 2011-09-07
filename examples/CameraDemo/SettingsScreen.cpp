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



/**
 * This method implements a custom event listener.
 * Widget events are sent as custom events.
 */
void SettingsScreen::customEvent(const MAEvent& event)
{
	// Check if this is a widget event.
	if (EVENT_TYPE_WIDGET == event.type)
	{
		// Get the widget event data structure.
		MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

		// Here we handle clicked events.
		if (MAW_EVENT_CLICKED == eventData->eventType)
		{
			if(mFlashModeButton == eventData->widgetHandle)
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
				maWidgetSetProperty(mFlashModeButton,MAW_BUTTON_TEXT, buffer);
			}
			else if(mSwapCameraButton == eventData->widgetHandle)
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
					maWidgetSetProperty(
							mSwapCameraButton,
							MAW_LABEL_TEXT,
							"Camera Selected: Back");
				}
				else
				{
					maWidgetSetProperty(
							mSwapCameraButton,
							MAW_LABEL_TEXT,
							"Camera Selected: Front");
				}
			}
			else if(mOKButton == eventData->widgetHandle)
			{
				isViewed= false;
				maWidgetStackScreenPop(mStackScreen);
			}
		}
	}
}

/**
 * Lazy initialization
 */
int SettingsScreen::initialize(MAHandle stackScreen, MAHandle previewWidget)
{
	mPreviewWidget = previewWidget;
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
	mScreen = maWidgetCreate(MAW_SCREEN);

	// Create the screen's main layout widget.
	mMainLayoutWidget = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Make the layout fill the entire screen. For properties that
	// take an integer parameter we use the widgetSetPropertyInt
	// function, for properties that takes a string parameter
	// we use the maWidgetSetProperty function.
	widgetSetPropertyInt(
		mMainLayoutWidget,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		mMainLayoutWidget,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);


	// Add the layout as the root of the screen's widget tree.
	maWidgetAddChild(mScreen, mMainLayoutWidget);

	mFlashModeButton = maWidgetCreate(MAW_BUTTON);
	widgetSetPropertyInt(
		mFlashModeButton,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		mFlashModeButton,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_WRAP_CONTENT);
	maWidgetSetProperty(
		mFlashModeButton,
		MAW_LABEL_TEXT,
		"Flash Mode: OFF");
	maWidgetAddChild(mMainLayoutWidget, mFlashModeButton);

	mSwapCameraButton = maWidgetCreate(MAW_BUTTON);
	widgetSetPropertyInt(
		mSwapCameraButton,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		mSwapCameraButton,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_WRAP_CONTENT);
	maWidgetSetProperty(
		mSwapCameraButton,
		MAW_LABEL_TEXT,
		"Camera Selected: Back");
	maWidgetAddChild(mMainLayoutWidget, mSwapCameraButton);

	//Only enable switch camera buttons if we have more than one camera
	if(numCameras == 1)
	{
		maWidgetSetProperty(mSwapCameraButton,
				MAW_WIDGET_ENABLED,
				"false");
	}

	//If the phone does not support flash disable the flash button
	if(flashSupported == false)
	{
		maWidgetSetProperty(mFlashModeButton,
				MAW_WIDGET_ENABLED,
				"false");
	}


	//The button that finishes the setting operation
	mOKButton = maWidgetCreate(MAW_BUTTON);
	widgetSetPropertyInt(
		mOKButton,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		mOKButton,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_WRAP_CONTENT);
	maWidgetSetProperty(
			mOKButton,
		MAW_LABEL_TEXT,
		"Back to Main Screen");
	maWidgetAddChild(mMainLayoutWidget, mOKButton);
}


/**
 * pushes the settings screen to the device screen
 */
void SettingsScreen::pushSettingsScreen()
{
	isViewed = true;
	maWidgetStackScreenPush(mStackScreen, mScreen);
}

/**
 * A wrapper for iterating over flash modes
 */
char* SettingsScreen::getModeForIndex( int index)
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
char * SettingsScreen::getFLashMode()
{
	return getModeForIndex(flashModeIndex);
}
