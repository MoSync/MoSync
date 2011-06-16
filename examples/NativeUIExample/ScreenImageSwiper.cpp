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
	imageWidgets = new ScreenImage[DISPLAYED_IMAGES];
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

	MAExtent size = maGetImageSize(images[0].handle);

	int imageWidth = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;
	int imageHeight = imageWidth * EXTENT_Y(size) / EXTENT_X(size);
	//leftImage
	imageWidgets[LEFT_IMAGE].index = 0;
	imageWidgets[LEFT_IMAGE].posX = mScreenWidth / 6 - imageWidth - mScreenWidth / 12;
	imageWidgets[LEFT_IMAGE].posY = mScreenHeight / 5;

	imageWidgets[LEFT_IMAGE].shadow = new RelativeLayout();
	imageWidgets[LEFT_IMAGE].shadow->setBackgroundColor(0);
	imageWidgets[LEFT_IMAGE].shadow->setProperty(MAW_WIDGET_ALPHA, "0.2");
	imageWidgets[LEFT_IMAGE].shadow->setSize(imageWidth, imageHeight);
	imageWidgets[LEFT_IMAGE].shadow->setPosition(imageWidgets[LEFT_IMAGE].posX + SHADOW_OFFSET, imageWidgets[LEFT_IMAGE].posY + SHADOW_OFFSET);

	imagesLayout->addChild(imageWidgets[LEFT_IMAGE].shadow);

	imageWidgets[LEFT_IMAGE].image = new Image();
	imageWidgets[LEFT_IMAGE].image->setResource(images[imageWidgets[LEFT_IMAGE].index].handle);
	imageWidgets[LEFT_IMAGE].image->setSize(imageWidth, imageHeight);
	imageWidgets[LEFT_IMAGE].image->setPosition(imageWidgets[LEFT_IMAGE].posX, imageWidgets[LEFT_IMAGE].posY);

	imagesLayout->addChild(imageWidgets[LEFT_IMAGE].image);

	//centerImage
	imageWidgets[CENTER_IMAGE].index = 1;
	imageWidgets[CENTER_IMAGE].posX = mScreenWidth / 6 + mScreenWidth / 12;
	imageWidgets[CENTER_IMAGE].posY = mScreenHeight / 5;

	imageWidgets[CENTER_IMAGE].shadow = new RelativeLayout();
	imageWidgets[CENTER_IMAGE].shadow->setBackgroundColor(0);
	imageWidgets[CENTER_IMAGE].shadow->setProperty(MAW_WIDGET_ALPHA, "0.2");
	imageWidgets[CENTER_IMAGE].shadow->setSize(imageWidth, imageHeight);
	imageWidgets[CENTER_IMAGE].shadow->setPosition(imageWidgets[CENTER_IMAGE].posX + SHADOW_OFFSET, imageWidgets[CENTER_IMAGE].posY + SHADOW_OFFSET);

	imagesLayout->addChild(imageWidgets[CENTER_IMAGE].shadow);

	imageWidgets[CENTER_IMAGE].image = new Image();
	imageWidgets[CENTER_IMAGE].image->setResource(images[imageWidgets[CENTER_IMAGE].index].handle);
	imageWidgets[CENTER_IMAGE].image->setSize(imageWidth, imageHeight);

	imageWidgets[CENTER_IMAGE].image->setPosition(imageWidgets[CENTER_IMAGE].posX, imageWidgets[CENTER_IMAGE].posY);

	imagesLayout->addChild(imageWidgets[CENTER_IMAGE].image);

	//rightImage
	imageWidgets[RIGHT_IMAGE].index = 2;
	imageWidgets[RIGHT_IMAGE].posX = mScreenWidth - mScreenWidth / 12;
	imageWidgets[RIGHT_IMAGE].posY = mScreenHeight / 5;

	imageWidgets[RIGHT_IMAGE].shadow = new RelativeLayout();
	imageWidgets[RIGHT_IMAGE].shadow->setBackgroundColor(0);
	imageWidgets[RIGHT_IMAGE].shadow->setProperty(MAW_WIDGET_ALPHA, "0.2");
	imageWidgets[RIGHT_IMAGE].shadow->setSize(imageWidth, imageHeight);
	imageWidgets[RIGHT_IMAGE].shadow->setPosition(imageWidgets[RIGHT_IMAGE].posX + SHADOW_OFFSET, imageWidgets[RIGHT_IMAGE].posY + SHADOW_OFFSET);

	imagesLayout->addChild(imageWidgets[RIGHT_IMAGE].shadow);

	imageWidgets[RIGHT_IMAGE].image = new Image();
	imageWidgets[RIGHT_IMAGE].image->setResource(images[imageWidgets[RIGHT_IMAGE].index].handle);
	imageWidgets[RIGHT_IMAGE].image->setSize(imageWidth, imageHeight);

	imageWidgets[RIGHT_IMAGE].image->setPosition(imageWidgets[RIGHT_IMAGE].posX, imageWidgets[RIGHT_IMAGE].posY);

	imagesLayout->addChild(imageWidgets[RIGHT_IMAGE].image);

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
	images = new ResImage[imagesSize];
	for (int i = 0; i < imagesSize; i++)
	{
		int resID = firstImage + i + 1;
		images[i].handle = maCreatePlaceholder();
		int ret = maCreateImageFromData(images[i].handle, resID, 0, maGetDataSize(resID));
		//readString(resID + 1, 0, images[i].text);
	}
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
