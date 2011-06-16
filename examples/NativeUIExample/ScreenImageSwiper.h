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
#define SMALL_SCREEN_RESOLUTION			320
#define MEDIUM_SCREEN_RESOLUTION		480
#define LARGE_SCREEN_RESOLUTION			1024

//#define SHADOW_OFFSET					5
/*
 * DEFINES
 */

using namespace MoSync::UI;

struct ResImage
{
	MAHandle handle;
	MAUtil::String text;
};

class ScreenImage : public Image
{
public:
	MAHandle handle;
	//RelativeLayout* shadow;
	int posX;
	int posY;
	int width;

	void setResource()
	{
		Image::setResource(handle);
	}
};

/**
 * TODO: Add comment.
 */
class ScreenImageSwiper : public Screen
{
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

	/**
	 * Handle pointer presses.
	 */
	void handlePointerPressed(MAPoint2d p);

	/**
	 * Handle pointer moves.
	 */
	void handlePointerMoved(MAPoint2d p);

	/**
	 * Handle pointer releases.
	 */
	void handlePointerReleased(MAPoint2d p);

private:
	int mScreenWidth;
	int mScreenHeight;

	VerticalLayout* mainLayout;
	RelativeLayout* imagesLayout;
	Label* labelLayout;

	ScreenImage** images;
	int imagesSize;
	int currentImage;

	// pointer X coordinate on screen
	int mPointerX;

	void getScreenSize();

	void loadImages(int screenWidth);

	//bool readString(int resID, int pos, MAUtil::String &result);
};

#endif
