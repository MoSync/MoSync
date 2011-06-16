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

	void setResource()
	{
		Image::setResource(handle);
	}
};

/**
 * A Image Swiper class.
 */
class ScreenImageSwiper : public Screen, public MAUtil::TimerListener
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

	/*
	 * Sets the properties for the images widgets.
	 * @param width The desired width of the images widgets.
	 * @param height The desired height of the images widgets.
	 */
	void setupImages(int width, int height);

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

	/*
	 * Update the application
	 */
	void runTimerEvent();

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

	// pointer X coordinates on screen
	int mPointerXStart;
	int mPointerXEnd;

	void getScreenSize();

	/*
	 * Load the images from resources according to the screen resolution.
	 * @param screenWidth The width of the screen.
	 */
	void loadImages(int screenWidth);

	/*
	 * Read the only string from one string resource.
	 * @param resID A valid resource id.
	 * @param pos The start position.
	 * @param output The resulting string.
	 */
	bool readStringFromResource(MAHandle resID, int& pos, MAUtil::String &output) const;

	/*
	 * Creates an image with a gray gradient.
	 */
	Image* createBackgroundGradient();
};

#endif
