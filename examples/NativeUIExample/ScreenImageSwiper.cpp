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

#define FRAME_DELAY						50

#define DEFAULT_IMAGE_INDEX				0
/*
 * DEFINES
 */

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

	delete images;
	images = NULL;
}

/*
 * Create the UI for the image swiper screen.
 */
void ScreenImageSwiper::createUI()
{
	// Create the main layout for this screen.
	mainLayout = new VerticalLayout();

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
		mainLayout->addChild(mTitleWidget);

		// Set screen icon for iOS.
		setIcon(RES_TAB_ICON_IMAGE_SWIPER);
	}

	getScreenSize();

	// Create a RelativeLayout as container for images.
	imagesLayout = new RelativeLayout();

	// Set the layout's background color.
	imagesLayout->setBackgroundColor(SCREEN_BG_COLOR);

	// Create a background gradient and add it to the images Layout
	//imagesLayout->addChild(createBackgroundGradient());

	// Loads the needed image according to the screen resolution.
	loadImages(mScreenWidth);

	// Get the image size.
	MAExtent size = maGetImageSize(images[0]->handle);

	// Compute the needed width and height for images.
	int imageWidth = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;
	int imageHeight = imageWidth * EXTENT_Y(size) / EXTENT_X(size);

	setupImages(imageWidth, imageHeight);

	//Creates a label layout to display image names and sets it's properties
	labelLayout = new Label();
	labelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	labelLayout->setBackgroundColor(LABEL_BG_COLOR);
	labelLayout->setText(images[DEFAULT_IMAGE_INDEX]->name);
	labelLayout->setFontColor(FONT_COLOR);
	labelLayout->centerTextHorizontally();
	labelLayout->centerTextVertically();

	// Adds the created layouts as childrens of the main one.
	mainLayout->addChild(imagesLayout);
	mainLayout->addChild(labelLayout);

	// Sets the main layout as the main widget for the current screen.
	setMainWidget(mainLayout);
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
	for (int i=0; i<imagesSize; i++)
	{
		// Set image widget properties.
		images[i]->posX = startX;
		images[i]->posY = mScreenHeight / 5;
		images[i]->width = width;

		images[i]->setResource();
		images[i]->setSize(width, height);
		images[i]->setPosition(images[i]->posX, images[i]->posY);

		imagesLayout->addChild(images[i]);

		// Update x position for the next image widget.
		startX += width + mScreenWidth / 6;
	}
}

/*
 * Method for getting the screen size inside the static members sScreenWidth and sScreenHeight
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
	// Set the image indexes inside the resources according to the resolution
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

	// Compute the numer of images
	imagesSize = lastImage - firstImage - 1;

	// Create the images widgets
	images = new ScreenImage*[imagesSize];
	for (int i = 0; i < imagesSize; i++)
	{
		int pos = 0;
		images[i] = new ScreenImage();
		// Get the resource id of the current image.
		int resID = firstImage + i + 1;

		// Create a placeholder to store the current image.
		images[i]->handle = maCreatePlaceholder();

		// Load the image name from resources
		readStringFromResource(resID, pos, images[i]->name);

		// Create the image from ubin
		maCreateImageFromData(images[i]->handle, resID, pos, maGetDataSize(resID) - images[i]->name.length() - 1);
	}
}

/**
 * Handle pointer presses.
 * @param p A valid point on the screen.
 */
void ScreenImageSwiper::handlePointerPressed(MAPoint2d p)
{
	mPointerXStart = mPointerXEnd = p.x;
}

/**
 * Handle pointer moves.
 * @param p A valid point on the screen.
 */
void ScreenImageSwiper::handlePointerMoved(MAPoint2d p)
{
	mPointerXEnd = p.x;
}

/**
 * Handle pointer releases.
 * @param p A valid point on the screen.
 */
void ScreenImageSwiper::handlePointerReleased(MAPoint2d p)
{
	// Search for the image on the center of the screen
	int centeredImage = -1;
	for (int i=0; i<imagesSize; i++)
	{
		if ( (images[i]->posX - mScreenWidth / 12 <= (mScreenWidth >> 1)) && ((images[i]->posX + images[i]->width + mScreenWidth / 12) >= (mScreenWidth >> 1)) )
		{
			centeredImage = i;
			break;
		}
	}

	// Align images to grid
	if (centeredImage >= 0)
	{
		labelLayout->setText(images[centeredImage]->name);
		int offset = (mScreenWidth >> 1) - images[centeredImage]->posX - (images[centeredImage]->width >> 1);
		for (int i=0; i<imagesSize; i++)
		{
			images[i]->posX += offset;
			images[i]->setPosition(images[i]->posX, images[i]->posY);
		}
	}
	mPointerXStart = mPointerXEnd = p.x;
}

/*
 * Update the application
 */
void ScreenImageSwiper::runTimerEvent()
{
	// Get the swiping length.
	int offset = mPointerXEnd - mPointerXStart;

	if (offset < 0) // We have a swiping from right to left.
	{
		if (images[imagesSize - 1]->posX + images[imagesSize - 1]->width + offset < (mScreenWidth >> 1)) // Stop scrolling if the last image will pass the middle screen.
		{
			return;
		}
	}
	else  // We have a swiping from left to right.
	{
		if (images[0]->posX + offset > (mScreenWidth >> 1))  // Stop scrolling if the first image will pass the middle screen.
		{
			return;
		}
	}

	// Update the images according to the swiping length.
	for (int i=0; i<imagesSize; i++)
	{
		images[i]->posX += offset;

		images[i]->setPosition(images[i]->posX, images[i]->posY);
	}
	mPointerXStart = mPointerXEnd;
}

/*
 * Read the only string from one string resource.
 * @param resID A valid resource id.
 * @param pos The start position.
 * @param output The resulting string.
 */
bool ScreenImageSwiper::readStringFromResource(MAHandle resID, int& pos, MAUtil::String &output) const
{
	// Get all the characters on one read.
	byte stringLen = 0;
	maReadData(resID, (void*) &stringLen, pos, sizeof(byte));

	if (stringLen > maGetDataSize(resID) || stringLen <= 0)
		return false;

	pos += sizeof(byte);
	output.resize(stringLen);

	maReadData(resID, (void*) output.c_str(), pos, stringLen);
	pos += stringLen;
	return true;
}

/*
 * Creates an image with a gray gradient.
 */
Image* ScreenImageSwiper::createBackgroundGradient()
{
	MAHandle h = maCreatePlaceholder();
	int *src = new int[10 * 256];
	for (int i=0; i<256; i++)
	{
		for (int j=0; j<10; j++)
		{
			int color = 0xFF - (i * 0xFF / mScreenWidth);
			src[i*mScreenWidth + j] = color | (color << 8) | (color << 16);
		}
	}

	maCreateImageRaw(h, src, EXTENT(mScreenWidth, mScreenHeight), 0);
	Image* img = new Image();
	img->setResource(h);
	img->setPosition(0, 0);
	img->setSize(mScreenWidth, mScreenHeight);
	int size = maGetImageSize(h);
	delete src;
	return img;
}
