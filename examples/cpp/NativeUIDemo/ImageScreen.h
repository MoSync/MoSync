/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _IMAGESCREEN_H
#define _IMAGESCREEN_H

#include <NativeUI/Screen.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Button.h>
#include <NativeUI/Image.h>
#include <NativeUI/Label.h>

/**
 * An extension to the Image widget.
 */
class CustomImage: public NativeUI::Image {
public:
	/**
	 * Set the image shown in the widget.
	 */
	void showImage() {
		NativeUI::Image::setImage(mImageHandle);
	}

	/*
	 * Setter for the image handle.
	 * @param The new handle for the image.
	 */
	void setHandle(int h) {
		mImageHandle = h;
	}

	/*
	 * Setter for the image name.
	 * @param name The new name of the image.
	 */
	void setName(MAUtil::String name) {
		mName = name;
	}

	/*
	 * Getter for the image handle.
	 */
	MAHandle getHandle() {
		return mImageHandle;
	}

	/*
	 * Getter for the image name.
	 */
	MAUtil::String getName() {
		return mName;
	}

private:
	/**
	 * The handle of the image.
	 */
	MAHandle mImageHandle;

	/**
	 * The image's name.
	 */
	MAUtil::String mName;
};

class ImageScreenWithButtons: public NativeUI::Screen, public NativeUI::ButtonListener {
public:
	ImageScreenWithButtons();
	~ImageScreenWithButtons();

	void CreateUI();
	void LoadImages();

	//the button clicked event handler
	virtual void buttonClicked(Widget* button);

	bool readStringFromResource(MAHandle resID, int& pos,
			MAUtil::String& output) const;
private:
	NativeUI::Button* mNextImage;
	NativeUI::Button* mPreviousImage;
	NativeUI::HorizontalLayout* mButtonBar;
	MAUtil::Vector<CustomImage*> mImages;
	NativeUI::VerticalLayout* mMainLayout;
	NativeUI::HorizontalLayout* mImageLayout;
	NativeUI::HorizontalLayout* mImageNameLayout;
	NativeUI::Label* mImageName;
	int mScreenWidth;
	int mScreenHeight;
	int mCurrentPosition;
	int mImagesSize;

	/**
	 * Ads banner that will be added on top of the screen.
	 */
	Ads::Banner* mAdsBanner;
};

#endif //_IMAGESCREEN_H
