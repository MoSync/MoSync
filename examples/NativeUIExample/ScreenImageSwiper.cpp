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

//ScreenImageSwiper *ScreenImageSwiper::sInstance = NULL;

int ScreenImageSwiper::mScreenWidth = NULL;
int ScreenImageSwiper::mScreenHeight = NULL;

/**
 * TODO: Add comment.
 */
Screen* ScreenImageSwiper::create()
{
	Screen* screen = new Screen();
	screen->setTitle("Images");
	if (WidgetManager::isAndroid())
	{
		screen->setIcon(RES_TAB_ICON_IMAGE_SWIPER_ANDROID);
	}
	else
	{
		screen->setIcon(RES_TAB_ICON_IMAGE_SWIPER);
	}

	getScreenSize();

	VerticalLayout* mainLayout = new VerticalLayout();
	HorizontalLayout* imagesLayout = new HorizontalLayout();
	imagesLayout->setBackgroundColor(0xFF0000);

	Label* labelLayout = new Label();
	labelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	labelLayout->setBackgroundColor(0x111111);
	labelLayout->setText("CACA");
	labelLayout->setFontColor(0xFFFFFF);
	labelLayout->centerTextHorizontally();
	labelLayout->centerTextVertically();

	RelativeLayout* leftImageLayout = new RelativeLayout();
	leftImageLayout->setSize(mScreenWidth / 6, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	//leftImageLayout->setBackgroundColor(0xFF0000);

	RelativeLayout* centerImageLayout = new RelativeLayout();
	centerImageLayout->setSize(2 * mScreenWidth / 3, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	centerImageLayout->setProperty("childVerticalAlignment", MAW_ALIGNMENT_CENTER);
	//centerImageLayout->setBackgroundColor(0x00FF00);

	RelativeLayout* rightImageLayout = new RelativeLayout();
	rightImageLayout->setSize(mScreenWidth / 6, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	//rightImageLayout->setBackgroundColor(0xFF0000);

	int imageSize = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;

	Image** images = new Image*[DISPLAYED_IMAGES];
	for (int i=0; i<DISPLAYED_IMAGES; i++)
	{
		images[i] = new Image();
		images[i]->setResource(RES_IMAGE1 + i);
		images[i]->setSize(imageSize, MAW_CONSTANT_WRAP_CONTENT);
	}

	images[LEFT_IMAGE]->setPosition(mScreenWidth / 6 - imageSize - mScreenWidth / 12, mScreenWidth / 10);
	leftImageLayout->addChild(images[LEFT_IMAGE]);

	images[CENTER_IMAGE]->setPosition(mScreenWidth / 12, mScreenWidth / 10);
	centerImageLayout->addChild(images[CENTER_IMAGE]);

	images[RIGHT_IMAGE]->setPosition(mScreenWidth / 12, mScreenWidth / 10);
	rightImageLayout->addChild(images[RIGHT_IMAGE]);

	imagesLayout->addChild(leftImageLayout);
	imagesLayout->addChild(centerImageLayout);
	imagesLayout->addChild(rightImageLayout);

	mainLayout->addChild(imagesLayout);
	mainLayout->addChild(labelLayout);

	screen->setMainWidget(mainLayout);

	return screen;
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
