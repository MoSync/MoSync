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
 * @file PressedImageScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PressedImageScreen.
 */

#ifndef IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_
#define IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageButtonTest
{

	class PressedImageScreenListener
	{
	public:
		/**
		 * Display the pressed image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectPressedImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the pressed image from resources.
		 */
		virtual void selectPressedImageFromHandle() = 0;

		/**
		 * Show previous set pressed image handle.
		 */
		virtual void showPressedImageHandle() = 0;

		/**
		 * Show previous set pressed image path.
		 */
		virtual void showPressedImagePath() = 0;
	};

} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_PRESSED_IMAGE_SCREEN_LISTENER_H_ */
