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

/** 
* \file Image.cpp
* \brief Image widget, with position, size, and autosizing control.
* \author Patrick Broman and Niklas Nummelin
*/

#include "Image.h"
#include <MAUtil/Graphics.h>

namespace MAUI {

	Image::Image(
		int x, 
		int y, 
		int width, 
		int height,
		MAHandle res, 
		bool mAutoSizeX, 
		bool mAutoSizeY)
			: Widget(x, y, width, height),
			mResource(res),
			mAutoSizeX(mAutoSizeX),
			mAutoSizeY(mAutoSizeY)
	{ 
		// TODO: Document why commented out or delete.
		//setDrawBackground(false);
		setResource(res);
	}

	bool Image::isTransparent() const {
		return true;
	}

	void Image::drawWidget() {
		if(mResource) {
			Gfx_drawImage(
				mResource, 
				(mPaddedBounds.width>>1)-(mImageWidth>>1), 
				(mPaddedBounds.height>>1)-(mImageHeight>>1));
		}
	}

	void Image::setResource(MAHandle res) {
		mResource = res;
		if(res == 0) {
			return;
		}
		MAExtent imgSize = maGetImageSize(res);
		mImageWidth = EXTENT_X(imgSize);
		mImageHeight = EXTENT_Y(imgSize);
		if(mAutoSizeX) {
			this->setWidth(EXTENT_X(imgSize));
		}
		if(mAutoSizeY) {
			this->setHeight(EXTENT_Y(imgSize));
		}
		requestRepaint();
	}

	MAHandle Image::getResource() const {
		return mResource;
	}
}
