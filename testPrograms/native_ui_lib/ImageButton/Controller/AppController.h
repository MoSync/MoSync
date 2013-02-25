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
