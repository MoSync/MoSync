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
 * @brief File containing screen listeners.
 */

#ifndef SCREENLISTENERS_H_
#define SCREENLISTENERS_H_

/**
 * Listener for SettingsScreen.
 */
class SettingsScreenListener
{
public:
	/**
	 * Used for showing the video quality screen.
	 */
	virtual void showVideoQualityScreen() = 0;

	/**
	 * Used for showing a screen containing a taken image.
	 */
	virtual void showImageScreen() = 0;

	/**
	 * Used for showing a screen containing the latest recorded video.
	 */
	virtual void showVideoScreen() = 0;

	/**
	 * Used for showing the flash mode screen.
	 */
	virtual void showFlashModeScreen() = 0;
};


#endif /* SCREENLISTENERS_H_ */
