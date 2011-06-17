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
 * @author Florin Leu
 *
 * This screen allows the user to flip between images by
 * swiping left or right with the finger (or some other
 * pointing device).
 */

#ifndef SCREEN_IMAGE_SWIPER_H_
#define SCREEN_IMAGE_SWIPER_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include "UIWrapper/Widgets.h"
#include "MAHeaders.h"

#include "conprint.h"

using namespace MoSync::UI;

enum eScrollingDirection
{
	DIRECTION_NONE = 0,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

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

	/*
	 * Check if the horizontal center of the screen is inside the image.
	 * @param screenWidth The width of the screen.
	 */
	bool isOnScreenCenter(int screenWidth)
	{
		return ( (mPosX - screenWidth / 12 <= (screenWidth >> 1))
			&& ((mPosX + mWidth + screenWidth / 12) >= (screenWidth >> 1)) );
	}

	/*
	 * Gets the offset needed to align the image to grid.
	 * @param screenWidth The width of the screen.
	 */
	int getGridOffset(int screenWidth)
	{
		return ((screenWidth >> 1) - mPosX - (mWidth >> 1));
	}

	/*
	 * Updates the image position using the given offsets.
	 * @param xOffset The offset needed to align the image to the horizontal grid.
	 * @param yOffset The offset needed to align the image to the vertical grid.
	 */
	void update(int xOffset, int yOffset)
	{
		mPosX += xOffset;
		mPosY += yOffset;
		Image::setPosition(mPosX, mPosY);
	}

	/*
	 * Checks if you can scroll the image further to the given direction.
	 * @param offset The scrolling offset.
	 * @screenWidth The screen width.
	 * @direction The direction of the scrolling.
	 */
	bool canScroll(int offset, int screenWidth, int direction)
	{
		switch (direction)
		{
			case DIRECTION_LEFT:
				return (mPosX + mWidth + offset <
						(screenWidth >> 1));
			case DIRECTION_RIGHT:
				return (mPosX + offset >
						(screenWidth >> 1));
		}
	}

	/*
	 * Sets the image position inside the parent widget.
	 * @param x A valid x position.
	 * @param y A valid y position.
	 */
	void setPosition(int x, int y)
	{
		mPosX = x;
		mPosY = y;
		Image::setPosition(x, y);
	}

	/*
	 * Sets the image width.
	 * @param A valid width for the image.
	 */
	void setWidth(int w)
	{
		mWidth = w;
	}

	/*
	 * Setter for the image handle.
	 * @param The new handle for the image.
	 */
	void setHandle(int h)
	{
		mImageHandle = h;
	}

	/*
	 * Setter for the image name.
	 * @param name The new name of the image.
	 */
	void setName(MAUtil::String name)
	{
		mName = name;
	}

	/*
	 * Getter for the image handle.
	 */
	MAHandle getHandle()
	{
		return mImageHandle;
	}

	/*
	 * Getter for the image name.
	 */
	MAUtil::String getName()
	{
		return mName;
	}

private:

	/*
	 * The handle of the image.
	 */
	MAHandle mImageHandle;

	/*
	 * The image's position ont he screen.
	 */
	int mPosX;
	int mPosY;

	/*
	 * The image's width.
	 */
	int mWidth;

	/*
	 * The image's name.
	 */
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
	/*
	 * Method for getting the screen size inside the members mScreenWidth and mScreenHeight
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
	MAUtil::Vector<ScreenImage*> mImages;

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
