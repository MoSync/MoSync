#include "ImageScreen.h"
#include <NativeUI/ImageButton.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Image.h>
#include "MAHeaders.h"
#include <mastdlib.h>

#define SMALL_SCREEN_RESOLUTION			320
#define MEDIUM_SCREEN_RESOLUTION		480
#define LARGE_SCREEN_RESOLUTION			1024

#define TXT_SCREEN_TITLE				"Images"

#define SCREEN_BG_COLOR					0xF0F0F0
#define LABEL_BG_COLOR					0x111111
#define FONT_COLOR						0xFFFFFF

#define FRAME_DELAY						50

#define DEFAULT_IMAGE_INDEX				0

ImageScreenWithButtons::ImageScreenWithButtons() :
		Screen() {
	CreateUI();
}

ImageScreenWithButtons::~ImageScreenWithButtons() {
	mNextImage->removeButtonListener(this);
	mPreviousImage->removeButtonListener(this);

	delete mNextImage;
	delete mPreviousImage;

	delete imageLayout;
	delete mButtonBar;
	delete mMainLayout;

	for (int i = 0; i < mImages.size(); i++)
		delete mImages[i];
}

void ImageScreenWithButtons::CreateUI() {
	this->setTitle("Images");
	/// Get screen dimensions.
	MAExtent size = maGetScrSize();

	/// Extract the screen width
	mScreenWidth = EXTENT_X(size);

	/// Extract the screen height
	mScreenHeight = EXTENT_Y(size);

	mNextImage = new NativeUI::Button();
	mNextImage->setText("-->");
	mNextImage->wrapContentHorizontally();
	mNextImage->addButtonListener(this);

	mPreviousImage = new NativeUI::Button();
	mPreviousImage->setText("<--");
	mNextImage->wrapContentHorizontally();
	mPreviousImage->addButtonListener(this);

	mButtonBar = new NativeUI::HorizontalLayout();

	mButtonBar->fillSpaceHorizontally();
	mButtonBar->wrapContentVertically();

	mButtonBar->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

	mButtonBar->addChild(mPreviousImage);
	mButtonBar->addChild(mNextImage);

	mMainLayout = new NativeUI::VerticalLayout();

	mMainLayout->fillSpaceHorizontally();
	mMainLayout->fillSpaceVertically();

	imageLayout = new NativeUI::HorizontalLayout();
	imageLayout->fillSpaceHorizontally();
	imageLayout->fillSpaceVertically();
	imageLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);

	LoadImages();

	mCurrentPosition = DEFAULT_IMAGE_INDEX;
	imageLayout->addChild(mImages[DEFAULT_IMAGE_INDEX]);
	mImages[DEFAULT_IMAGE_INDEX]->showImage();
	imageLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

	mMainLayout->addChild(imageLayout);
	mMainLayout->addChild(mButtonBar);

	this->setMainWidget(mMainLayout);
}

void ImageScreenWithButtons::LoadImages() {
	// Set the image indexes inside the resources according to the resolution.
	int firstImage = RES_FIRST_IMAGE_LARGE;
	int lastImage = RES_LAST_IMAGE_LARGE;
	if (mScreenWidth <= SMALL_SCREEN_RESOLUTION)
	{
		firstImage = RES_FIRST_IMAGE_SMALL;
		lastImage = RES_LAST_IMAGE_SMALL;
	} else if (mScreenHeight <= MEDIUM_SCREEN_RESOLUTION)
	{
		firstImage = RES_FIRST_IMAGE_MEDIUM;
		lastImage = RES_LAST_IMAGE_MEDIUM;
	}

	// Compute the number of images.
	mImagesSize = lastImage - firstImage - 1;

	// Create the image widgets.
	for (int i = 0; i < mImagesSize; i++) {
		// Byte index in the resource data.
		int pos = 0;

		// Create an image object.
		mImages[i] = new CustomImage();

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
		maCreateImageFromData(mImages[i]->getHandle(), resID, pos,
				maGetDataSize(resID) - mImages[i]->getName().length() - 1);
	}
}

 void ImageScreenWithButtons::buttonClicked(Widget* button) {
	if (mNextImage == button) {
		int lastPosition = mCurrentPosition;
		mCurrentPosition++;
		mCurrentPosition %= mImagesSize;

		imageLayout->removeChild(mImages[lastPosition]);
		mImages[mCurrentPosition]->showImage();
		imageLayout->addChild(mImages[mCurrentPosition]);
	} else if (mPreviousImage == button) {
		int lastPosition = mCurrentPosition;
		mCurrentPosition = mImagesSize + (mCurrentPosition - 1);
		mCurrentPosition %= mImagesSize;

		imageLayout->removeChild(mImages[lastPosition]);
		mImages[mCurrentPosition]->showImage();
		imageLayout->addChild(mImages[mCurrentPosition]);
	}
}

bool ImageScreenWithButtons::readStringFromResource(MAHandle resID, int& pos,
		MAUtil::String& output) const {
	// Get all the characters on one read.

	// Get the length of the string stored as a .pstring
	// (Pascal string). The first byte contains the length.
	byte stringLen = 0;
	maReadData(resID, (void*) &stringLen, pos, sizeof(byte));
	if (stringLen > maGetDataSize(resID) || stringLen <= 0) {
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
