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
 * @file AppController.cpp
 * @author Bogdan Iusco
 *
 * @brief Application main controller.
 * Shows a TabScreen and makes the connection between the screens.
 */

#define BUFFER_SIZE 512

#include <conprint.h>
#include <NativeUI/TabScreen.h>
#include <Wormhole/FileUtil.h>

#include "../View/TestScreen.h"
#include "../View/ImagePropertiesScreen.h"
#include "AppController.h"
#include "MAHeaders.h"

namespace ImageButtonTest
{
	/**
	 * Constructor.
	 */
	AppController::AppController() :
		mTabScreen(NULL),
		mTestScreen(NULL),
		mImagePropertiesScreen(NULL),
		mFileUtil(NULL)
	{
		mTabScreen = new NativeUI::TabScreen();
		mTestScreen = new TestScreen();
		mImagePropertiesScreen = new ImagePropertiesScreen(*this);

		mTabScreen->addTab(mTestScreen);
		mTabScreen->addTab(mImagePropertiesScreen);

		mTabScreen->show();

		mFileUtil = new Wormhole::FileUtil();
		// Extract bundled files to the local file system.
		mFileUtil->extractLocalFiles();

		char buf[BUFFER_SIZE];
		maGetSystemProperty("mosync.path.local", buf, BUFFER_SIZE);

		sprintf(buf, "%sForegroundPath.png", buf);
		mImagePropertiesScreen->setDisplayedForegoundImagePath(buf);

		sprintf(buf, "%sBackgroundPath.png", buf);
		mImagePropertiesScreen->setDisplayedBackgoundImagePath(buf);
	}

	/**
	 * Destructor.
	 */
	AppController::~AppController()
	{
		delete mTabScreen;
		delete mFileUtil;
	}

	/**
	 * Display the foreground image from a given file path.
	 * @param path Image file path.
	 */
	void AppController::selectForegroundImageFromPath(const MAUtil::String& path)
	{
		mTestScreen->setImageButtonForeground(path);
	}

	/**
	 * Display the foreground image from resources.
	 */
	void AppController::selectForegroundImageFromHandle()
	{
		mTestScreen->setImageButtonForeground(RES_IMAGE_FOREGROUND);
	}

	/**
	 * Show previous set foreground image path.
	 */
	void AppController::showForegroundImagePath()
	{
		MAUtil::String text = mTestScreen->getImageButtonForegroundPath();
		maAlert(
			"The foreground image path is",
			text.c_str(),
			"OK",
			NULL,
			NULL);
	}

	/**
	 * Display the background image from a given file path.
	 * @param path Image file path.
	 */
	void AppController::selectBackgroundImageFromPath(const MAUtil::String& path)
	{
		mTestScreen->setImageButtonBackground(path);
	}

	/**
	 * Display the background image from resources.
	 */
	void AppController::selectBackgroundImageFromHandle()
	{
		mTestScreen->setImageButtonForeground(RES_IMAGE_BACKGROUND);
	}

	/**
	 * Show previous set background image path.
	 */
	void AppController::showBackgroundImagePath()
	{
		MAUtil::String text = mTestScreen->getImageButtonForegroundPath();
		maAlert(
			"The foreground image path is",
			text.c_str(),
			"OK",
			NULL,
			NULL);
	}

} // end of ImageButtonTest
