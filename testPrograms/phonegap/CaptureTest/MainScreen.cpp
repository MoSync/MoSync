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

#define INVALID_HANDLE -1

/**
 * @file MainScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Stack screen used for displaying screens.
 */

#include <conprint.h>

#include "MainScreen.h"
#include "SettingsScreen.h"
#include "VideoQualityScreen.h"
#include "ImageScreen.h"
#include "VideoViewScreen.h"
#include "FlashModeScreen.h"
#include "Util.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	StackScreen(),
	mSettingsScreen(NULL),
	mVideoQualityScreen(NULL),
	mImageScreen(NULL),
	mVideoScreen(NULL),
	mFlashModeScreen(NULL),
	mLatestPictureTakenHandle(INVALID_HANDLE),
	mLatestVideoRecordedHandle(INVALID_HANDLE)
{
	mSettingsScreen = new SettingsScreen(*this);
	mVideoQualityScreen = new VideoQualityScreen();
	mImageScreen = new ImageScreen();
	mVideoScreen = new VideoViewScreen();
	if ( isIOS() )
	{
		mFlashModeScreen = new FlashModeScreen();
	}

	this->push(mSettingsScreen);

	Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
//	delete mSettingsScreen;
//	delete mVideoQualityScreen;
//	delete mImageScreen;
//	delete mVideoScreen;
//	delete mFlashModeScreen;

	Environment::getEnvironment().removeCustomEventListener(this);
}

/**
 * Used for showing the video quality screen.
 */
void MainScreen::showVideoQualityScreen()
{
	this->push(mVideoQualityScreen);
}

/**
 * Used for showing a screen containing a taken image.
 */
void MainScreen::showImageScreen()
{
	this->push(mImageScreen);
}

/**
 * Used for showing a screen containing the latest recorded video.
 */
void MainScreen::showVideoScreen()
{
	this->push(mVideoScreen);
}

/**
 * Used for showing the flash mode screen.
 */
void MainScreen::showFlashModeScreen()
{
	this->push(mFlashModeScreen);
}

/**
 * Called when a custom event is fired.
 * Used for listening capture events.
 * @param event The fired event.
 */
void MainScreen::customEvent(const MAEvent& event)
{
	int syscallResult;
	if (event.type == EVENT_TYPE_CAPTURE)
	{
		printf("MainScreen::customEvent");
		MACaptureEventData eventData= event.captureData;
		if (eventData.type == MA_CAPTURE_EVENT_TYPE_IMAGE)
		{
			printf("MainScreen::customEvent image received");

			// destroy the previous picture
			if (mLatestPictureTakenHandle != INVALID_HANDLE)
			{
				mLatestPictureTakenHandle = maCaptureDestroyData(
					mLatestPictureTakenHandle);
				printf("maCaptureDestroyData() = %d", syscallResult);
			}

			mLatestPictureTakenHandle = eventData.handle;
			mImageScreen->setImage(mLatestPictureTakenHandle);
			if ( isAndroid() )
			{
				char buf[MAX_BUF_SIZE];
				int res = maCaptureGetImagePath(mLatestPictureTakenHandle, buf, MAX_BUF_SIZE);
				printf("image file path = %s", buf);
				mSettingsScreen->setCapturedImagePath(buf);
			}
		}
		else if (eventData.type == MA_CAPTURE_EVENT_TYPE_VIDEO)
		{
			printf("MainScreen::customEvent video recorded.");

			// Destroy the previous video recorded.
			if (mLatestVideoRecordedHandle != INVALID_HANDLE)
			{
				mLatestVideoRecordedHandle = maCaptureDestroyData(
						mLatestVideoRecordedHandle);
				printf("maCaptureDestroyData() = %d", syscallResult);
			}

			// Get video file path.
			mLatestVideoRecordedHandle = eventData.handle;
			char buf[MAX_BUF_SIZE];
			syscallResult = maCaptureGetVideoPath(
				mLatestVideoRecordedHandle, buf, MAX_BUF_SIZE);
			printf("maCaptureGetVideoPath() = %d", mLatestVideoRecordedHandle);
			printf("video file path = %s", buf);
			mVideoScreen->setVideoFilePath(buf);
		}
	}
}
