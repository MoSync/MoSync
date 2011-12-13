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
	mImage->setHeight(this->getHeight());
	mImage->setWidth(this->getWidth());
	mImage->setScaleMode(IMAGE_SCALE_XY);
	mainLayout->addChild(mImage);
//	Screen::setMainWidget(mImage);
}
