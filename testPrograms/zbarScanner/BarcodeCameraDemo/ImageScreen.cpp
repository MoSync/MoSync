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
 * @file ImageScreen.cpp
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */

#include "ImageScreen.h"

#include <mastdlib.h>
#include <mavsprintf.h>
#include "WidgetUtil.h"

using namespace NativeUI;

/**
 * This method implements a button event listener.
 * Button click events are sent here.
 */
void ImageScreen::buttonClicked(Widget* button)
{
	if(mOKButton == button)
	{
		mStackScreen->pop();
	}
}

/**
 * Lazy initializations
 */
void ImageScreen::initialize(StackScreen* stackScreen)
{
	mImageHandle = 0;
	mStackScreen = stackScreen;
	createUI();
}

/**
 * Create the UI and components required
 */
void ImageScreen::createUI()
{
	// Create a Native UI screen. As the screen is a member
	// variable (also called instance variable) we have
	// prefixed the variable name with "m".
	mScreen = new Screen();

	mScreen->setTitle("Captured Image");
	// Create the screen's main layout widget.
	mMainLayoutWidget = new VerticalLayout();

	// Make the layout fill the entire screen.
	mMainLayoutWidget->fillSpaceHorizontally();
	mMainLayoutWidget->fillSpaceVertically();

	// Add the layout as the root of the screen's widget tree.
	mScreen->setMainWidget(mMainLayoutWidget);

	mImageWidget = new Image();

	mImageWidget->fillSpaceHorizontally();
	mImageWidget->fillSpaceVertically();

	mMainLayoutWidget->addChild(mImageWidget);

	mOKButton = new Button();

	mOKButton->fillSpaceHorizontally();
	mOKButton->wrapContentVertically();
	mOKButton->setText("Back to Main Screen");
	mOKButton->addButtonListener(this);

	mMainLayoutWidget->addChild(mOKButton);
}


/**
 * Pushes the image screen to the screen
 */
void ImageScreen::pushImageScreen()
{
	mStackScreen->push(mScreen);
}

/**
 * sets the captured buffer handle to the image handle that
 * can be shown on the screen
 */
void ImageScreen::setImageDataHandle(MAHandle dataHandle)
{
	if(mImageHandle != 0) {
		maDestroyObject(mImageHandle);
	}
	mImageHandle = maCreatePlaceholder();
	int res = maCreateImageFromData(
			mImageHandle,
				dataHandle,
				0,
				maGetDataSize(dataHandle));
		if(res != RES_OK)
	{
		maPanic(res, "failed to create the image");
	}
		//Set the image to the image widget
	mImageWidget->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
	mImageWidget->setImage(mImageHandle);
}

void ImageScreen::setImageDataRaw(int* data, int width, int height)
{
	if(mImageHandle != 0) {
		maDestroyObject(mImageHandle);
	}

	mImageHandle = maCreatePlaceholder();

	MAExtent ext = EXTENT(width, height);

	int res = maCreateImageRaw(mImageHandle, data, ext, 0);
	if(res != RES_OK)
	{
		maPanic(res, "failed to create the image");
	}

	mImageWidget->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
	mImageWidget->setImage(mImageHandle);

}
