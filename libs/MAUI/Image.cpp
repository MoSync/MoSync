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
			Gfx_drawImage(resource, (paddedBounds.width>>1)-(imageWidth>>1), (paddedBounds.height>>1)-(imageHeight>>1));
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
