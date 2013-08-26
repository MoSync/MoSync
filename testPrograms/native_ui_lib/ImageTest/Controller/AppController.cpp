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
		maAlert("Image path is",
				path.c_str(),
				"OK",
				NULL,
				NULL);
	}
}
