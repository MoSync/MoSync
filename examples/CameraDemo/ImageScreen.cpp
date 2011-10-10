/*
Copyright (C) 2011 MoSync AB

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
