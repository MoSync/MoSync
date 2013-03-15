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
