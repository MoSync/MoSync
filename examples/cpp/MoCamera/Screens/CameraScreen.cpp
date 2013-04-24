/*
 Copyright (C) 2013 MoSync AB

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
 * @file CameraScreen.cpp
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Screen with camera controls and preview.
 */

#include "CameraScreen.h"
#include "ScreenUtils.h"

// This is needed in order to access resources from the res.lstx
#include "MAHeaders.h"

#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace NativeUI;

#define BTN_RATIO 0.16 // Image button/container width ratio for normal buttons
#define TAKE_PICTURE_BTN_RATIO 0.30 // Image button/container width ratio for the -take snapshot- button
#define ZOOM_STEP_RATIO 0.1 // (Zoom step)/(maximum zoom) ratio
#define ACTIVITY_INDICATOR_WIDTH 100 // Width of the activity indicator
#define ACTIVITY_INDICATOR_HEIGHT 100 // Height of the activity indicator

// The resource arrays must be synchronized with the kFlashModes array!
static const CAMERA_FLASH_MODE kFlashModes[] = {FLASH_AUTO, FLASH_ON, FLASH_OFF, FLASH_TORCH};
static const int kFlashModesImg[] = {RES_FLASH_AUTO_IMG, RES_FLASH_ON_IMG, RES_FLASH_OFF_IMG, RES_FLASH_TORCH_IMG};
static const int kFlashModesImgPressed[] = {RES_FLASH_AUTO_IMG_PRESSED,
		RES_FLASH_ON_IMG_PRESSED, RES_FLASH_OFF_IMG_PRESSED, RES_FLASH_TORCH_IMG_PRESSED};

namespace MoSyncCamera
{
	CameraScreen::CameraScreen(CameraScreenObserver& observer):
			mObserver(observer),
			mMainLayout(NULL),
			mCamera(NULL),
			mTakeSnapshotButton(NULL),
			mShowSnapshotButton(NULL),
			mSetNextFlashModeButton(NULL),
			mSetNextCameraButton(NULL),
			mZoomInButton(NULL),
			mZoomOutButton(NULL),
			mCurrentFlashMode(0)
	{
		createUI();
	}


	CameraScreen::~CameraScreen()
	{
		mCamera->stopPreview();

		mTakeSnapshotButton->removeButtonListener(this);
		mShowSnapshotButton->removeButtonListener(this);
		mSetNextFlashModeButton->removeButtonListener(this);
		mSetNextCameraButton->removeButtonListener(this);
		mZoomInButton->removeButtonListener(this);
		mZoomOutButton->removeButtonListener(this);
	}


	void CameraScreen::createUI()
	{
        setupMainLayout();
		setupCamera();
		setupButtons();
		setupActivityIndicator();
		arrangeWidgets();
	}


	void CameraScreen::setupMainLayout()
	{
		mMainLayout = new RelativeLayout();
		mMainLayout->setBackgroundColor(BLACK_COLOR);
		setMainWidget(mMainLayout);
	}


	void CameraScreen::setupCamera()
	{
		mCamera = new Camera();
		mCamera->fillSpaceHorizontally();
		mCamera->fillSpaceVertically();
		mMainLayout->addChild(mCamera);
	}


	void CameraScreen::setupActivityIndicator()
	{
		mActivityIndicator = new ActivityIndicator();
		mActivityIndicator->setWidth(ACTIVITY_INDICATOR_WIDTH);
		mActivityIndicator->setHeight(ACTIVITY_INDICATOR_HEIGHT);
		mActivityIndicator->hide();
		mMainLayout->addChild(mActivityIndicator);
	}


	void CameraScreen::resetCameraPropeties()
	{
		if ( mCamera )
		{
			mCamera->setFlashMode(FLASH_AUTO);
			mCamera->setFocusMode(FOCUS_AUTO);
		}
	}


	void CameraScreen::setupButtons()
	{
		// take snapshot button
		mTakeSnapshotButton = new NativeUI::ImageButton();
		ScreenUtils::setupImageButton(mTakeSnapshotButton,
				this, RES_TAKE_PICTURE_IMG, RES_TAKE_PICTURE_IMG_PRESSED);
		mMainLayout->addChild(mTakeSnapshotButton);

		// show snapshot button
		mShowSnapshotButton = new NativeUI::ImageButton();
		mShowSnapshotButton->setEnabled(false);
		ScreenUtils::setupImageButton(mShowSnapshotButton,
				this, RES_SHOW_PICTURE_IMG_DISABLED, RES_SHOW_PICTURE_IMG_DISABLED);
		mMainLayout->addChild(mShowSnapshotButton);

		// flash mode button
		mSetNextFlashModeButton = new NativeUI::ImageButton();
		ScreenUtils::setupImageButton(mSetNextFlashModeButton,
				this, RES_FLASH_AUTO_IMG, RES_FLASH_AUTO_IMG_PRESSED);
		mMainLayout->addChild(mSetNextFlashModeButton);

		// If there is not flash support on the default camera make the button unavailable.
		if ( !mCamera->isFlashSupported() )
		{
			// If flash not supported disable the flash button.
			mSetNextFlashModeButton->setEnabled(false);
			mSetNextFlashModeButton->setBackgroundImage(kFlashModesImg[FLASH_OFF]);
			// Pressed is not handled so no pressed image needed.
			mSetNextFlashModeButton->setPressedImage(kFlashModesImg[FLASH_OFF]);
		}

		// If we have only one camera we don't add the swap camera button.
		uint nrOfCameras = mCamera->getNumberOfAvailableCameras();
		if ( nrOfCameras > 1 )
		{
			// camera source button
			mSetNextCameraButton = new NativeUI::ImageButton();
			ScreenUtils::setupImageButton(mSetNextCameraButton,
					this, RES_CAMERA_SWAP_IMG, RES_CAMERA_SWAP_IMG_PRESSED);
			mMainLayout->addChild(mSetNextCameraButton);
		}

		// If zoom is not supported do not add zoom buttons.
		if ( mCamera->isZoomSupported() )
		{
			// camera zoom in button
			mZoomInButton = new NativeUI::ImageButton();
			ScreenUtils::setupImageButton(mZoomInButton,
					this, RES_ZOOM_IN_IMG, RES_ZOOM_IN_IMG_PRESSED);
			mMainLayout->addChild(mZoomInButton);

			// camera zoom out button
			mZoomOutButton = new NativeUI::ImageButton();
			ScreenUtils::setupImageButton(mZoomOutButton,
					this, RES_ZOOM_OUT_IMG, RES_ZOOM_OUT_IMG_PRESSED);
			mMainLayout->addChild(mZoomOutButton);
		}
	}


	void CameraScreen::arrangeWidgets()
	{
		/// Get screen dimensions.
		MAExtent size = maGetScrSize();
		/// Extract the screen width
		int screenWidth = EXTENT_X(size);
		/// Extract the screen height
		int screenHeight = EXTENT_Y(size);

		mCamera->setPosition(0, 0);
		mCamera->setWidth(screenWidth);
		mCamera->setHeight(screenHeight);

		mActivityIndicator->setPosition((screenWidth - mActivityIndicator->getWidth())/2,
				(screenHeight - mActivityIndicator->getHeight())/2);

		ScreenUtils::resizeWidget(mTakeSnapshotButton, size, TAKE_PICTURE_BTN_RATIO);
		int btnY = screenHeight - mTakeSnapshotButton->getHeight(); // bottom
		int btnX = (screenWidth - mTakeSnapshotButton->getWidth())/2; // middle
		mTakeSnapshotButton->setPosition(btnX, btnY);

		ScreenUtils::resizeWidget(mShowSnapshotButton, size, BTN_RATIO);
		btnY = screenHeight - mShowSnapshotButton->getHeight(); // bottom
		btnX = screenWidth - mShowSnapshotButton->getWidth(); // right
		mShowSnapshotButton->setPosition(btnX, btnY);

		ScreenUtils::resizeWidget(mSetNextFlashModeButton, size, BTN_RATIO);
		btnY = 0; // top
		btnX = 0; // left
		mSetNextFlashModeButton->setPosition(btnX, btnY);

		// If we have only one camera we don't add the swap camera button.
		int nrOfCameras = mCamera->getNumberOfAvailableCameras();
		if ( nrOfCameras > 1 )
		{
			ScreenUtils::resizeWidget(mSetNextCameraButton, size, BTN_RATIO);
			btnY = 0; // top
			btnX = screenWidth - mSetNextCameraButton->getWidth(); // right
			mSetNextCameraButton->setPosition(btnX, btnY);
		}

		if ( mCamera->isZoomSupported() )
		{
			ScreenUtils::resizeWidget(mZoomInButton, size, BTN_RATIO);
			btnY = (screenHeight - mZoomInButton->getHeight())/2; // center
			btnX = 0; // left
			mZoomInButton->setPosition(btnX, btnY);

			ScreenUtils::resizeWidget(mZoomOutButton, size, BTN_RATIO);
			btnY = btnY + mZoomInButton->getHeight(); // under zoom in
			btnX = 0; // left
			mZoomOutButton->setPosition(btnX, btnY);
		}
	}


	void CameraScreen::show()
	{
		Screen::show();

		mCamera->startPreview();
		resetCameraPropeties();
	}


	int CameraScreen::showWithTransition(MAWScreenTransitionType screenTransitionType,
			uint screenTransitionDuration)
	{
		int returnCode = Screen::showWithTransition(screenTransitionType,
				screenTransitionDuration);
		if ( returnCode >= 0 )
		{
			mCamera->startPreview();
			resetCameraPropeties();
		}
		return returnCode;
	}


	void CameraScreen::takeSnapshot(MAHandle imageDataHandle)
	{
		if ( imageDataHandle > 0 )
		{
			mCamera->takeSnapshot( imageDataHandle );
		}
	}


	void CameraScreen::registerCameraListener(CameraSnapshotListener* cameraListener)
	{
		mCamera->addSnapshotListener(cameraListener);
	}


	void CameraScreen::unregisterCameraListener(CameraSnapshotListener* cameraListener)
	{
		mCamera->removeSnapshotListener(cameraListener);
	}


	void CameraScreen::showSnapshotInProgress()
	{
		toogleShowSnapshotButton(false);
		mTakeSnapshotButton->setEnabled(false);

		mActivityIndicator->show();
	}


	void CameraScreen::hideSnapshotInProgress(bool snapshotIsValid)
	{
		toogleShowSnapshotButton(snapshotIsValid);
		mTakeSnapshotButton->setEnabled(true);

		mActivityIndicator->hide();
	}


	void CameraScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mTakeSnapshotButton)
		{
			mObserver.snapshotRequested();
		}
		else if ( button == mShowSnapshotButton )
		{
			mCamera->stopPreview();
			mObserver.snapshotDisplayRequested();
		}
		else if ( button == mSetNextFlashModeButton )
		{
			activateNextFlashMode();
		}
		else if ( button == mSetNextCameraButton )
		{
			activateNextCamera();
		}
		else if ( button == mZoomInButton )
		{
			zoomIn();
		}
		else if ( button == mZoomOutButton )
		{
			zoomOut();
		}
	}


	void CameraScreen::activateNextFlashMode()
	{
		uint nextFlashMode = mCurrentFlashMode + 1;
		int arraySize = sizeof(kFlashModes)/sizeof(kFlashModes[0]);

		nextFlashMode = nextFlashMode % arraySize;

		// We need to get the next available flash mode.
		while ( false == mCamera->setFlashMode(kFlashModes[nextFlashMode]) )
		{
			nextFlashMode++;
			nextFlashMode = nextFlashMode % arraySize;
		}

		mSetNextFlashModeButton->setBackgroundImage(kFlashModesImg[nextFlashMode]);
		mSetNextFlashModeButton->setPressedImage(kFlashModesImgPressed[nextFlashMode]);
		mCurrentFlashMode = nextFlashMode;
	}


	void CameraScreen::activateNextCamera()
	{
		// See which is the next camera index (in a circular manner).
		uint currentCameraIndex = mCamera->getCurrentCameraIndex();
		currentCameraIndex++;

		uint nextCameraIndex = currentCameraIndex % mCamera->getNumberOfAvailableCameras();
		mCamera->setCurrentCameraByIndex(nextCameraIndex);

		if ( !mCamera->isFlashSupported() )
		{
			// if flash not supported disable the flash button.
			mSetNextFlashModeButton->setEnabled(false);
			mSetNextFlashModeButton->setBackgroundImage(kFlashModesImg[FLASH_OFF]);
			// Pressed is not handled so no pressed image needed.
			mSetNextFlashModeButton->setPressedImage(kFlashModesImg[FLASH_OFF]);
		}
		else
		{
			// set/restore flash mode.
			mSetNextFlashModeButton->setEnabled(true);
			mCamera->setFlashMode(kFlashModes[mCurrentFlashMode]);
			mSetNextFlashModeButton->setBackgroundImage(kFlashModesImg[mCurrentFlashMode]);
			mSetNextFlashModeButton->setPressedImage(kFlashModesImgPressed[mCurrentFlashMode]);
		}
	}


	void CameraScreen::zoomIn()
	{
		uint maxZoomLevel = mCamera->getMaxZoomLevel();
		uint currentZoomLevel = mCamera->getZoomLevel();
		uint zoomStep = (uint)(ZOOM_STEP_RATIO * maxZoomLevel);

		unsigned int nextZoomLevel = currentZoomLevel + zoomStep;
		if ( nextZoomLevel > maxZoomLevel )
		{
			nextZoomLevel = maxZoomLevel;
		}
		mCamera->setZoomLevel(nextZoomLevel);
	}


	void CameraScreen::zoomOut()
	{
		uint maxZoomLevel = mCamera->getMaxZoomLevel();
		uint currentZoomLevel = mCamera->getZoomLevel();
		uint zoomStep = (uint)(ZOOM_STEP_RATIO * maxZoomLevel);

		int nextZoomLevel = currentZoomLevel - zoomStep;
		if ( 0 > nextZoomLevel )
		{
			nextZoomLevel = 0;
		}
		mCamera->setZoomLevel(nextZoomLevel);
	}

	void CameraScreen::toogleShowSnapshotButton(bool enabled)
	{
		if ( enabled )
		{
			// Use enabled images
			mShowSnapshotButton->setBackgroundImage(RES_SHOW_PICTURE_IMG);
			mShowSnapshotButton->setPressedImage(RES_SHOW_PICTURE_IMG_PRESSED);
		}
		else
		{
			// Use disabled images
			mShowSnapshotButton->setBackgroundImage(RES_SHOW_PICTURE_IMG_DISABLED);
			mShowSnapshotButton->setPressedImage(RES_SHOW_PICTURE_IMG_DISABLED);
		}
		mShowSnapshotButton->setEnabled(enabled);
	}
} // MoSyncCamera
