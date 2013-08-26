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
 * @file MoCameraController.cpp
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Application's main controller.
 */

#include <NativeUI/Screen.h>

#include "MoCameraController.h"
#include "Screens/ScreenUtils.h"

#define SNAPSHOT_TITLE "Capture snapshot"
#define SNAPSHOT_FAILED_STR "Please try again"

#define SAVE_IMAGE_TITLE "Save to gallery"
#define SAVE_IMAGE_FAILED_STR "Please try again"
#define SAVE_IMAGE_SUCCEEDED_STR "Completed"
#define IMAGE_NAME "MoSyncCameraPic"

#define ALERT_OK_STR "OK"

namespace MoSyncCamera
{
	MoCameraController::MoCameraController():
		mCameraScreen(NULL),
		mImageViewerScreen(NULL),
		mForwardTransition(MAW_TRANSITION_TYPE_NONE),
		mBackwardTransition(MAW_TRANSITION_TYPE_NONE),
		mDisplayedScreen(-1),
		mDisplayedImageHandle(0),
		mLastSnapshotDataHandle(0)
	{
		mCameraScreen = new CameraScreen(*this);
		mImageViewerScreen = new ImageViewerScreen(*this);

		mCameraScreen->registerCameraListener(this);
		MAUtil::Environment::getEnvironment().addMediaExportListener(this);
		MAUtil::Environment::getEnvironment().addPointerListener(this);

		setScreenTransitions();
	}


	MoCameraController::~MoCameraController()
	{
		if ( mDisplayedImageHandle > 0 )
		{
			maDestroyPlaceholder(mDisplayedImageHandle);
		}

		if ( mLastSnapshotDataHandle > 0 )
		{
			maDestroyPlaceholder(mLastSnapshotDataHandle);
		}

		mCameraScreen->unregisterCameraListener(this);
		MAUtil::Environment::getEnvironment().removeMediaExportListener(this);
		MAUtil::Environment::getEnvironment().removePointerListener(this);

		delete mCameraScreen;
		delete mImageViewerScreen;
	}


	void MoCameraController::displayMainScreen()
	{
		if ( !isDisplayed(*mCameraScreen) )
		{
			mCameraScreen->show();

			setCurrentScreen(*mCameraScreen);
		}
	}


	void MoCameraController::snapshotDisplayRequested()
	{
		if ( !isDisplayed(*mImageViewerScreen) )
		{
			if ( mLastSnapshotDataHandle > 0 )
			{
				mDisplayedImageHandle = maCreatePlaceholder();

				maCreateImageFromData(
					mDisplayedImageHandle,
					mLastSnapshotDataHandle,
					0,
					maGetDataSize(mLastSnapshotDataHandle));

				mImageViewerScreen->setImageWithData(mDisplayedImageHandle);
				mImageViewerScreen->showWithTransition(mForwardTransition, SCREEN_TRANSITION_DURATION);

				setCurrentScreen(*mImageViewerScreen);
			}
		}
	}


	void MoCameraController::imageViewingDone()
	{
		if ( !isDisplayed(*mCameraScreen) )
		{
			mCameraScreen->showWithTransition(mBackwardTransition, SCREEN_TRANSITION_DURATION);

			if ( mDisplayedImageHandle > 0 )
			{
				maDestroyPlaceholder(mDisplayedImageHandle);
				mDisplayedImageHandle = 0;
			}

			setCurrentScreen(*mCameraScreen);
		}
	}


	void MoCameraController::snapshotRequested()
	{
		mCameraScreen->showSnapshotInProgress();

		if ( mLastSnapshotDataHandle > 0 )
		{
			maDestroyPlaceholder(mLastSnapshotDataHandle);
			mLastSnapshotDataHandle = 0;
		}
		mLastSnapshotDataHandle = maCreatePlaceholder();

		mCameraScreen->takeSnapshot(mLastSnapshotDataHandle);
	}


	void MoCameraController::snapshotFinished( const NativeUI::CameraSnapshotData& imageData )
	{
		bool snapshotIsAvailable = true;
		if ( imageData.resultCode != MA_CAMERA_RES_OK )
		{
			maAlert(SNAPSHOT_TITLE, SNAPSHOT_FAILED_STR, ALERT_OK_STR, NULL, NULL);
			snapshotIsAvailable = false;

			if ( mLastSnapshotDataHandle > 0 )
			{
				maDestroyPlaceholder(mLastSnapshotDataHandle);
				mLastSnapshotDataHandle = 0;
			}
		}
		mCameraScreen->hideSnapshotInProgress(snapshotIsAvailable);
	}


	void MoCameraController::exportImageToGalleryRequested()
	{
		if ( mDisplayedImageHandle > 0 )
		{
			mImageViewerScreen->toggleImageSavingInProgress(true);
			int returnCode = maSaveImageToDeviceGallery(mDisplayedImageHandle, IMAGE_NAME);
			if ( returnCode != MA_MEDIA_RES_OK )
			{
				mImageViewerScreen->toggleImageSavingInProgress(false);
				maAlert(SAVE_IMAGE_TITLE, SAVE_IMAGE_FAILED_STR, ALERT_OK_STR, NULL, NULL);
			}
		}
	}


	void MoCameraController::imageExportToGalleryFinished(
			const MAHandle& imageHandle,
			int resultCode)
	{
		MAUtil::String message = SAVE_IMAGE_SUCCEEDED_STR;

		if ( mDisplayedImageHandle != imageHandle &&
				resultCode != MA_MEDIA_RES_OK )
		{
			message = SAVE_IMAGE_FAILED_STR;
		}

		mImageViewerScreen->toggleImageSavingInProgress(false);
		maAlert(SAVE_IMAGE_TITLE, message.c_str(), ALERT_OK_STR, NULL, NULL);
	}


	void MoCameraController::handleBackPress()
	{
		if ( isDisplayed(*mImageViewerScreen) )
		{
			imageViewingDone();
		}
		else
		{
			// This will be replaced to "send to background" in the near future.
			maExit(0);
		}
	}


	void MoCameraController::pointerPressEvent(MAPoint2d point)
	{
		if ( isDisplayed(*mCameraScreen) )
		{
			mCameraScreen->triggerAutoFocus();
		}
	}


    void MoCameraController::pointerMoveEvent(MAPoint2d point)
    {}


    void MoCameraController::pointerReleaseEvent(MAPoint2d point)
    {}


	void MoCameraController::setScreenTransitions()
	{
		switch (MAUtil::Environment::getEnvironment().getCurrentPlatform())
		{
		case MAUtil::OS_IOS:
			mForwardTransition = MAW_TRANSITION_TYPE_FLIP_FROM_LEFT;
			mBackwardTransition = MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT;
			break;
		case MAUtil::OS_ANDROID:
			mForwardTransition = MAW_TRANSITION_TYPE_SLIDE_LEFT;
			mBackwardTransition = MAW_TRANSITION_TYPE_SLIDE_RIGHT;
			break;
		case MAUtil::OS_WIN:
			mForwardTransition = MAW_TRANSITION_TYPE_SWIVEL_IN;
			mBackwardTransition = MAW_TRANSITION_TYPE_SWIVEL_OUT;
			break;
		default:
			mForwardTransition = MAW_TRANSITION_TYPE_NONE;
			mBackwardTransition = MAW_TRANSITION_TYPE_NONE;
			break;
		}
	}

	void MoCameraController::setCurrentScreen( const NativeUI::Screen& currentScreen )
	{
		mDisplayedScreen = currentScreen.getWidgetHandle();
	}

	bool MoCameraController::isDisplayed( const NativeUI::Screen& currentScreen )
	{
		return (mDisplayedScreen == currentScreen.getWidgetHandle());
	}
} // MoSyncCamera
