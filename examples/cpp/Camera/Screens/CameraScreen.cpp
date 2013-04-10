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
			mToggleFlashModeButton(NULL),
			mSwapCameraButton(NULL),
			mZoomInButton(NULL),
			mZoomOutButton(NULL),
			mLastSnapshotDataHandle(0),
			mCurrentFlashMode(0)
	{
		createUI();
	}


	CameraScreen::~CameraScreen()
	{
		mCamera->stopPreview();
		mTakeSnapshotButton->removeButtonListener(this);
		mShowSnapshotButton->removeButtonListener(this);
		mToggleFlashModeButton->removeButtonListener(this);
		mSwapCameraButton->removeButtonListener(this);
		mZoomInButton->removeButtonListener(this);
		mZoomOutButton->removeButtonListener(this);
		if ( 0 < mLastSnapshotDataHandle )
		{
			maDestroyPlaceholder(mLastSnapshotDataHandle);
		}
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
		mCamera->setCameraController(this);
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
		mToggleFlashModeButton = new NativeUI::ImageButton();
		ScreenUtils::setupImageButton(mToggleFlashModeButton,
				this, RES_FLASH_AUTO_IMG, RES_FLASH_AUTO_IMG_PRESSED);
		mMainLayout->addChild(mToggleFlashModeButton);

		// If there is not flash support on the default camera make the button unavailable.
		if ( !mCamera->isFlashSupported() )
		{
			// If flash not supported disable the flash button.
			mToggleFlashModeButton->setEnabled(false);
			mToggleFlashModeButton->setBackgroundImage(kFlashModesImg[FLASH_OFF]);
			// Pressed is not handled so no pressed image needed.
			mToggleFlashModeButton->setPressedImage(kFlashModesImg[FLASH_OFF]);
		}

		// If we have only one camera we don't add the swap camera button.
		uint nrOfCameras = mCamera->getNumberOfAvailableCameras();
		if ( nrOfCameras > 1 )
		{
			// camera source button
			mSwapCameraButton = new NativeUI::ImageButton();
			ScreenUtils::setupImageButton(mSwapCameraButton,
					this, RES_CAMERA_SWAP_IMG, RES_CAMERA_SWAP_IMG_PRESSED);
			mMainLayout->addChild(mSwapCameraButton);
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

		ScreenUtils::resizeWidget(mToggleFlashModeButton, size, BTN_RATIO);
		btnY = 0; // top
		btnX = 0; // left
		mToggleFlashModeButton->setPosition(btnX, btnY);

		// If we have only one camera we don't add the swap camera button.
		int nrOfCameras = mCamera->getNumberOfAvailableCameras();
		if ( nrOfCameras > 1 )
		{
			ScreenUtils::resizeWidget(mSwapCameraButton, size, BTN_RATIO);
			btnY = 0; // top
			btnX = screenWidth - mSwapCameraButton->getWidth(); // right
			mSwapCameraButton->setPosition(btnX, btnY);
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


	void CameraScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mTakeSnapshotButton)
		{
			captureSnapshot();
		}
		else if ( button == mShowSnapshotButton && (0 < mLastSnapshotDataHandle) )
		{
			// Only if we have a valid snapshot.
			mCamera->stopPreview();
			mObserver.displaySnapshot(mLastSnapshotDataHandle);
		}
		else if ( button == mToggleFlashModeButton )
		{
			toogleFlashMode();
		}
		else if ( button == mSwapCameraButton )
		{
			swapCameras();
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


	void CameraScreen::captureSnapshot()
	{
		toogleShowSnapshotButton(false);
		mTakeSnapshotButton->setEnabled(false);

		// Destroy the previous snapshot and ask for another.
		if ( 0 < mLastSnapshotDataHandle )
		{
			maDestroyPlaceholder(mLastSnapshotDataHandle);
			mLastSnapshotDataHandle = 0;
		}
		mActivityIndicator->show();

		mLastSnapshotDataHandle = maCreatePlaceholder();
		mCamera->takeSnapshot( mLastSnapshotDataHandle );
	}


	void CameraScreen::toogleFlashMode()
	{
		// Toggle the flash modes
		uint nextFlashMode = mCurrentFlashMode + 1;
		int arraySize = sizeof(kFlashModes)/sizeof(kFlashModes[0]);

		nextFlashMode = nextFlashMode % arraySize;

		// We need to get the next available flash mode.
		while ( false == mCamera->setFlashMode(kFlashModes[nextFlashMode]) )
		{
			nextFlashMode++;
			nextFlashMode = nextFlashMode % arraySize;
		}

		mToggleFlashModeButton->setBackgroundImage(kFlashModesImg[nextFlashMode]);
		mToggleFlashModeButton->setPressedImage(kFlashModesImgPressed[nextFlashMode]);
		mCurrentFlashMode = nextFlashMode;
	}


	void CameraScreen::swapCameras()
	{
		// See which is the next camera index (in a circular manner).
		uint currentCameraIndex = mCamera->getCurrentCameraIndex();
		currentCameraIndex++;

		uint nextCameraIndex = currentCameraIndex % mCamera->getNumberOfAvailableCameras();
		mCamera->setCurrentCameraByIndex(nextCameraIndex);

		if ( !mCamera->isFlashSupported() )
		{
			// if flash not supported disable the flash button.
			mToggleFlashModeButton->setEnabled(false);
			mToggleFlashModeButton->setBackgroundImage(kFlashModesImg[FLASH_OFF]);
			// Pressed is not handled so no pressed image needed.
			mToggleFlashModeButton->setPressedImage(kFlashModesImg[FLASH_OFF]);
		}
		else
		{
			// set/restore flash mode.
			mToggleFlashModeButton->setEnabled(true);
			mCamera->setFlashMode(kFlashModes[mCurrentFlashMode]);
			mToggleFlashModeButton->setBackgroundImage(kFlashModesImg[mCurrentFlashMode]);
			mToggleFlashModeButton->setPressedImage(kFlashModesImgPressed[mCurrentFlashMode]);
		}
	}


	void CameraScreen::zoomIn()
	{
		uint maxZoomLevel = mCamera->getMaxZoomLevel();
		uint currentZoomLevel = mCamera->getZoomLevel();
		uint zoomStep = (uint)(ZOOM_STEP_RATIO * maxZoomLevel);

		int nextZoomLevel = currentZoomLevel + zoomStep;
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


	void CameraScreen::snapshotSucceeded( const NativeUI::CameraSnapshotData& imageData )
	{
		if ( mLastSnapshotDataHandle == imageData.dataHandle )
		{
			toogleShowSnapshotButton(true);
		}
		mTakeSnapshotButton->setEnabled(true);
		mSwapCameraButton->setEnabled(true);
		mActivityIndicator->hide();
	}


	void CameraScreen::snapshotFailed( const MAHandle& snapshotHandle )
	{
		maAlert("Camera", "Snapshot failed", "OK", NULL, NULL);

		if ( mLastSnapshotDataHandle > 0 )
		{
			toogleShowSnapshotButton(true);
		}
		mTakeSnapshotButton->setEnabled(true);
		mActivityIndicator->hide();
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
} // CameraDemo
