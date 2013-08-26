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
 * @file ImagePropertiesScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PathPropertyScreen.
 */

#ifndef IMAGE_PROPERTIES_SCREEN_LISTENER_H_
#define IMAGE_PROPERTIES_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageButtonTest
{

	class ImagePropertiesScreenListener
	{
	public:
		/**
		 * Display the foreground image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectForegroundImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the foreground image from resources.
		 */
		virtual void selectForegroundImageFromHandle() = 0;

		/**
		 * Show previous set foreground image path.
		 */
		virtual void showForegroundImagePath() = 0;

		/**
		 * Display the background image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectBackgroundImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the background image from resources.
		 */
		virtual void selectBackgroundImageFromHandle() = 0;

		/**
		 * Show previous set background image path.
		 */
		virtual void showBackgroundImagePath() = 0;
	};

} // end of ImageButtonTest

#endif /* IMAGE_PROPERTIES_SCREEN_LISTENER_H_ */
