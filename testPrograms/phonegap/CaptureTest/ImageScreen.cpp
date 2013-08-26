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
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a image.
 */

#include "ImageScreen.h"

/**
 * Constructor.
 */
ImageScreen::ImageScreen() :
	Screen(),
	mImage(NULL)
{
	this->createMainLayout();
}

/**
 * Destructor.
 */
ImageScreen::~ImageScreen()
{

}

/**
 * Set currently displayed image.
 * @param handle Handle to a image.
 */
void ImageScreen::setImage(MAHandle handle)
{
	mImage->setImage(handle);
}

/**
 * Creates and adds main layout to the screen.
 */
void ImageScreen::createMainLayout()
{
	HorizontalLayout* mainLayout = new HorizontalLayout();
	Screen::setMainWidget(mainLayout);

	mImage = new Image();
//	mImage->setHeight(this->getHeight());
//	mImage->setWidth(this->getWidth());
	mImage->fillSpaceHorizontally();
	mImage->fillSpaceVertically();
	mImage->setScaleMode(IMAGE_SCALE_XY);
	mainLayout->addChild(mImage);
}
