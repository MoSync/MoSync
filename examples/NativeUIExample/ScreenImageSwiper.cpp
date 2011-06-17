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
 * @file ScreenImageSwiper.cpp
 * @author Florin Leu
 *
 * This screen allows the user to flip between images by
 * swiping left or right with the finger (or some other
 * pointing device).
 */

#include "MAHeaders.h"
#include "ScreenImageSwiper.h"

/**
 * Return the absolute value of a number.
 */
#define ABS(a)	   						(((a) < 0) ? -(a) : (a))

#define SMALL_SCREEN_RESOLUTION			320
#define MEDIUM_SCREEN_RESOLUTION		480
#define LARGE_SCREEN_RESOLUTION			1024

#define TXT_SCREEN_TITLE				"Images"

#define SCREEN_BG_COLOR					0xF0F0F0
#define LABEL_BG_COLOR					0x111111
#define FONT_COLOR						0xFFFFFF

#define FRAME_DELAY						50

#define DEFAULT_IMAGE_INDEX				0

/**
 * Create the Swiper Screen.
 * @return An instance of class ScreenImageSwiper.
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
	// Add a timer to keep the update at a 20 FPS rate
	MAUtil::Environment::getEnvironment().addTimer(this, FRAME_DELAY, 0);
}

/*
 * Destructor
 */
ScreenImageSwiper::~ScreenImageSwiper()
{
	// Remove the timer
	MAUtil::Environment::getEnvironment().removeTimer(this);
}

/*
 * Create the UI for the image swiper screen.
 */
void ScreenImageSwiper::createUI()
{
	// Create the main layout for this screen.
	mMainLayout = new VerticalLayout();

	// Set the screne title.
	setTitle(TXT_SCREEN_TITLE);

	if (WidgetManager::isAndroid())
	{
		// Set screen icon for Android.
		setIcon(RES_TAB_ICON_IMAGE_SWIPER_ANDROID);
	}
	else
	{
		// Create the navigation bar for iOS
		mTitleWidget = new NavigationBar();

		// Set the navigation's bar sizes.
		//mTitleWidget->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, TITLE_WIDGET_HEIGHT);

		// Set the navigation's bar title.
		mTitleWidget->setTitle(TXT_SCREEN_TITLE);

		// Remove the navigation's bar back button.
		mTitleWidget->setBackButtonTitle("");

		// Add the navigation bar to the main layout of the screen.
		mMainLayout->addChild(mTitleWidget);

		// Set screen icon for iOS.
		setIcon(RES_TAB_ICON_IMAGE_SWIPER);
	}

	getScreenSize();

	// Create a RelativeLayout as container for images.
	mImagesLayout = new RelativeLayout();

	// Set the layout's background color.
	mImagesLayout->setBackgroundColor(SCREEN_BG_COLOR);

	// Create a background gradient and add it to the images Layout
	mImagesLayout->addChild(createBackgroundGradient());

	// Loads the needed image according to the screen resolution.
	loadImages(mScreenWidth);

	// Get the image size.
	MAExtent size = maGetImageSize(mImages[DEFAULT_IMAGE_INDEX]->getHandle());

	// Compute the needed width and height for images.
	int imageWidth = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;
	int imageHeight = imageWidth * EXTENT_Y(size) / EXTENT_X(size);

	setupImages(imageWidth, imageHeight);

	//Creates a label layout to display image names and sets it's properties
	mLabelLayout = new Label();
	mLabelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	mLabelLayout->setBackgroundColor(LABEL_BG_COLOR);
	mLabelLayout->setText(mImages[DEFAULT_IMAGE_INDEX]->getName());
	mLabelLayout->setFontColor(FONT_COLOR);
	mLabelLayout->centerTextHorizontally();
	mLabelLayout->centerTextVertically();

	// Adds the created layouts as childrens of the main one.
	mMainLayout->addChild(mImagesLayout);
	mMainLayout->addChild(mLabelLayout);

	// Sets the main layout as the main widget for the current screen.
	setMainWidget(mMainLayout);
}

/*
 * Sets the properties for the images widgets.
 * @param width The desired width of the images widgets.
 * @param height The desired height of the images widgets.
 */
void ScreenImageSwiper::setupImages(int width, int height)
{
	// The starting point for the first image.
	int startX = (mScreenWidth - width) >> 1;
	for (int i=0; i<mImagesSize; i++)
	{
		// Set image widget properties.
		mImages[i]->setPosition(startX, mScreenHeight / 5);
		mImages[i]->setWidth(width);

		mImages[i]->showImage();
		mImages[i]->setSize(width, height);

		mImagesLayout->addChild(mImages[i]);

		// Update x position for the next image widget.
		startX += width + mScreenWidth / 6;
	}
}

/*
 * Method for getting the screen size inside the members mScreenWidth and mScreenHeight
 */
void ScreenImageSwiper::getScreenSize()
{
	if (WidgetManager::isAndroid())
	{
		/// Get screen dimensions.
		MAExtent size = maGetScrSize();

		/// Extract the screen width
		mScreenWidth = EXTENT_X(size);

		/// Extract the screen height
		mScreenHeight = EXTENT_Y(size);
	}
	else
	{
		/// Extract the screen width
		mScreenWidth = getPropertyInt(MAW_WIDGET_WIDTH);

		/// Extract the screen height
		mScreenHeight = getPropertyInt(MAW_WIDGET_HEIGHT);
	}
}

/*
 * Load the images from resources according to the screen resolution.
 * @param screenWidth The width of the screen.
 */
void ScreenImageSwiper::loadImages(int screenWidth)
{
	// Set the image indexes inside the resources according to the resolution.
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

	// Compute the number of images.
	mImagesSize = lastImage - firstImage - 1;

	// Create the image widgets.
	for (int i = 0; i < mImagesSize; i++)
	{
		int pos = 0;
		mImages[i] = new ScreenImage();
		// Get the resource id of the current image.
		int resID = firstImage + i + 1;

		// Create a placeholder to store the current image.
		mImages[i]->setHandle(maCreatePlaceholder());

		// Load the image name from resources.
		MAUtil::String name;
		readStringFromResource(resID, pos, name);
		mImages[i]->setName(name);
		// Create the image from ubin.
		maCreateImageFromData(
			mImages[i]->getHandle(),
			resID,
			pos,
			maGetDataSize(resID) - mImages[i]->getName().length() - 1);
	}
}

/**
 * Handle pointer presses.
 * @param point The pointer position on the screen.
 */
void ScreenImageSwiper::handlePointerPressed(MAPoint2d point)
{
	// Set start and end swipe point to the touch down point.
	mPointerXStart = point.x;
	mPointerXEnd = point.x;
}

/**
 * Handle pointer moves.
 * @param point The pointer position on the screen.
 */
void ScreenImageSwiper::handlePointerMoved(MAPoint2d point)
{
	// Update the end swipe position.
	mPointerXEnd = point.x;
}

/**
 * Handle pointer releases.
 * @param point The pointer position on the screen.
 */
void ScreenImageSwiper::handlePointerReleased(MAPoint2d point)
{
	// Search for the image on the center of the screen.
	int centeredImage = -1;
	for (int i = 0; i < mImagesSize; ++i)
	{
		if ( mImages[i]->isOnScreenCenter(mScreenWidth) )
		{
			centeredImage = i;
			break;
		}
	}

	// Align images to grid.
	if (centeredImage >= 0)
	{
		mLabelLayout->setText(mImages[centeredImage]->getName());
		int offset = mImages[centeredImage]->getGridOffset(mScreenWidth);
		for (int i = 0; i < mImagesSize; ++i)
		{
			mImages[i]->update(offset, 0);
		}
	}

	mPointerXStart = point.x;
	mPointerXEnd = point.x;
}

/**
 * Update the application.
 */
void ScreenImageSwiper::runTimerEvent()
{
	// Get the swiping length.
	int offset = mPointerXEnd - mPointerXStart;

	if (offset < 0) // We have a swiping from right to left.
	{
		// Stop scrolling if the last image will pass the middle screen.
		if (mImages[mImagesSize - 1]->canScroll(offset, mScreenWidth, DIRECTION_LEFT))
		{
			return;
		}
	}
	// Else we have a swiping from left to right.
	else
	{
		// Stop scrolling if the first image will pass the middle screen.
		if (mImages[0]->canScroll(offset, mScreenWidth, DIRECTION_RIGHT))
		{
			return;
		}
	}

	// Update the images according to the swiping length.
	for (int i=0; i<mImagesSize; ++i)
	{
		mImages[i]->update(offset, 0);
	}

	mPointerXStart = mPointerXEnd;
}

/*
 * Read the only string from one string resource.
 * @param resID A valid resource id.
 * @param pos The start position.
 * @param output The resulting string.
 */
bool ScreenImageSwiper::readStringFromResource(
	MAHandle resID,
	int& pos,
	MAUtil::String& output) const
{
	// Get all the characters on one read.
	byte stringLen = 0;
	maReadData(resID, (void*) &stringLen, pos, sizeof(byte));

	if (stringLen > maGetDataSize(resID) || stringLen <= 0)
	{
		return false;
	}

	pos += sizeof(byte);
	output.resize(stringLen);

	maReadData(resID, (void*) output.c_str(), pos, stringLen);
	pos += stringLen;
	return true;
}

/**
 * Creates an image with a gray gradient.
 */
Image* ScreenImageSwiper::createBackgroundGradient()
{
	MAHandle h = maCreatePlaceholder();
	int *src = new int[16 * 16];
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j<16; ++j)
		{
			int color = 0xFF - (i * 0xFF / 16);
			src[i*16 + j] = color | (color << 8) | (color << 16);
		}
	}

	maCreateImageRaw(h, src, EXTENT(16, 16), 0);
	Image* img = new Image();
	img->setImage(h);
	img->setPosition(0, 0);
	img->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	int size = maGetImageSize(h);
	delete src;
	return img;
}
