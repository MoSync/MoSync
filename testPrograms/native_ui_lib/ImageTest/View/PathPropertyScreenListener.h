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
 * @file PathPropertyScreenListener.h
 * @author Bogdan Iusco.
 *
 * @brief Listener for PathPropertyScreen.
 */

#ifndef PATH_PROPERTY_SCREEN_LISTENER_H_
#define PATH_PROPERTY_SCREEN_LISTENER_H_

#include <MAUtil/String.h>

namespace ImageTest
{

	class PathPropertyScreenListener
	{
	public:
		/**
		 * Display a image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectImageFromPath(const MAUtil::String& path) = 0;

		/**
		 * Display the image from resources.
		 */
		virtual void selectImageFromHandle() = 0;

		/**
		 * Show previous set image path.
		 */
		virtual void showImagePath() = 0;
	};

}

#endif /* PATH_PROPERTY_SCREEN_LISTENER_H_ */
