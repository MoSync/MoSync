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
 * @author Bogdan Iusco
 *
 * This screen allows the user to flip between images by
 * swiping left or right with the finger (or some other
 * pointing device).
 */

#ifndef SCREEN_IMAGE_SWIPER_H_
#define SCREEN_IMAGE_SWIPER_H_

#include <MAUtil/String.h>
#include "UIWrapper/Widgets.h"
#include "MAHeaders.h"

#include "conprint.h"

using namespace MoSync::UI;

/**
 * An extension to the Image widget which
 * uses ubin (unloaded binary) resources.
 */
class ScreenImage : public Image
{
public:
	/**
	 * Set the image shown in the widget.
	 */
	void showImage()
	{
		Image::setImage(mImageHandle);
	}

	// TODO: Document, make private.
	MAHandle mImageHandle;
	int mPosX;
	int mPosY;
	int mWidth;
	MAUtil::String mName;
};

/**
 * An Image Swiper class.
 */
class ScreenImageSwiper :
	public Screen,
	public MAUtil::TimerListener
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
	 * @return An instance of class ScreenImageSwiper.
	 */
	static Screen* create();

	/**
	 * Handle pointer presses.
	 * @param point The pointer position on the screen.
	 */
	void handlePointerPressed(MAPoint2d point);

	/**
	 * Handle pointer moves.
	 * @param point The pointer position on the screen.
	 */
	void handlePointerMoved(MAPoint2d point);

	/**
	 * Handle pointer releases.
	 * @param point The pointer position on the screen.
	 */
	void handlePointerReleased(MAPoint2d point);

	/*
	 * Update the application
	 */
	void runTimerEvent();

private:
	/**
	 * TODO: Document.
	 */
	void getScreenSize();

	/**
	 * Load the images from resources according to the screen resolution.
	 * @param screenWidth The width of the screen.
	 */
	void loadImages(int screenWidth);

	/**
	 * Read the only string from one string resource.
	 * @param resID A valid resource id.
	 * @param pos The start position.
	 * @param output The resulting string.
	 */
	bool readStringFromResource(
		MAHandle resID,
		int& pos,
		MAUtil::String& output) const;

	/**
	 * Creates an image with a gray gradient.
	 */
	Image* createBackgroundGradient();

	/**
	 * Screen width.
	 */
	int mScreenWidth;

	/**
	 * Screen height.
	 */
	int mScreenHeight;

	/**
	 * The main layout of the image swiper screen.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * Layout used to hold images.
	 */
	RelativeLayout* mImagesLayout;

	/**
	 * Layout for the label that displayes the image name.
	 */
	Label* mLabelLayout;

	/**
	 * A simple navigation bar for iOS devices.
	 * It is used only for displaying a text (the title
	 * of the screen).
	 */
	NavigationBar* mTitleWidget;

	/**
	 * Pointer to array of ScreenImage pointers.
	 */
	ScreenImage** mImages;

	/**
	 * Number of images.
	 */
	int mImagesSize;

	/**
	 * X coordinate on screen of the pointer pressed point.
	 */
	int mPointerXStart;

	/**
	 * X coordinate on screen of the pointer released point.
	 */
	int mPointerXEnd;
};

#endif
