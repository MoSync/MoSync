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

#ifndef SCREEN_IMAGE_SWIPER_H_
#define SCREEN_IMAGE_SWIPER_H_

#include <MAUtil/String.h>
#include "UIWrapper/Widgets.h"
#include "MAHeaders.h"

#include "conprint.h"

/*
 * DEFINES
 */
#define LEFT_IMAGE				0
#define CENTER_IMAGE			1
#define RIGHT_IMAGE				2
#define DISPLAYED_IMAGES		3

#define SMALL_SCREEN_RESOLUTION			320
#define MEDIUM_SCREEN_RESOLUTION		480
#define LARGE_SCREEN_RESOLUTION			1024

#define SHADOW_OFFSET					5
/*
 * DEFINES
 */

using namespace MoSync::UI;

struct ResImage
{
	MAHandle handle;
	MAUtil::String text;
};

struct ScreenImage
{
	Image* image;
	RelativeLayout* shadow;
	int posX;
	int posY;
	int index;
};

/**
 * TODO: Add comment.
 */
class ScreenImageSwiper : public Screen
{
private:
	int mScreenWidth;
	int mScreenHeight;

	VerticalLayout* mainLayout;
	RelativeLayout* imagesLayout;
	Label* labelLayout;

	ResImage* images;
	ScreenImage* imageWidgets;
	int imagesSize;
	int currentImage;

public:

	/*
	 * Constructor
	 */
	ScreenImageSwiper();

	void createUI();

	/**
	 * TODO: Add comment.
	 */
	static Screen* create();

private:
	void getScreenSize();

	void loadImages(int screenWidth);

	//bool readString(int resID, int pos, MAUtil::String &result);
};

#endif
