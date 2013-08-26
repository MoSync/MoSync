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

#include "ImageScreen.h"
#include <MAUI/Image.h>
#include "Util.h"
#include "MAHeaders.h"

ImageScreen::ImageScreen(Screen *previous) : previous(previous) {

	mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, 1, 2);
	softKeys = createSoftKeyBar(30, "", "Back");
	Image *image = new Image(0, 0, scrWidth, scrHeight-softKeys->getHeight(), NULL, false, false, RES_IMAGE);
	image->setSkin(gSkin);
	image->setDrawBackground(true);
	mainLayout->add(image);
	mainLayout->add(softKeys);

	mainLayout->setSkin(NULL);
	mainLayout->setDrawBackground(true);
	mainLayout->setBackgroundColor(0);

	this->setMain(mainLayout);
}

ImageScreen::~ImageScreen() {
}

void ImageScreen::keyPressEvent(int keyCode, int nativeCode) {
	if(keyCode == MAK_SOFTRIGHT) {
		previous->show();
	}
}

void ImageScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);
	if(softKeys->contains(p)) {
		if(softKeys->getChildren()[0]->contains(p)) {
			// Do nothing
		}
		else if(softKeys->getChildren()[1]->contains(p)) {
			keyPressEvent(MAK_SOFTRIGHT, 0);
		}
	}
}

void ImageScreen::pointerReleaseEvent(MAPoint2d point) {
}
