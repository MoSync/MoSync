/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "Image.h"
#include <MAUtil/Graphics.h>

namespace MAUI {

	Image::Image(int x, int y, int width, int height,
		MAHandle res, bool mAutoSizeX, bool mAutoSizeY)
		: Widget(x, y, width, height),
		mResource(res),
		mAutoSizeX(mAutoSizeX),
		mAutoSizeY(mAutoSizeY)
	{ 
		//setDrawBackground(false);
		setResource(res);
	}

	bool Image::isTransparent() const {
		return true;
	}

	void Image::drawWidget() {
		if(mResource) {
			Gfx_drawImage(mResource, (mPaddedBounds.width>>1)-(mImageWidth>>1), (mPaddedBounds.height>>1)-(mImageHeight>>1));
		}
	}

	void Image::setResource(MAHandle res) {
		mResource = res;
		if(res == 0) return;
		MAExtent imgSize = maGetImageSize(res);
		mImageWidth = EXTENT_X(imgSize);
		mImageHeight = EXTENT_Y(imgSize);
		if(mAutoSizeX) this->setWidth(EXTENT_X(imgSize));
		if(mAutoSizeY) this->setHeight(EXTENT_Y(imgSize));
	}

	MAHandle Image::getResource() const {
		return mResource;
	}
}
