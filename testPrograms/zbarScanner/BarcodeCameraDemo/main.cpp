/*
Copyright (C) 2012 MoSync AB

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

// Include header file for Moblets.
#include <MAUtil/Moblet.h>

// Include NativeUI class interface
#include <NativeUI/Widgets.h>

// Include widget utility functions. These functions simplify
// getting and setting widget properties.
#include "WidgetUtil.h"

#include <conprint.h>
#include <MAUtil/String.h>
#include "SettingsScreen.h"
#include "ImageScreen.h"

#include "BarcodeScanner.h"

#include <mavsprintf.h>

using namespace MAUtil;
using namespace NativeUI;
/**
 * Class that wraps a NativeUI screen widget. We implement
 * the CustomEventListener interface to be able to process
 * widget events.
 */
class AppScreen : public ButtonListener, public StackScreenListener
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
		delete mScreen;
	}

	/**
	 * Here we create the user interface widgets.
	 */
	void createUI()
	{
		// Make the AppScreen listen for events coming from widgets.
		//MAUtil::Environment::getEnvironment().addCustomEventListener(this);
		char buffer[256];
		maCameraGetProperty(MA_CAMERA_MAX_ZOOM, buffer, 256);
		maxZoom = atoi(buffer);

		maScreenSetOrientation(SCREEN_ORIENTATION_LANDSCAPE);

		mStackScreen = new StackScreen();
		mStackScreen->addStackScreenListener(this);
		createSettingsScreen();
		createMainScreen();
		createImageScreen();
		setupCameraProperties();
	}

	void createSettingsScreen()
	{
		mSettingsScreen = new SettingsScreen();
		mSettingsScreen->numCameras = maCameraNumber();
		char buffer[256];
		maCameraGetProperty(MA_CAMERA_FLASH_SUPPORTED, buffer, 256);
		if(strcmp(buffer, "true") == 0)
		{
			mSettingsScreen->flashSupported = true;
		}
		else
		{
			mSettingsScreen->flashSupported = false;
		}
		mSettingsScreen->initialize(mStackScreen);
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
		mScreen = new Screen();

		mScreen->setTitle("Camera Demo");

		// Create the screen's main layout widget.
		mMainLayoutWidget = new VerticalLayout();

		// Make the layout fill the entire screen.
		mMainLayoutWidget->fillSpaceHorizontally();
		mMainLayoutWidget->fillSpaceVertically();

		// Add the layout as the root of the screen's widget tree.
		mScreen->setMainWidget(mMainLayoutWidget);

		//A wrapper that creates the preview widget that holds the
		//live view from the camera
		createCameraWidget();

		//Camera Control buttons are created here
		//createCameraControlButtons();

		mAutoFocusButton = new Button();
		mAutoFocusButton->setText("Auto Focus");
		mAutoFocusButton->fillSpaceHorizontally();
		mAutoFocusButton->addButtonListener(this);

		mMainLayoutWidget->addChild(mAutoFocusButton);
	}

	/**
	 * A Wrapper function for creating the camera control buttons
	 * and the layout that holds them
	 */
/*
	void createCameraControlButtons()
	{

		mZoomInButton = new Button();
		mZoomInButton->setText("Zoom +");
		mZoomInButton->fillSpaceHorizontally();
		mZoomInButton->addButtonListener(this);

		mShowLastImageButton = new Button();
		mShowLastImageButton->setText("Image");
		mShowLastImageButton->fillSpaceHorizontally();
		mShowLastImageButton->addButtonListener(this);

		mSettingsButton = new Button();
		mSettingsButton->setText("Settings");
		mSettingsButton->fillSpaceHorizontally();
		mSettingsButton->addButtonListener(this);

		mZoomOutButton = new Button();
		mZoomOutButton->setText("Zoom -");
		mZoomOutButton->fillSpaceHorizontally();
		mZoomOutButton->addButtonListener(this);

		mSecondLayoutWidget = new HorizontalLayout();
		mSecondLayoutWidget->fillSpaceHorizontally();

		if(isWP7())
		{
			mZoomInButton->setHeight(80);
			mZoomInButton->setFontSize(17);
			mZoomOutButton->setHeight(80);
			mZoomOutButton->setFontSize(17);
			mSettingsButton->setHeight(80);
			mSettingsButton->setFontSize(17);
			mShowLastImageButton->setHeight(80);
			mShowLastImageButton->setFontSize(17);
			mSecondLayoutWidget->setHeight(80);
		}
		else
		{
			mZoomInButton->setHeight(60);
			mZoomOutButton->setHeight(60);
			mSettingsButton->setHeight(60);
			mShowLastImageButton->setHeight(60);
			mSecondLayoutWidget->setHeight(60);
		}

		//Adding buttons to the horizontal Layout
		mSecondLayoutWidget->addChild(mZoomInButton);
		mSecondLayoutWidget->addChild(mShowLastImageButton);
		mSecondLayoutWidget->addChild(mSettingsButton);
		mSecondLayoutWidget->addChild(mZoomOutButton);


		// Then we add the layout to its parent
		mMainLayoutWidget->addChild(mSecondLayoutWidget);

		//We create the capture button as a larger button
		//so it is easier to be touched
		mCaptureButton = new Button();

		mCaptureButton->setText("Take snapshot");
		mCaptureButton->fillSpaceHorizontally();
		mCaptureButton->wrapContentVertically();
		mCaptureButton->addButtonListener(this);


		//Add the capture button to the main layout so
		//it will be larger than others.
		mMainLayoutWidget->addChild(mCaptureButton);

	}
*/
	/**
	 * A Wrapper function that creates the camera widget
	 * and binds it to the default camera, The binding
	 * can be changed through the settings screen
	 */
	void createCameraWidget()
	{
		mCameraPreview = new CameraPreview();

		mCameraPreview->fillSpaceHorizontally();
		mCameraPreview->fillSpaceVertically();

		//bind the widget to the default camera
		mCameraPreview->bindToCurrentCamera();
		mMainLayoutWidget->addChild(mCameraPreview);
	}

	void initCameraPreviewEvents()
	{

		MAExtent size = maCameraPreviewSize();

		int height = EXTENT_Y(size);
		int width = EXTENT_X(size);

		lprintfln("Returned values width:%u height:%u\n", width, height);

		int pHeight = 60;
		int pWidth = width;
		int pTop = height/2+pHeight/2;
		int pLeft = 0;

		lprintfln("left:%u top:%u width:%u height:%u\n", pLeft, pTop, pWidth, pHeight);

		mCameraPreviewWidth = pWidth;
		mCameraPreviewHeight = pHeight;

		mCameraPreviewSize = pHeight * pWidth;
		mCameraPreviewBuffer = new int[mCameraPreviewSize];

		MARect previewRect = {pLeft,pTop,pWidth,pHeight};

		int e = maCameraPreviewEventEnable(MA_CAMERA_PREVIEW_FRAME, mCameraPreviewBuffer, &previewRect);
		if(e<0)
			maPanic(e, "NO!");

		mBarcodeScanner = new BarcodeScanner();
		mBarcodeScanner->initiate();
	}

	/**
	 * Method that makes the screen visible.
	 */
	void show()
	{
		// Display the NativeUI screen.
		// Note: This would hide any previously visible screen.
		mStackScreen->push(mScreen);
		mStackScreen->show();
		maCameraStart();

		int res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_MACRO );
		if(res < 0)
		{
			lprintfln("NO! WE HAVE NO MACRO FOCUS!");
			res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_AUTO );
			if(res < 0)
			{
				lprintfln("NO! WE HAVE NO AUTO FOCUS!");
			}
		}

		initCameraPreviewEvents();
	}

	/**
	 * A function to setup the camera properties whenever they are
	 * changed and we come back from the settings screen
	 */
	void setupCameraProperties()
	{
		setupCameraSize();
		maCameraSelect(mSettingsScreen->getCurrentCamera());
		mCameraPreview->bindToCurrentCamera();
		maCameraSetProperty(
			MA_CAMERA_FLASH_MODE,
			mSettingsScreen->getFLashMode()
			);
/*
		int res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_MACRO );
		if(res < 0)
		{
			lprintfln("NO! WE HAVE NO MACRO FOCUS!");
			res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_AUTO );
			if(res < 0)
			{
				lprintfln("NO! WE HAVE NO AUTO FOCUS!");
			}
		}
*/
		char buffer[256];
		maCameraGetProperty(MA_CAMERA_MAX_ZOOM, buffer, 256);
		maxZoom = atoi(buffer);
/*
		//Disable the zoom buttons if zoom is not supported
		if(maxZoom == 0)
		{
			mZoomInButton->setEnabled(false);
			mZoomInButton->setFontColor(0x969696);

			mZoomOutButton->setEnabled(false);
			mZoomOutButton->setFontColor(0x969696);
		}
		else //Or enable show them if it's supported
		{
			mZoomInButton->setEnabled(true);
			mZoomInButton->setFontColor(0x000000);

			mZoomOutButton->setEnabled(true);
			mZoomOutButton->setFontColor(0x000000);
		}
*/
	}


	void setupCameraSize()
	{
		int numFormats = maCameraFormatNumber();
		//Choose the smallest possible format.
		mCameraFormat = numFormats-1;
	}

	/**
	 * This method implements a button event listener.
	 * Button click events are sent here.
	 */
	virtual void buttonClicked(Widget* button)
	{
		if(mAutoFocusButton == button)
		{
			int res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_MACRO );
			if(res < 0)
			{
				res = maCameraSetProperty(MA_CAMERA_FOCUS_MODE, MA_CAMERA_FOCUS_AUTO );
			}
		}
/*
		if (mCaptureButton == button)
		{
			captureButtonClicked();
		}
		else if (mSettingsButton == button)
		{
			maCameraStop();
			mSettingsScreen->pushSettingsScreen();
		}
		else if (mShowLastImageButton == button)
		{
			showImageButtonClicked();
		}
		else if (mZoomInButton == button)
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
		else if (mZoomOutButton == button)
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
*/
	}

	/**
	 * This method implements a stack screen event listener.
	 * Pop events are sent here
	 */
	virtual void stackScreenScreenPopped(
	            StackScreen* stackScreen,
	            Screen* fromScreen,
	            Screen* toScreen)
	{
		if (toScreen == mScreen)
		{
			setupCameraProperties();
			maCameraStart();
		}
	}

	/**
	 * This method is called when the Clear button is clicked.
	 */
	void settingsButtonClicked()
	{
		mSettingsScreen->pushSettingsScreen();
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
		maCameraSnapshot(mCameraFormat, mLastEnc);
		setupCameraProperties();
		maCameraStart();
	}


	/**
	 * This method is called when the Submit button is clicked.
	 */
	void showImageButtonClicked()
	{
		//if(mLastEnc == 0)
		//{
			//do nothing when there is no image
			//return;
		//}
		maCameraStop();
		//mImageScreen->setImageDataHandle(mLastEnc);

		mImageScreen->setImageDataRaw(mCameraPreviewBuffer,
										mCameraPreviewWidth,
										mCameraPreviewHeight);


		mImageScreen->pushImageScreen();
	}

	bool getBarCode()
	{
		//mBarcodeScanner->uploadImage(mImageHandle);
		mBarcodeScanner->uploadRGB888(mCameraPreviewBuffer,
										mCameraPreviewWidth,
										mCameraPreviewHeight);

		char t[40];
		char c[40];


		if(mBarcodeScanner->getBarcode(t, c))
		{
			maAlert(t, c, "apa", "ko", "kalv");
			return true;
		}

		return false;
	}

private:

	/* The Settings screen class that
	 * creates and handles the settings view*/
	SettingsScreen *mSettingsScreen;

	/* The Image screen class that
	 * creates and handles the Image view*/
	ImageScreen *mImageScreen;

	/* Stack Screen used to handle screen changes*/
	StackScreen *mStackScreen;

	/* place holder used for keeping the last image*/
	MAHandle mLastEnc;

	/** The screen widget. */
	Screen *mScreen;

	/** The main layout that holds the other widgets. */
	VerticalLayout *mMainLayoutWidget;

	/** The main layout that holds the other widgets. */
//	HorizontalLayout *mSecondLayoutWidget;

	/** Text editor box for user input. */
	CameraPreview *mCameraPreview;

	Button *mAutoFocusButton;

	/** The Settings button. */
//	Button *mSettingsButton;

	/** The Show Image button. */
//	Button *mShowLastImageButton;

	/** The Capture button. */
//	Button *mCaptureButton;

	/** The Zoom In button. */
//	Button *mZoomInButton;

	/** The Zoom Out button. */
//	Button *mZoomOutButton;

	/* index of the current zoom level*/
	int mCurrentZoomIndex;

	/* maximum zoom supported by the camera*/
	int maxZoom;

	/* Size of the taken picture */
	int mCameraFormat;

	int mCameraPreviewSize;
	int mCameraPreviewWidth;
	int mCameraPreviewHeight;
	int* mCameraPreviewBuffer;

	BarcodeScanner* mBarcodeScanner;

	/**
	 * isWP7 function verifies if the platform is Windows Phone 7
	 * @return true if we are running on WP7 otherwise false
	 */
	bool isWP7()
	{
		char platform[20];
		maGetSystemProperty("mosync.device.OS", platform, 20);

		if(strcmp(platform, "iPhone OS") == 0 || strcmp(platform, "Android") == 0) return false;
		return true;
	}
};

// That's the screen class finished, now we move on to the Moblet that
// is the main object in the application.

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events. Our Moblet inherits
 * the Moblet base class in the MAUtil library.
 */
class CameraDemoMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The user interface is created in the constructor method.
	 */
	CameraDemoMoblet()
	{
		Environment::getEnvironment().addCustomEventListener(this);

		// Create the main (and only) screen used in the application.
		mAppScreen = new AppScreen();

		// Display the screen.
		mAppScreen->show();
	}

	/**
	 * The destructor deletes the screen object.
	 */
	virtual ~CameraDemoMoblet()
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

	void customEvent(const MAEvent &e)
	{
		lprintfln("Got a custom event\n");
		if(e.type == EVENT_TYPE_CAMERA_PREVIEW)
		{
			lprintfln("Got a preview event\n");
			int start = maGetMilliSecondCount();
			bool result = mAppScreen->getBarCode();
			int end = maGetMilliSecondCount();
			lprintfln("barcode search finished in %u ms\n", (end-start));

			if(!result)
				maCameraPreviewEventConsumed();
		}
		if(e.type == EVENT_TYPE_ALERT)
		{
			maCameraPreviewEventConsumed();
		}

		if(e.type == EVENT_TYPE_KEY_PRESSED)
		{
			if(e.key == MAK_BACK)
			{
				close();
			}
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
	MAUtil::Moblet::run(new CameraDemoMoblet());

	// The Moblet will run until it is closed by the user.
	// When it is closed we end our program in a well-behaved
	// way, returning zero.
	return 0;
}
