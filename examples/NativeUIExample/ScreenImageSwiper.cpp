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
 * Create the Swiper Screen.
 */
Screen* ScreenImageSwiper::create()
{
	ScreenImageSwiper* screen = new ScreenImageSwiper();
	screen->createUI();
	return screen;
}

/*
 * Constructor
 */
ScreenImageSwiper::ScreenImageSwiper()
{
}

/*
 * Destructor
 */
ScreenImageSwiper::~ScreenImageSwiper()
{
	delete[] images;
	images = NULL;
}

void ScreenImageSwiper::createUI()
{
	setTitle(TXT_SCREEN_TITLE);
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
	imagesLayout->setBackgroundColor(SCREEN_BG_COLOR);

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

		images[i]->setResource();
		images[i]->setSize(imageWidth, imageHeight);
		images[i]->setPosition(images[i]->posX, images[i]->posY);

		imagesLayout->addChild(images[i]);
		startX += imageWidth + mScreenWidth / 6;
	}

	labelLayout = new Label();
	labelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	labelLayout->setBackgroundColor(LABEL_BG_COLOR);
	labelLayout->setText("Image X");
	labelLayout->setFontColor(FONT_COLOR);
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

/*
 * Load the needed images from ubins.
 */
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
		if (images[imagesSize - 1]->posX + images[imagesSize - 1]->width <= (mScreenWidth >> 1))
		{
			return;
		}
	}
	else
	{
		if (images[0]->posX >= (mScreenWidth >> 1))
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
	int centerImage = -1;
	for (int i=0; i<imagesSize; i++)
	{
		if ( (images[i]->posX - mScreenWidth / 12 < (mScreenWidth >> 1)) && ((images[i]->posX + images[i]->width + mScreenWidth / 12) > (mScreenWidth >> 1)) )
		{
			centerImage = i;
			break;
		}
	}

	if (centerImage > 0)
	{
		int offset = (mScreenWidth >> 1) - images[centerImage]->posX - (images[centerImage]->width >> 1);
		for (int i=0; i<imagesSize; i++)
		{
			images[i]->posX += offset;

			images[i]->setPosition(images[i]->posX, images[i]->posY);
		}
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
