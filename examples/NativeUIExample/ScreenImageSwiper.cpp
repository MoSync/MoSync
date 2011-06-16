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
}

/*
 * Destructor
 */
ScreenImageSwiper::~ScreenImageSwiper()
{
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
		mTitleWidget->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, TITLE_WIDGET_HEIGHT);

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
	imagesLayout->addChild(createBackgroundGradient());

	loadImages(mScreenWidth);

	MAExtent size = maGetImageSize(images[0]->handle);

	int imageWidth = mScreenWidth - (mScreenWidth / 3) - mScreenWidth / 6;
	int imageHeight = imageWidth * EXTENT_Y(size) / EXTENT_X(size);

	int startX = mScreenWidth / 6 - imageWidth - mScreenWidth / 12;
	for (int i=0; i<imagesSize; i++)
	{
		images[i]->posX = startX;
		images[i]->posY = mScreenHeight / 5;
		images[i]->width = imageWidth;

		images[i]->setResource();
		images[i]->setSize(imageWidth, imageHeight);
		images[i]->shadow = new RelativeLayout();
		images[i]->shadow->setSize(imageWidth, imageHeight);
		images[i]->shadow->setBackgroundColor(0);
		images[i]->shadow->setProperty(MAW_WIDGET_ALPHA, SHADOW_ALPHA);
		images[i]->shadow->setPosition(images[i]->posX + 5, images[i]->posY + 5);
		images[i]->setPosition(images[i]->posX, images[i]->posY);

		imagesLayout->addChild(images[i]->shadow);
		imagesLayout->addChild(images[i]);
		startX += imageWidth + mScreenWidth / 6;
	}

	labelLayout = new Label();
	labelLayout->setSize(MAW_CONSTANT_FILL_AVAILABLE_SPACE, mScreenHeight / 10);
	labelLayout->setBackgroundColor(LABEL_BG_COLOR);
	labelLayout->setText(images[1]->name);
	labelLayout->setFontColor(FONT_COLOR);
	labelLayout->centerTextHorizontally();
	labelLayout->centerTextVertically();

	mainLayout->addChild(imagesLayout);
	mainLayout->addChild(labelLayout);

	setMainWidget(mainLayout);
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
 * Load the needed images from ubins.
 */
void ScreenImageSwiper::loadImages(int screenWidth)
{
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
	imagesSize = lastImage - firstImage - 1;
	images = new ScreenImage*[imagesSize];
	for (int i = 0; i < imagesSize; i++)
	{
		int pos = 0;
		images[i] = new ScreenImage();
		int resID = firstImage + i + 1;
		images[i]->handle = maCreatePlaceholder();

		readStringFromResource(resID, pos, images[i]->name);

		maCreateImageFromData(images[i]->handle, resID, pos, maGetDataSize(resID) - images[i]->name.length() - 1);
	}
}

/**
 * Handle pointer presses.
 */
void ScreenImageSwiper::handlePointerPressed(MAPoint2d p)
{
	mPointerX = p.x;
}

/**
 * Handle pointer moves.
 */
void ScreenImageSwiper::handlePointerMoved(MAPoint2d p)
{
	int offset = p.x - mPointerX;

	if (ABS(offset) < mScreenWidth / 20)
		return;

	if (offset < 0)
	{
		if (images[imagesSize - 1]->posX + images[imagesSize - 1]->width <= (mScreenWidth >> 1))
		{
			return;
		}
	}
	else
	{
		if (images[0]->posX >= (mScreenWidth >> 1))
		{
			return;
		}
	}

	for (int i=0; i<imagesSize; i++)
	{
		images[i]->posX += offset;

		images[i]->shadow->setPosition(images[i]->posX + 5, images[i]->posY + 5);
		images[i]->setPosition(images[i]->posX, images[i]->posY);
	}
	mPointerX = p.x;
}

/**
 * Handle pointer releases.
 */
void ScreenImageSwiper::handlePointerReleased(MAPoint2d p)
{
	int centeredImage = -1;
	for (int i=0; i<imagesSize; i++)
	{
		if ( (images[i]->posX - mScreenWidth / 12 < (mScreenWidth >> 1)) && ((images[i]->posX + images[i]->width + mScreenWidth / 12) > (mScreenWidth >> 1)) )
		{
			centeredImage = i;
			break;
		}
	}
	labelLayout->setText(images[centeredImage]->name);
	if (centeredImage >= 0)
	{
		int offset = (mScreenWidth >> 1) - images[centeredImage]->posX - (images[centeredImage]->width >> 1);
		for (int i=0; i<imagesSize; i++)
		{
			images[i]->posX += offset;
			images[i]->shadow->setPosition(images[i]->posX + 5, images[i]->posY + 5);
			images[i]->setPosition(images[i]->posX, images[i]->posY);
		}
	}
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
	//	GUI("stringLen = %d", stringLen);

	if (stringLen > maGetDataSize(resID) || stringLen <= 0)
		return false;

	pos += sizeof(byte);
	output.resize(stringLen);
	//	GUI("read %d bytes from %d", stringLen, pos);
	maReadData(resID, (void*) output.c_str(), pos, stringLen);
	pos += stringLen;
	return true;
}

Image* ScreenImageSwiper::createBackgroundGradient()
{
	MAHandle h = maCreatePlaceholder();
	int *src = new int[mScreenWidth * mScreenHeight];
	for (int i=0; i<mScreenWidth; i++)
	{
		for (int j=0; j<mScreenHeight; j++)
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
