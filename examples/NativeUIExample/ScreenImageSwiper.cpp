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
 * @file ScreenImageSwiper.h
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#include "MAHeaders.h"
#include "ScreenImageSwiper.h"

/**
 * TODO: Add comment.
 */
Screen* ScreenImageSwiper::create()
{
	ScreenImageSwiper* screen = new ScreenImageSwiper();
	screen->createUI();
	return screen;
}

ScreenImageSwiper::ScreenImageSwiper() :
	currentImage(0)
{
}

void ScreenImageSwiper::createUI()
{
	setTitle("Images");
	if (WidgetManager::isAndroid())
	{
		setIcon(RES_TAB_ICON_IMAGE_SWIPER_ANDROID);
	}
	else
	{
		setIcon(RES_TAB_ICON_IMAGE_SWIPER);
	}

	getScreenSize();

	mainLayout = new VerticalLayout();

	imagesLayout = new RelativeLayout();
	imagesLayout->setBackgroundColor(0xF0F0F0);

	loadImages(mScreenWidth);

	MAExtent size = maGetImageSize(images[0]->handle);

	int imageWidth = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;
	int imageHeight = imageWidth * EXTENT_Y(size) / EXTENT_X(size);

	int startX = mScreenWidth / 6 - imageWidth - mScreenWidth / 12;
	for (int i=0; i<imagesSize; i++)
	{
		images[i]->posX = startX;
		images[i]->posY = mScreenHeight / 5;
		images[i]->width = imageWidth;

//		imageWidgets[LEFT_IMAGE].shadow = new RelativeLayout();
//		imageWidgets[LEFT_IMAGE].shadow->setBackgroundColor(0);
//		imageWidgets[LEFT_IMAGE].shadow->setProperty(MAW_WIDGET_ALPHA, "0.2");
//		imageWidgets[LEFT_IMAGE].shadow->setSize(imageWidth, imageHeight);
//		imageWidgets[LEFT_IMAGE].shadow->setPosition(imageWidgets[LEFT_IMAGE].posX + SHADOW_OFFSET, imageWidgets[LEFT_IMAGE].posY + SHADOW_OFFSET);
//
//		imagesLayout->addChild(imageWidgets[LEFT_IMAGE].shadow);

		images[i]->setResource();
		images[i]->setSize(imageWidth, imageHeight);
		images[i]->setPosition(images[i]->posX, images[i]->posY);

		imagesLayout->addChild(images[i]);
		startX += imageWidth + mScreenWidth / 6;
	}

	labelLayout = new Label();
	labelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	labelLayout->setBackgroundColor(0x111111);
	labelLayout->setText("Image X");
	labelLayout->setFontColor(0xFFFFFF);
	labelLayout->centerTextHorizontally();
	labelLayout->centerTextVertically();

	mainLayout->addChild(imagesLayout);
	mainLayout->addChild(labelLayout);

	setMainWidget(mainLayout);
}

/*
 * Method for getting the screen size inside the static members sScreenWidth and sScreenHeight
 */
void ScreenImageSwiper::getScreenSize()
{
	/// Get screen dimensions.
	MAExtent size = maGetScrSize();

	/// Extract the screen width
	mScreenWidth = EXTENT_X(size);

	/// Extract the screen height
	mScreenHeight = EXTENT_Y(size);
}

void ScreenImageSwiper::loadImages(int screenWidth)
{
	int firstImage = RES_FIRST_IMAGE_LARGE;
	int lastImage = RES_LAST_IMAGE_LARGE;
	if (screenWidth <= SMALL_SCREEN_RESOLUTION)
	{
		firstImage = RES_FIRST_IMAGE_SMALL;
		lastImage = RES_LAST_IMAGE_SMALL;
	}
	else if (screenWidth <= MEDIUM_SCREEN_RESOLUTION)
	{
		firstImage = RES_FIRST_IMAGE_MEDIUM;
		lastImage = RES_LAST_IMAGE_MEDIUM;
	}
	imagesSize = lastImage - firstImage - 1;
	images = new ScreenImage*[imagesSize];
	for (int i = 0; i < imagesSize; i++)
	{
		images[i] = new ScreenImage();
		int resID = firstImage + i + 1;
		images[i]->handle = maCreatePlaceholder();
		maCreateImageFromData(images[i]->handle, resID, 0, maGetDataSize(resID));
		//readString(resID + 1, 0, images[i].text);
	}
}

/**
 * Handle pointer presses.
 */
void ScreenImageSwiper::handlePointerPressed(MAPoint2d p)
{
	mPointerX = p.x;
}

/**
 * Handle pointer moves.
 */
void ScreenImageSwiper::handlePointerMoved(MAPoint2d p)
{
	int offset = p.x - mPointerX;

	if (offset < 0)
	{
		if (images[imagesSize - 1]->posX + images[imagesSize - 1]->width < mScreenWidth)
		{
			return;
		}
	}
	else
	{
		if (images[0]->posX > 0)
		{
			return;
		}
	}

	for (int i=0; i<imagesSize; i++)
	{
		images[i]->posX += offset;

		images[i]->setPosition(images[i]->posX, images[i]->posY);
	}
	mPointerX = p.x;
}

/**
 * Handle pointer releases.
 */
void ScreenImageSwiper::handlePointerReleased(MAPoint2d p)
{
}

//bool ScreenImageSwiper::readString(int resID, int pos, MAUtil::String &result)
//{
//	byte stringLen = 0;
//	maReadData(resID, (void*) &stringLen, pos, sizeof(byte));
//
//	if (stringLen > maGetDataSize(resID) || stringLen <= 0)
//		return false;
//
//	pos += sizeof(byte);
//	result.resize(stringLen);
//
//	maReadData(resID, (void*) result.c_str(), pos, stringLen);
//	pos += stringLen;
//	return true;
//}
