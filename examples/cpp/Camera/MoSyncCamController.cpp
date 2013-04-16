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
 * @file MoSyncCamController.cpp
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Application's main controller.
 */

#include <NativeUI/Screen.h>
#include <MAUtil/Environment.h>

#include "MoSyncCamController.h"
#include "Screens/ScreenUtils.h"

namespace MoSyncCamera
{
	MoSyncCamController::MoSyncCamController():
		mCameraScreen(NULL),
		mImageViewerScreen(NULL),
		mForwardTransition(MAW_TRANSITION_TYPE_NONE),
		mBackwardTransition(MAW_TRANSITION_TYPE_NONE),
		mDisplayedScreen(0)
	{
		mCameraScreen = new CameraScreen(*this);
		mImageViewerScreen = new ImageViewerScreen(*this);

		setScreenTransitions();
	}


	MoSyncCamController::~MoSyncCamController()
	{
		delete mCameraScreen;
		delete mImageViewerScreen;
	}


	void MoSyncCamController::displayMainScreen()
	{
		if ( !isDisplayed(*mCameraScreen) )
		{
			mCameraScreen->show();

			setCurrentScreen(*mCameraScreen);
		}
	}


	void MoSyncCamController::displaySnapshot(const MAHandle& imageDataHandle)
	{
		if ( !isDisplayed(*mImageViewerScreen) )
		{
			mImageViewerScreen->setImageWithData(imageDataHandle);
			mImageViewerScreen->showWithTransition(mForwardTransition, SCREEN_TRANSITION_DURATION);

			setCurrentScreen(*mImageViewerScreen);
		}
	}


	void MoSyncCamController::dismissSnapshot()
	{
		if ( !isDisplayed(*mCameraScreen) )
		{
			mCameraScreen->showWithTransition(mBackwardTransition, SCREEN_TRANSITION_DURATION);

			setCurrentScreen(*mCameraScreen);
		}
	}


	MAWScreenTransitionType MoSyncCamController::setScreenTransitions()
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

	void MoSyncCamController::setCurrentScreen( const NativeUI::Screen& currentScreen )
	{
		mDisplayedScreen = currentScreen.getWidgetHandle();
	}

	bool MoSyncCamController::isDisplayed( const NativeUI::Screen& currentScreen )
	{
		return mDisplayedScreen == currentScreen.getWidgetHandle();
	}
} // MoSyncCamera
