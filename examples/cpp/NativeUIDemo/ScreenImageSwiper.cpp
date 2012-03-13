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
#include "Util.h"
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
 * Google Publisher ID for ads banner.
 * Replace this constant with your publisher id.
 */
#define ADS_PUBLISHER_ID "a14dbba084368db"

/*
 * Constructor
 */
ScreenImageSwiper::ScreenImageSwiper():
	Screen(),
	mAdsBanner(NULL)
{
	createUI();

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

	// Delete the ads banner.
	delete mAdsBanner;
}

/*
 * Create the UI for the image swiper screen.
 */
void ScreenImageSwiper::createUI()
{
	// Create the main layout for this screen.
	mMainLayout = new VerticalLayout();

	// Set the screen title.
	setTitle(TXT_SCREEN_TITLE);

	// Get the screen size.
	getScreenSize();

	if (getPlatform() == ANDROID)
	{
		// Set screen icon for Android.
		setIcon(RES_TAB_ICON_IMAGE_SWIPER_ANDROID);

		// Create ads banner.
		if ( mScreenWidth > 50 )
		{
			if ( mScreenWidth > 60 )
			{
				mAdsBanner = new Ads::Banner(ADS_PUBLISHER_ID, Ads::BANNER_SIZE_LEADERBOARD);
			}
			else
			{
				mAdsBanner = new Ads::Banner(ADS_PUBLISHER_ID, Ads::BANNER_SIZE_IAB);
			}
		}
		else
		{
			mAdsBanner = new Ads::Banner(ADS_PUBLISHER_ID);
		}
	}
	else if(getPlatform() == IOS)
	{
		// Create the navigation bar for iOS.
		mTitleWidget = new NavigationBar();

		// Set the navigation's bar title.
		mTitleWidget->setTitle(TXT_SCREEN_TITLE);

		// Remove the navigation's bar back button.
		mTitleWidget->setBackButtonTitle("");

		mTitleWidget->fillSpaceHorizontally();

		// Add the navigation bar to the main layout of the screen.
		mMainLayout->addChild(mTitleWidget);

		// Set screen icon for iOS.
		setIcon(RES_TAB_ICON_IMAGE_SWIPER);

		// Create ads banner.
		mAdsBanner = new Ads::Banner(ADS_PUBLISHER_ID);
	}

	mAdsBanner->requestContent(true);

	// Add banner to layout.
	mMainLayout->addBanner(mAdsBanner);

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
	mLabelLayout->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	mLabelLayout->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);

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
		mImages[i]->setProperty("scaleMode", "scalePreserveAspect");

		mImagesLayout->addChild(mImages[i]);

		// Update x position for the next image widget.
		startX += width + mScreenWidth / 6;
	}
}

/**
 * Method that gets the screen size into the instance variables
 * mScreenWidth and mScreenHeight.
 */
void ScreenImageSwiper::getScreenSize()
{
	if (getPlatform() == IOS)
	{
		/// Extract the screen width
		mScreenWidth = getWidth();

		/// Extract the screen height
		mScreenHeight = getHeight();
	}
	else
	{

		/// Get screen dimensions.
		MAExtent size = maGetScrSize();

		/// Extract the screen width
		mScreenWidth = EXTENT_X(size);

		/// Extract the screen height
		mScreenHeight = EXTENT_Y(size);
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
		// Byte index in the resource data.
		int pos = 0;

		// Create an image object.
		mImages[i] = new ScreenImage();

		// Get the resource id of the current image.
		int resID = firstImage + i + 1;

		// Create a placeholder to store the current image.
		mImages[i]->setHandle(maCreatePlaceholder());

		// Load the image name from resources.
		MAUtil::String name;
		// This reads the string into name and updates
		// the value of pos to the start of the image data.
		readStringFromResource(resID, pos, name);
		// Set the image name.
		mImages[i]->setName(name);

		// Create the image from ubin. Note that pos is
		// updated by the call to readStringFromResource
		// to point at the start of the image data.
		maCreateImageFromData(
			mImages[i]->getHandle(),
			resID,
			pos,
			maGetDataSize(resID) - mImages[i]->getName().length() - 1);
	}
}

float ScreenImageSwiper::getScaleFactor() {
	 return (float)mScreenWidth / (float)EXTENT_X(maGetScrSize());
}

/**
 * Handle pointer presses.
 * @param point The pointer position on the screen.
 */
void ScreenImageSwiper::handlePointerPressed(MAPoint2d point)
{
	// Set start and end swipe point to the touch down point.

	float scaleFactor = getScaleFactor();
	mPointerXStart = (int)((float) point.x * scaleFactor);
	mPointerXEnd = (int)((float) point.x * scaleFactor);
}

/**
 * Handle pointer moves.
 * @param point The pointer position on the screen.
 */
void ScreenImageSwiper::handlePointerMoved(MAPoint2d point)
{
	float scaleFactor = getScaleFactor();
	// Update the end swipe position.
	mPointerXEnd = (int)((float) point.x * scaleFactor);
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

	float scaleFactor = getScaleFactor();
	mPointerXStart = (int)((float) point.x * scaleFactor);
	mPointerXEnd = (int)((float) point.x * scaleFactor);
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
 * Note: The parameter pos is used both for input and output
 * of the position in the resource data.
 * @param resID A valid resource id.
 * @param pos In: The start position. Out: The position
 * after the string in the resource.
 * @param output The resulting string.
 */
bool ScreenImageSwiper::readStringFromResource(
	MAHandle resID,
	int& pos,
	MAUtil::String& output) const
{
	// Get all the characters on one read.

	// Get the length of the string stored as a .pstring
	// (Pascal string). The first byte contains the length.
	byte stringLen = 0;
	maReadData(resID, (void*) &stringLen, pos, sizeof(byte));
	if (stringLen > maGetDataSize(resID) || stringLen <= 0)
	{
		return false;
	}

	// Read the string.
	pos += sizeof(byte);
	output.resize(stringLen);
	maReadData(resID, (void*) output.c_str(), pos, stringLen);

	// Update position to the byte after the string.
	pos += stringLen;

	return true;
}

/**
 * Creates an image with a gray gradient.
 */
Image* ScreenImageSwiper::createBackgroundGradient()
{
	// Create gradient bitmap.
	int *gradientBitmap = new int[16 * 16];
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j<16; ++j)
		{
			int color = 0xFF - (i * 0xFF / 16);
			gradientBitmap[i*16 + j] = color | (color << 8) | (color << 16);
		}
	}

	// Create image with gradient.
	MAHandle imageHandle = maCreatePlaceholder();
	maCreateImageRaw(imageHandle, gradientBitmap, EXTENT(16, 16), 0);
	delete gradientBitmap;

	// Create image widget.
	Image* imageWidget = new Image();
	imageWidget->setImage(imageHandle);
	imageWidget->setPosition(0, 0);
	imageWidget->setSize(
		MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	// Tell image to stretch.
	imageWidget->setProperty("scaleMode", "scaleXY");

	return imageWidget;
}
