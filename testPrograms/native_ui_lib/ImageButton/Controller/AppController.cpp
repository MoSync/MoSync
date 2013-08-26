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
#include "../View/ImagePropertiesScreen.h"
#include "../View/PressedImageScreen.h"
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
		mPressedImageScreen(NULL),
		mFileUtil(NULL)
	{
		mTabScreen = new NativeUI::TabScreen();
		mTestScreen = new TestScreen();
		mImagePropertiesScreen = new ImagePropertiesScreen(*this);
		mPressedImageScreen = new PressedImageScreen(*this);

		mTabScreen->addTab(mTestScreen);
		mTabScreen->addTab(mImagePropertiesScreen);
		mTabScreen->addTab(mPressedImageScreen);

		mTabScreen->show();

		mFileUtil = new Wormhole::FileUtil();
		// Extract bundled files to the local file system.
		mFileUtil->extractLocalFiles();

		char dirPath[BUFFER_SIZE];
		maGetSystemProperty("mosync.path.local", dirPath, BUFFER_SIZE);

		char filePath[BUFFER_SIZE];
		sprintf(filePath, "%sForegroundPath.png", dirPath);
		mImagePropertiesScreen->setDisplayedForegoundImagePath(filePath);

		sprintf(filePath, "%sBackgroundPath.png", dirPath);
		mImagePropertiesScreen->setDisplayedBackgoundImagePath(filePath);

		sprintf(filePath, "%sPressedPath.png", dirPath);
		mPressedImageScreen->setDisplayedPressedImagePath(filePath);
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
		mTestScreen->setImageButtonBackground(RES_IMAGE_BACKGROUND);
	}

	/**
	 * Show previous set background image path.
	 */
	void AppController::showBackgroundImagePath()
	{
		MAUtil::String text = mTestScreen->getImageButtonBackgroundPath();
		maAlert(
			"The background image path is",
			text.c_str(),
			"OK",
			NULL,
			NULL);
	}

	/**
	 * Display the pressed image from a given file path.
	 * @param path Image file path.
	 */
	void AppController::selectPressedImageFromPath(const MAUtil::String& path)
	{
		mTestScreen->setImageButtonPressed(path);
	}

	/**
	 * Display the pressed image from resources.
	 */
	void AppController::selectPressedImageFromHandle()
	{
		mTestScreen->setImageButtonPressed(RES_IMAGE_PRESSED);
	}

	/**
	 * Show previous set pressed image handle.
	 */
	void AppController::showPressedImageHandle()
	{
		int handle = mTestScreen->getImageButtonPressedHandle();
		MAUtil::String text = MAUtil::integerToString(handle);
		maAlert(
			"The pressed image handle is",
			text.c_str(),
			"OK",
			NULL,
			NULL);
	}

	/**
	 * Show previous set pressed image path.
	 */
	void AppController::showPressedImagePath()
	{
		MAUtil::String text = mTestScreen->getImageButtonPressedPath();
		maAlert(
			"The pressed image path is",
			text.c_str(),
			"OK",
			NULL,
			NULL);
	}

} // end of ImageButtonTest
