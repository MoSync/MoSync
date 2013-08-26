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
 * @file AppController.h
 * @author Bogdan Iusco
 *
 * @brief Application main controller.
 * Shows a TabScreen and makes the connection between the screens.
 */

#ifndef IMAGE_TEST_APP_CONTROLLER_H_
#define IMAGE_TEST_APP_CONTROLLER_H_

#include "../View/PathPropertyScreenListener.h"

namespace NativeUI
{
	class TabScreen;
}

// Forward declarations for Wormhole namespace classes.
namespace Wormhole
{
	class FileUtil;
}

namespace ImageTest
{

	class TestScreen;
	class PathPropertyScreen;

	class AppController: public PathPropertyScreenListener
	{
	public:
		/**
		 * Constructor.
		 */
		AppController();

		/**
		 * Destructor.
		 */
		~AppController();

		/**
		 * Display a image from a given file path.
		 * From PathPropertyScreenListener.
		 * @param path Image file path.
		 */
		virtual void selectImageFromPath(const MAUtil::String& path);

		/**
		 * Display the image from resources.
		 * From PathPropertyScreenListener.
		 */
		virtual void selectImageFromHandle();

		/**
		 * Show previous set image path.
		 * From PathPropertyScreenListener.
		 */
		virtual void showImagePath();

	private:
		/**
		 * Application's tab screen.
		 */
		NativeUI::TabScreen* mTabScreen;

		/**
		 * First tab screen;
		 */
		TestScreen* mTestScreen;

		/**
		 * Second tab screen.
		 */
		PathPropertyScreen* mPathPropertyScreen;

		/**
		 * Used to extract LocalFiles.bin
		 */
		Wormhole::FileUtil* mFileUtil;
	};
} // end of ImageTest

#endif /* IMAGE_TEST_APP_CONTROLLER_H_ */
