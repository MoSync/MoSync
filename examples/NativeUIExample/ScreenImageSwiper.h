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
#define ABS(a)	   						(((a) < 0) ? -(a) : (a))

#define SMALL_SCREEN_RESOLUTION			320
#define MEDIUM_SCREEN_RESOLUTION		480
#define LARGE_SCREEN_RESOLUTION			1024

#define TXT_SCREEN_TITLE				"Images"

#define SCREEN_BG_COLOR					0xF0F0F0
#define LABEL_BG_COLOR					0x111111
#define FONT_COLOR						0xFFFFFF

#define SHADOW_ALPHA					"0.2"

#define TITLE_WIDGET_HEIGHT				30
/*
 * DEFINES
 */

using namespace MoSync::UI;

/*
 * An extension to the Image widget wich uses ubin
 */
class ScreenImage : public Image
{
public:
	MAHandle handle;
	int posX;
	int posY;
	int width;

	MAUtil::String name;

	RelativeLayout* shadow;

	void setResource()
	{
		Image::setResource(handle);
	}
};

/**
 * A Image Swiper class.
 */
class ScreenImageSwiper : public Screen
{
public:

	/*
	 * Constructor
	 */
	ScreenImageSwiper();

	/*
	 * Destructor
	 */
	~ScreenImageSwiper();

	/*
	 * Create the UI for the image swiper screen.
	 */
	void createUI();

	/**
	 * Create the Swiper Screen.
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
	// Screen sizes.
	int mScreenWidth;
	int mScreenHeight;

	VerticalLayout* mainLayout;
	RelativeLayout* imagesLayout;
	Label* labelLayout;

	/**
	 * A simple navigation bar for iOS devices.
	 * It is used only for displaying a text(the title of the screen).
	 */
	NavigationBar* mTitleWidget;

	ScreenImage** images;
	int imagesSize;

	// pointer X coordinate on screen
	int mPointerX;

	void getScreenSize();

	void loadImages(int screenWidth);

	/*
	 * Read the only string from one string resource.
	 * @param resID A valid resource id.
	 * @param pos The start position.
	 * @param output The resulting string.
	 */
	bool readStringFromResource(MAHandle resID, int& pos, MAUtil::String &output) const;

	Image* createBackgroundGradient();
};

#endif
