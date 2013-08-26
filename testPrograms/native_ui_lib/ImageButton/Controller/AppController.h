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

#ifndef IMAGE_BUTTON_APP_CONTROLLER_H_
#define IMAGE_BUTTON_APP_CONTROLLER_H_

#include "../View/ImagePropertiesScreenListener.h"
#include "../View/PressedImageScreenListener.h"

namespace NativeUI
{
	class TabScreen;
}

// Forward declarations for Wormhole namespace classes.
namespace Wormhole
{
	class FileUtil;
}

namespace ImageButtonTest
{

	class TestScreen;
	class ImagePropertiesScreen;
	class PressedImageScreen;

	class AppController:
		public ImagePropertiesScreenListener,
		public PressedImageScreenListener
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
		 * Display the foreground image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectForegroundImageFromPath(const MAUtil::String& path);

		/**
		 * Display the foreground image from resources.
		 */
		virtual void selectForegroundImageFromHandle();

		/**
		 * Show previous set foreground image path.
		 */
		virtual void showForegroundImagePath();

		/**
		 * Display the background image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectBackgroundImageFromPath(const MAUtil::String& path);

		/**
		 * Display the background image from resources.
		 */
		virtual void selectBackgroundImageFromHandle();

		/**
		 * Show previous set background image path.
		 */
		virtual void showBackgroundImagePath();

		/**
		 * Display the pressed image from a given file path.
		 * @param path Image file path.
		 */
		virtual void selectPressedImageFromPath(const MAUtil::String& path);

		/**
		 * Display the pressed image from resources.
		 */
		virtual void selectPressedImageFromHandle();

		/**
		 * Show previous set pressed image handle.
		 */
		virtual void showPressedImageHandle();

		/**
		 * Show previous set pressed image path.
		 */
		virtual void showPressedImagePath();

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
		ImagePropertiesScreen* mImagePropertiesScreen;

		/**
		 * Third tab screen.
		 */
		PressedImageScreen* mPressedImageScreen;

		/**
		 * Used to extract LocalFiles.bin
		 */
		Wormhole::FileUtil* mFileUtil;
	};

} // end of ImageButtonTest

#endif /* IMAGE_BUTTON_APP_CONTROLLER_H_ */
