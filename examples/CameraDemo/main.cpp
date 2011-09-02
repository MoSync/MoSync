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
 * @file main.cpp
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */

// Include MoSync syscalls.
#include <maapi.h>
#include <mastdlib.h>
// Include NativeUI extensions.
#include <IX_WIDGET.h>

// Include header file for Moblets.
#include <MAUtil/Moblet.h>

// Include widget utility functions. These functions simplify
// getting and setting widget properties.
#include "WidgetUtil.h"

#include <MAUtil/PlaceholderPool.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include "SettingsScreen.h"
#include "ImageScreen.h"


using namespace MAUtil;
/**
 * Class that wraps a NativeUI screen widget. We implement
 * the CustomEventListener interface to be able to process
 * widget events.
 */
class AppScreen : public MAUtil::CustomEventListener
{
public:
	/**
	 * In the constructor, we create the user interface.
	 */
	AppScreen()
	{
		mLastEnc = 0;

		mCurrentZoomIndex = 0;
		createUI();

	}


	/**
	 * In the destructor, we destroy the widgets to release memory.
	 * Destroying a widget automatically destroys all child widgets.
	 */
	virtual ~AppScreen()
	{
		// Destroying the screen widget destroys all of its children.
		maWidgetDestroy(mScreen);
	}

	/**
	 * Here we create the user interface widgets.
	 */
	void createUI()
	{
		// Make the AppScreen listen for events coming from widgets.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);
		char buffer[256];
		int length = maCameraGetProperty(MA_CAMERA_MAX_ZOOM, buffer, 256);
		maxZoom = atoi(buffer);
		createMainScreen();
		mStackScreen = maWidgetCreate(MAW_STACK_SCREEN);
	}

	void createSettingsScreen()
	{
		mSettingsString = new SettingsScreen();
		mSettingsString->numCameras = maCameraNumber();
		char buffer[256];
		maCameraGetProperty(MA_CAMERA_FLASH_SUPPORTED, buffer, 256);
		if(strcmp(buffer, "true") == 0)
		{
			mSettingsString->flashSupported = true;
		}
		else
		{
			mSettingsString->flashSupported = false;
		}
		mSettingsString->initialize(mStackScreen, mCameraPreview);
	}

	void createImageScreen()
	{
		mImageScreen = new ImageScreen();
		mImageScreen->initialize(mStackScreen);
	}

	void createMainScreen()
	{
		// Create a Native UI screen. As the screen is a member
		// variable (also called instance variable) we have
		// prefixed the variable name with "m".
		mScreen = maWidgetCreate(MAW_SCREEN);

		// Check if NativeUI is supported by the runtime platform.
		// For example, MoRE does not support NativeUI at the time
		// of writing this program.
		if (-1 == mScreen)
		{
			maPanic(
				0,
				"This program uses features that are only available on Android and iOS. "
				"You must run it directly on the device or devices emulator."
				);
		}

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

		//A wrapper that creates the preview widget that holds the
		//live view from the camera
		createCameraWidget();

		//Camera Control buttons are created here
		createCameraControlButtons();

	}

	/**
	 *
	 * A Wrapper function that sets the common properties
	 * for the camera control buttons
	 *
	 */
	void setCameraButtonProperties(
			MAHandle widgetHandle,
			const char * title,
			int width,
			int height
			)
	{
		widgetSetPropertyInt(
			widgetHandle,
			MAW_WIDGET_WIDTH,
			width);
		widgetSetPropertyInt(
			widgetHandle,
			MAW_WIDGET_HEIGHT,
			height);
		maWidgetSetProperty(
			widgetHandle,
			MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			widgetHandle,
			MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			widgetHandle,
			MAW_BUTTON_TEXT,
			title);
	}

	/**
	 * A Wrapper function for creating the camera control buttons
	 * and the layout that holds them
	 */
	void createCameraControlButtons()
	{
		mZoomInButton = maWidgetCreate(MAW_BUTTON);
		setCameraButtonProperties(
			mZoomInButton,
			"+",
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			60);

		mShowLastImageButton = maWidgetCreate(MAW_BUTTON);
		setCameraButtonProperties(
			mShowLastImageButton,
			"Image",
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			60);

		mSettingsButton = maWidgetCreate(MAW_BUTTON);
		setCameraButtonProperties(
			mSettingsButton,
			"Settings",
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			60);

		mZoomOutButton = maWidgetCreate(MAW_BUTTON);
		setCameraButtonProperties(
			mZoomOutButton,
			"-",
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			60);

		mSecondLayoutWidget = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
		widgetSetPropertyInt(
			mSecondLayoutWidget,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mSecondLayoutWidget,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		//Adding buttons to the horizontal Layout
		maWidgetAddChild(mSecondLayoutWidget, mZoomInButton);
		maWidgetAddChild(mSecondLayoutWidget, mShowLastImageButton);
		maWidgetAddChild(mSecondLayoutWidget, mSettingsButton);
		maWidgetAddChild(mSecondLayoutWidget, mZoomOutButton);

		//Disable zoom buttons if zoom is not supported
		if(maxZoom == 0)
		{
			maWidgetSetProperty(mZoomInButton,
					MAW_WIDGET_ENABLED,
					"false");
			maWidgetSetProperty(mZoomOutButton,
					MAW_WIDGET_ENABLED,
					"false");
		}

		// Then we add the layout to its parent
		maWidgetAddChild(mMainLayoutWidget, mSecondLayoutWidget);

		//We create the capture button as a larger button
		//so it is easier to be touched
		mCaptureButton = maWidgetCreate(MAW_BUTTON);
		setCameraButtonProperties(
			mCaptureButton,
			"Take Snapshot",
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			MAW_CONSTANT_WRAP_CONTENT
			);

		//Add the capture button to the main layout so
		//it will be larger than others.
		maWidgetAddChild(mMainLayoutWidget, mCaptureButton);

	}

	/**
	 * A Wrapper function that creates the camera widget
	 * and binds it to the default camera, The binding
	 * can be changed through the settings screen
	 */
	void createCameraWidget()
	{
		mCameraPreview = maWidgetCreate(MAW_CAMERA_PREVIEW);
			widgetSetPropertyInt(
				mCameraPreview,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE
				);
			widgetSetPropertyInt(
				mCameraPreview,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE
				);

			//bind the widget to the default camera
			maCameraSetPreview(mCameraPreview);
			maWidgetAddChild(mMainLayoutWidget, mCameraPreview);
	}

	/**
	 * Method that makes the screen visible.
	 */
	void show()
	{
		// Display the NativeUI screen.
		// Note: This would hide any previously visible screen.
		maWidgetStackScreenPush(mStackScreen, mScreen);
		maWidgetScreenShow(mStackScreen);
		setupCameraProperties();
		maCameraStart();
		createSettingsScreen();
		createImageScreen();

	}

	/**
	 * A function to setup the camera properties whenever they are
	 * changed and we come back fro mthe settings screen
	 */
	void setupCameraProperties()
	{
		maCameraSelect(mSettingsString->getCurrentCamera());
		maCameraSetPreview(mCameraPreview);
		maCameraSetProperty(
				MA_CAMERA_FLASH_MODE,
				mSettingsString->getFLashMode()
				);
		char buffer[256];
		int length = maCameraGetProperty(MA_CAMERA_MAX_ZOOM, buffer, 256);
		maxZoom = atoi(buffer);
		setupCameraSize();
	}


	void setupCameraSize()
	{
		MA_CAMERA_FORMAT *cameraFormat = new(MA_CAMERA_FORMAT);
		cameraFormat->height = 270;
		cameraFormat->width = 270;
		maCameraFormat(0, cameraFormat);
	}

	/**
	 * This method implements a custom event listener.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{
		// Check if this is a widget event.
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

			// Here we handle clicked events.
			if (MAW_EVENT_CLICKED == eventData->eventType)
			{
				if (mCaptureButton == eventData->widgetHandle)
				{
					captureButtonClicked();
				}
				else if (mSettingsButton == eventData->widgetHandle)
				{
					maCameraStop();
					mSettingsString->pushSettingsScreen();
				}
				else if (mShowLastImageButton == eventData->widgetHandle)
				{
					showImageButtonClicked();
				}
				else if (mZoomInButton == eventData->widgetHandle)
				{
					//Increase the zoom level if it is more
					//than the maximum supported zoom
					if(mCurrentZoomIndex < maxZoom)
					{
						mCurrentZoomIndex++;
					}
					char buffer[256];
					sprintf(buffer, "%i", mCurrentZoomIndex);
					maCameraSetProperty(MA_CAMERA_ZOOM, buffer);

				}
				else if (mZoomOutButton == eventData->widgetHandle)
				{
					//Decrease the zoom index if it is more than 0
					if(mCurrentZoomIndex > 0)
					{
						mCurrentZoomIndex--;
					}
					char buffer[256];
					sprintf(buffer, "%i", mCurrentZoomIndex);
					maCameraSetProperty(MA_CAMERA_ZOOM, buffer);

				}
				else
				{
					//forward the event to a screen that is viewed
					if(mSettingsString->isViewed)
					{
						mSettingsString->customEvent(event);
						if(false == mSettingsString->isViewed)
						{
							setupCameraProperties();
							maCameraStart();
						}
					}
					else if(mImageScreen->isViewed)
					{
						mImageScreen->customEvent(event);
						if(false == mSettingsString->isViewed)
						{
							setupCameraProperties();
							maCameraStart();
						}
					}
				}
			}
		}
	}

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void settingsButtonClicked()
	{
		mSettingsString->pushSettingsScreen();
	}

	/**
	 * This method is called when the Submit button is clicked.
	 */
	void captureButtonClicked()
	{
		if(mLastEnc != 0)
		{
			maDestroyObject(mLastEnc);
		}
		mLastEnc = maCreatePlaceholder();
		maCameraSnapshot(0, mLastEnc);
		setupCameraProperties();
		maCameraStart();
	}


	/**
	 * This method is called when the Submit button is clicked.
	 */
	void showImageButtonClicked()
	{
		maCameraStop();
		mImageScreen->setImageDataHandle(mLastEnc);
		mImageScreen->pushImageScreen();
	}

private:

	/* The Settings screen class that
	 * creates and handles the settings view*/
	SettingsScreen *mSettingsString;

	/* The Image screen class that
	 * creates and handles the Image view*/
	ImageScreen *mImageScreen;

	/* Stack Screen used to handle screen changes*/
	MAHandle mStackScreen;

	/* place holder used for keeping the last image*/
	MAHandle mLastEnc;

	/** The screen widget. */
	MAHandle mScreen;

	/** The main layout that holds the other widgets. */
	MAHandle mMainLayoutWidget;

	/** The main layout that holds the other widgets. */
	MAHandle mSecondLayoutWidget;

	/** Text editor box for user input. */
	MAHandle mCameraPreview;

	/** The Settings button. */
	MAHandle mSettingsButton;

	/** The Show Image button. */
	MAHandle mShowLastImageButton;

	/** The Capture button. */
	MAHandle mCaptureButton;

	/** The Zoom In button. */
	MAHandle mZoomInButton;

	/** The Zoom Out button. */
	MAHandle mZoomOutButton;

	/* index of the current zoom level*/
	int mCurrentZoomIndex;

	/* maximum zoom supported by the camera*/
	int maxZoom;
};

// That's the screen class finished, now we move on to the Moblet that
// is the main object in the application.

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events. Our Moblet inherits
 * the Moblet base class in the MAUtil library.
 */
class HelloNativeUIMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The user interface is created in the constructor method.
	 */
	HelloNativeUIMoblet()
	{
		// Create the main (and only) screen used in the application.
		mAppScreen = new AppScreen();

		// Display the screen.
		mAppScreen->show();
	}

	/**
	 * The destructor deletes the screen object.
	 */
	virtual ~HelloNativeUIMoblet()
	{
		delete mAppScreen;
	}

	/**
	 * This method is called when a key is pressed. The method
	 * is inherited from the Moblet class, and is overridden here.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Close the application if the back key is pressed.
		if (MAK_BACK == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

private:
	/**
	 * The screen of our application.
	 */
	AppScreen* mAppScreen;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Create and run the Moblet to start the application.
	MAUtil::Moblet::run(new HelloNativeUIMoblet());

	// The Moblet will run until it is closed by the user.
	// When it is closed we end our program in a well-behaved
	// way, returning zero.
	return 0;
}
