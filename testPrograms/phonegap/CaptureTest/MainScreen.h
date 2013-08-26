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
 * @file MainScreen.h
 * @author Bogdan Iusco
 *
 * @brief Stack screen used for displaying screens.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>
#include <MAUtil/Environment.h>

#include "ScreenListeners.h"

using namespace NativeUI;
using namespace Ads;
using namespace MAUtil;

// Forward declarations
class SettingsScreen;
class VideoQualityScreen;
class ImageScreen;
class VideoViewScreen;
class FlashModeScreen;

/**
 * @brief Stack screen used for displaying screens.
 */
class MainScreen:
	public StackScreen,
	public SettingsScreenListener,
	public CustomEventListener
{
public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	virtual ~MainScreen();

private:
	/**
	 * Used for showing the video quality screen.
	 */
	virtual void showVideoQualityScreen();

	/**
	 * Used for showing a screen containing a taken image.
	 */
	virtual void showImageScreen();

	/**
	 * Used for showing a screen containing the latest recorded video.
	 */
	virtual void showVideoScreen();

	/**
	 * Used for showing the flash mode screen.
	 */
	virtual void showFlashModeScreen();

	/**
	 * Called when a custom event is fired.
	 * Used for listening capture events.
	 * @param event The fired event.
	 */
	virtual void customEvent(const MAEvent& event);

private:
	/**
	 * Used for displaying setting for capture API.
	 */
	SettingsScreen* mSettingsScreen;

	/**
	 * Used for displaying video quality options.
	 */
	VideoQualityScreen* mVideoQualityScreen;

	/**
	 * Used for displaying a taken photo.
	 */
	ImageScreen* mImageScreen;

	/**
	 * Used for playing the latest recorded video.
	 */
	VideoViewScreen* mVideoScreen;

	/**
	 * Used for displaying flash mode options.
	 */
	FlashModeScreen* mFlashModeScreen;

	/**
	 * Handle to the latest picture taken.
	 */
	int mLatestPictureTakenHandle;

	/**
	 * Handle to the latest video recorded.
	 */
	int mLatestVideoRecordedHandle;
};

#endif /* MAINSCREEN_H_ */
