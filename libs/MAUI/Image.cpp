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

	Image::Image(int x, int y, int width, int height, Widget* parent, bool autoSizeX, bool autoSizeY, MAHandle res) 
		: Widget(x, y, width, height, parent),
		resource(res),
		autoSizeX(autoSizeX),
		autoSizeY(autoSizeY)
	{ 
		setDrawBackground(false);
		setResource(res);

	}

	void Image::drawWidget() {
		if(resource) {
			Gfx_drawImage(resource, (mPaddedBounds.width>>1)-(imageWidth>>1), (mPaddedBounds.height>>1)-(imageHeight>>1));
		}
	}

	void Image::setResource(MAHandle res) {
		resource = res;
		if(res == 0) return;
		MAExtent imgSize = maGetImageSize(res);
		imageWidth = EXTENT_X(imgSize);
		imageHeight = EXTENT_Y(imgSize);
		if(autoSizeX) this->setWidth(EXTENT_X(imgSize));
		if(autoSizeY) this->setHeight(EXTENT_Y(imgSize));
	}

	MAHandle Image::getResource() const {
		return resource;
	}
}
