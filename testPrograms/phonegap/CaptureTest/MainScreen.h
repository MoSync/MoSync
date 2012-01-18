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
