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
