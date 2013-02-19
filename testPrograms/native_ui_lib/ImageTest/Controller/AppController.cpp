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
#include "../View/PathPropertyScreen.h"
#include "AppController.h"
#include "MAHeaders.h"

namespace ImageTest
{
	/**
	 * Constructor.
	 */
	AppController::AppController() :
		mTabScreen(NULL),
		mTestScreen(NULL),
		mPathPropertyScreen(NULL),
		mFileUtil(NULL)
	{
		mTabScreen = new NativeUI::TabScreen();
		mTestScreen = new TestScreen();
		mPathPropertyScreen = new PathPropertyScreen(*this);

		mTabScreen->addTab(mTestScreen);
		mTabScreen->addTab(mPathPropertyScreen);

		mTabScreen->show();

		mFileUtil = new Wormhole::FileUtil();
		// Extract bundled files to the local file system.
		mFileUtil->extractLocalFiles();

		char buf[BUFFER_SIZE];
		maGetSystemProperty("mosync.path.local", buf, BUFFER_SIZE);
		sprintf(buf, "%sImagePath.png", buf);
		MAUtil::String path = buf;
		printf("Image path is %s", path.c_str());
		mPathPropertyScreen->setDisplayedImagePath(path);
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
	 * Display a image from a given file path.
	 * From PathPropertyScreenListener.
	 * @param path Image file path.
	 */
	void AppController::selectImageFromPath(const MAUtil::String& path)
	{
		mTestScreen->showImageUsingFilePath(path);
	}

	/**
	 * Display the image from resources.
	 * From PathPropertyScreenListener.
	 */
	void AppController::selectImageFromHandle()
	{
		mTestScreen->showImageUsingHandle(RES_IMAGE);
	}

	/**
	 * Show previous set image path.
	 * From PathPropertyScreenListener.
	 */
	void AppController::showImagePath()
	{
		MAUtil::String path = mTestScreen->getImagePath();
		printf("imagePath is %s", path.c_str());
		maAlert("Image path is",
				path.c_str(),
				"OK",
				NULL,
				NULL);
	}
}
