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
 * @author Mattias Frånberg and Chris Hughes
 *
 * This application provides a very basic example of how to work
 * with Native UI to position and and manipulate graphical user
 * interface elements. The code is very well commented so that you
 * can see what's happening at each step. The application uses one
 * screen and a some widgets to provide a password entry box and
 * some control buttons.
 *
 * NOTE: This example application has the same functionality as
 * our example application "HelloMAUI". Compare the two examples
 * to see how we do similar things in MAUI and in Native UI.
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
		mLastPic = 0;
		mCurrentSizeIndex = 0;
		createUI();
		mNumCameras = maCameraNumber();
		mActiveCamera = 0;
		currentViewIndex = 0;
		mNumFailure = 0;
		mNumSuccess = 0;
		char buffer[256];
		maCameraGetProperty(MA_CAMERA_MAX_ZOOM, buffer, 256);
		maxZoom = atoi(buffer);
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


	void myAssert(int cameraResult)
	{
		if((cameraResult == MA_CAMERA_RES_OK) ||
				(cameraResult == MA_CAMERA_RES_PROPERTY_NOTSUPPORTED) ||
				(cameraResult == MA_CAMERA_RES_VALUE_NOTSUPPORTED))
			mNumSuccess++;
		else
			mNumFailure++;
	}
	/**
	 * Here we create the user interface widgets.
	 */
	void createUI()
	{
		// Make the AppScreen listen for events coming from widgets.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);

		// Create a Native UI screen. As the screen is a member
		// variable (also called instance variable) we have
		// prefixed the variable name with "m".
		mScreen = maWidgetCreate(MAW_SCREEN);

		// Check if NativeUI is supported by the runtime platform.
		// For example, MoRE does not support NativeUI at the time
		// of writing this program.
		if (-1 == mScreen)
		{
			maPanic(0,
				"This program uses features that are only available on Android and iOS. "
				"You must run it directly on the device or devices emulator.");
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

		// the second layout is a horizontal layout that
		//contains the buttons that control the camera
		mSecondLayoutWidget = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
		widgetSetPropertyInt(
			mSecondLayoutWidget,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mSecondLayoutWidget,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		// Add the layout as the root of the screen's widget tree.
		maWidgetAddChild(mScreen, mMainLayoutWidget);

		// Next, we create four child widgets within the main layout widget.

		// The first widget is a label that we'll use to present instructions.
		mInstructions = maWidgetCreate(MAW_LABEL);

		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
			mInstructions,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mInstructions,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
			mInstructions,
			MAW_LABEL_TEXT,
			"Camera started with default properties!!!");

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, mInstructions);

		//Camera Control buttons are created here
		createCameraControlButtons();

		createCameraWidget();

		createSizeArray();


	}

	void createSizeArray()
	{
		mNumFormats = maCameraFormatNumber();
		if(mNumFormats >10)
			mNumFormats = 10;
		MA_CAMERA_FORMAT fmt[10];
		int width = 0;
		int height = 0;
		for (int i = 0; i< mNumFormats; i++)
		{
			height += 270;
			width = (height * 4) / 3;

			fmt[i].width = width;
			fmt[i].height = height;
			maCameraFormat(i , &fmt[i]);
		}
	}

	void setButtonProperties(MAHandle widgetHandle , const char * title)
	{
		widgetSetPropertyInt(
			widgetHandle,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			widgetHandle,
			MAW_WIDGET_HEIGHT,
			50);
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

	void createCameraControlButtons()
	{

		mZoomInButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mZoomInButton, "+");
		maWidgetAddChild(mSecondLayoutWidget, mZoomInButton);

		mZoomOutButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mZoomOutButton, "-");
		maWidgetAddChild(mSecondLayoutWidget, mZoomOutButton);

		mShowLastImageButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mShowLastImageButton, "Image");
		maWidgetAddChild(mSecondLayoutWidget, mShowLastImageButton);

		mStartPreviewButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mStartPreviewButton, "Start");
		maWidgetAddChild(mSecondLayoutWidget, mStartPreviewButton);

		mStopPreviewButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mStopPreviewButton, "Stop");
		maWidgetAddChild(mSecondLayoutWidget, mStopPreviewButton);



		maWidgetAddChild(mMainLayoutWidget, mSecondLayoutWidget);

		mCaptureButton = maWidgetCreate(MAW_BUTTON);
		setButtonProperties(mCaptureButton, "Take Snapshot & go to Next step");

		//Add the capture button to the main layout so
		//it will be larger than others.
		maWidgetAddChild(mMainLayoutWidget, mCaptureButton);

	}

	void createCameraWidget()
	{
		mCameraPreview = maWidgetCreate(MAW_CAMERA_PREVIEW);
			widgetSetPropertyInt(
						mCameraPreview,
						MAW_WIDGET_WIDTH,
						MAW_CONSTANT_FILL_AVAILABLE_SPACE);
			widgetSetPropertyInt(
						mCameraPreview,
						MAW_WIDGET_HEIGHT,
						MAW_CONSTANT_FILL_AVAILABLE_SPACE);

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
		maWidgetScreenShow(mScreen);
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
				else if (mStartPreviewButton == eventData->widgetHandle)
				{
					startButtonClicked();
				}
				else if (mStopPreviewButton == eventData->widgetHandle)
				{
					stopCamera();
				}
				else if (mShowLastImageButton == eventData->widgetHandle)
				{
					showImageButtonClicked();
				}
				else if (mZoomInButton == eventData->widgetHandle)
				{
					if(mCurrentZoomIndex < maxZoom)
						mCurrentZoomIndex++;
					char buffer[256];
					sprintf(buffer, "%i", mCurrentZoomIndex);
					setCameraProperty(MA_CAMERA_ZOOM, buffer);
				}
				else if (mZoomOutButton == eventData->widgetHandle)
				{
						if(mCurrentZoomIndex > 0)
							mCurrentZoomIndex--;
						char buffer[256];
						sprintf(buffer, "%i", mCurrentZoomIndex);
						setCameraProperty(MA_CAMERA_ZOOM, buffer);
				}

			}
		}
	}


	/**
	 * This method is called when the Clear button is clicked.
	 */
	void startButtonClicked()
	{
		if(mCameraPreview == 0)
		{
			if (mImageWidget != 0)
			{
				maWidgetDestroy(mImageWidget);
				mImageWidget = 0;
			}
			createCameraWidget();

		}
		startCamera();
	}

	void startCamera()
	{
		int res = maCameraStart();
		if (res != MA_CAMERA_RES_OK)
			maPanic(res, "Failed to start the Preview");

	}

	void stopCamera()
	{
		int res = maCameraStop();
		if (res != MA_CAMERA_RES_OK)
			maPanic(res, "Failed to stop the Preview");

	}

	int setCameraProperty(const char * property, const char* value)
	{
		int res = maCameraSetProperty(property, value);
		return res;
	}
	/**
	 * This method is called when the Submit button is clicked.
	 */
	void captureButtonClicked()
	{
		if(mLastEnc != 0)
			maDestroyObject(mLastEnc);
		mLastEnc = maCreatePlaceholder();
		int res = maCameraSnapshot(mCurrentSizeIndex, mLastEnc);
		if (res != MA_CAMERA_RES_OK)
			maPanic(res, "Failed to start the Preview");
		currentViewIndex++;
		setParameters(currentViewIndex);
	}

	void setParameters(int index)
	{
		if(currentViewIndex == 1)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FLASH_MODE, MA_CAMERA_FLASH_ON));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n  MA_CAMERA_FLASH_ON!!!");
		}
		else if (currentViewIndex == 2)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FLASH_MODE, MA_CAMERA_FLASH_AUTO));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FLASH_AUTO!!!");
		}
		else if (currentViewIndex == 3)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FLASH_MODE, MA_CAMERA_FLASH_TORCH));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FLASH_TORCH!!!");
		}
		else if (currentViewIndex == 4)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FLASH_MODE, MA_CAMERA_FLASH_OFF));
			myAssert(setCameraProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_AUTO));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FOCUS_AUTO!!!");
		}
		else if (currentViewIndex == 5)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_FIXED));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FOCUS_FIXED!!!");
		}
		else if (currentViewIndex == 6)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_INFINITY));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n\n  MA_CAMERA_FOCUS_INFINITY!!!");
		}
		else if (currentViewIndex == 7)
		{
			startCamera();
			myAssert(setCameraProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_MACRO));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FOCUS_MACRO!!!");
		}
		else if (currentViewIndex == 8)
		{
			createNewUI();
		}
		else
		{
			myAssert(setCameraProperty(MA_CAMERA_FLASH_MODE, MA_CAMERA_FLASH_AUTO));
			maWidgetSetProperty(mInstructions,MAW_LABEL_TEXT, "Camera with\n MA_CAMERA_FLASH_AUTO!!!");
		}
	}

	void createNewUI()
	{
		stopCamera();
		maWidgetDestroy(mScreen);
		// Create a Native UI screen. As the screen is a member
		// variable (also called instance variable) we have
		// prefixed the variable name with "m".
		mScreen = maWidgetCreate(MAW_SCREEN);
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

		// Next, we create four child widgets within the main layout widget.

		// The first widget is a label that we'll use to present instructions.
		mInstructions = maWidgetCreate(MAW_LABEL);

		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				mInstructions,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				mInstructions,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				mInstructions,
				MAW_LABEL_TEXT,
				"Reading the Camera properties!!!");

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, mInstructions);
		int flashLabel = maWidgetCreate(MAW_LABEL);

			// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				flashLabel,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				flashLabel,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				flashLabel,
				MAW_LABEL_TEXT,
				"\nMA_CAMERA_FLASH_SUPPORTED:");

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, flashLabel);

		int flashSupported = maWidgetCreate(MAW_LABEL);

		char buffer[256];
		maCameraGetProperty(MA_CAMERA_FLASH_SUPPORTED, buffer, 256);

		widgetSetPropertyInt(
				flashSupported,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				flashSupported,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				flashSupported,
				MAW_LABEL_TEXT,
				buffer);

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, flashSupported);
		int zoomLabel = maWidgetCreate(MAW_LABEL);

			// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				zoomLabel,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				zoomLabel,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				zoomLabel,
				MAW_LABEL_TEXT,
				"\n MA_CAMERA_ZOOM_SUPPORTED:");

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, zoomLabel);

		int zoomSupported = maWidgetCreate(MAW_LABEL);

		char buffer2[256];
		maCameraGetProperty(MA_CAMERA_ZOOM_SUPPORTED, buffer2, 256);

		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				zoomSupported,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				zoomSupported,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				zoomSupported,
			MAW_LABEL_TEXT,
			buffer2);

		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, zoomSupported);

		int numCameras = maCameraNumber();
		char buffer3[128];
		sprintf(buffer3,"\n Number of Cameras:  %d", numCameras);
		int cameraNumber = maWidgetCreate(MAW_LABEL);


		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				cameraNumber,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				cameraNumber,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				cameraNumber,
				MAW_LABEL_TEXT,
				buffer3);
		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, cameraNumber);

		char buffer4[128];
		sprintf(buffer4,
				"\n %d test failed out of  %d total tests",
				mNumFailure,
				mNumFailure+mNumSuccess);
		int failureNumber = maWidgetCreate(MAW_LABEL);


		// Make the label fill the width of the parent layout and
		// adjust its height to "shrink wrap" the size of the text.
		widgetSetPropertyInt(
				failureNumber,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				failureNumber,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_WRAP_CONTENT);

		// Set the label text.
		maWidgetSetProperty(
				failureNumber,
				MAW_LABEL_TEXT,
				buffer4);
		//Adding the label to the layout
		maWidgetAddChild(mMainLayoutWidget, failureNumber);
		maWidgetScreenShow(mScreen);
	}


	/**
	 * This method is called when the Submit button is clicked.
	 */
	void showImageButtonClicked()
	{
		stopCamera();
		if(mLastEnc != 0) {
			if(mLastPic != 0) {
				maDestroyObject(mLastPic);
			}
			mLastPic = maCreatePlaceholder();
			int res = maCreateImageFromData(mLastPic, mLastEnc, 0, maGetDataSize(mLastEnc));
			if(res != RES_OK)
			{
				maPanic(res, "failed to create the image");
			}

			//destroy the cmaera preview and replace it with an image widget
			maWidgetDestroy(mCameraPreview);
			mCameraPreview = 0;
			mImageWidget = maWidgetCreate(MAW_IMAGE);
			widgetSetPropertyInt(
					mImageWidget,
					MAW_WIDGET_WIDTH,
					MAW_CONSTANT_FILL_AVAILABLE_SPACE);
			widgetSetPropertyInt(
					mImageWidget,
					MAW_WIDGET_HEIGHT,
					MAW_CONSTANT_FILL_AVAILABLE_SPACE);
			maWidgetSetProperty(mImageWidget, MAW_IMAGE_SCALE_MODE, "scalePreserveAspect");
			maWidgetAddChild(mMainLayoutWidget, mImageWidget);
			res = widgetSetPropertyInt(
					mImageWidget,
					MAW_IMAGE_IMAGE,
					mLastPic);

		}
	}

private:

	MAHandle mLastEnc;

	MAHandle mLastPic;

	/** The screen widget. */
	MAHandle mScreen;

	/** The main layout that holds the other widgets. */
	MAHandle mMainLayoutWidget;

	MAHandle mImageLayoutWidget;

	/** The main layout that holds the other widgets. */
	MAHandle mSecondLayoutWidget;

	/** A text label that displays instructions. */
	MAHandle mInstructions;

	/** Text editor box for user input. */
	MAHandle mCameraPreview;

	/** The Clear button. */
	MAHandle mStartPreviewButton;

	/** The Clear button. */
	MAHandle mStopPreviewButton;

	/** The Clear button. */
	MAHandle mShowLastImageButton;

	/** The Submit button. */
	MAHandle mCaptureButton;

	/** The Submit button. */
	MAHandle mZoomInButton;

	/** The Submit button. */
	MAHandle mZoomOutButton;

	/** The Submit button. */
	MAHandle mSwapCameraButton;

	MAHandle mImageWidget;

	int mNumFormats;

	int mCurrentSizeIndex;

	int mNumCameras;

	int mActiveCamera;

	int currentViewIndex;

	int maxZoom;

	int mCurrentZoomIndex;

	int mNumSuccess;

	int mNumFailure;
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
		// Close the application if the back key or key 0 is pressed.
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
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
