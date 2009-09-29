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

#include "ImageScreen.h"
#include <MAUI/Image.h>
#include "Util.h"
#include "MAHeaders.h"

ImageScreen::ImageScreen(Screen *previous) : previous(previous) {
	mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, 1, 2);
	Widget *softKeys = createSoftKeyBar(30, "", "back");
	Image *image = new Image(0, 0, scrWidth, scrHeight-softKeys->getHeight(), NULL, false, false, RES_IMAGE);
	image->setSkin(gSkin);
	mainLayout->add(image);
	mainLayout->add(softKeys);
	this->setMain(mainLayout);
}

ImageScreen::~ImageScreen() {
}

void ImageScreen::keyPressEvent(int keyCode) {
	if(keyCode == MAK_SOFTRIGHT) {
		previous->show();
	}
}
