/*
 Copyright (C) 2013 MoSync AB

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
#include "ScreenUtils.h"

// Include NativeUI.
#include <IX_WIDGET.h>

#include "NativeUI/ImageButton.h"

namespace ScreenUtils
{
	void setupImageButton(
			NativeUI::ImageButton* imageButton,
			NativeUI::ButtonListener* btnListener,
			MAHandle normalImage,
			MAHandle pressedImage )
	{
		if ( NULL == imageButton)
		{
			return;
		}
		imageButton->wrapContentHorizontally();
		imageButton->wrapContentVertically();
		imageButton->setBackgroundImage(normalImage);
		imageButton->setPressedImage(pressedImage);
		imageButton->addButtonListener(btnListener);

		// Set initial image button size.
		MAExtent imageButtonSize = maGetImageSize(normalImage);
		imageButton->setWidth(EXTENT_X(imageButtonSize));
		imageButton->setHeight(EXTENT_Y(imageButtonSize));
	}

	void resizeWidget(
			NativeUI::Widget* widget,
			const MAExtent& containerSize,
			float widthRatio )
	{
		// set button size according to the container size.
		int widthValue = (int)(EXTENT_X(containerSize) * widthRatio);
		int heightValue = widget->getHeight() * widthValue/widget->getWidth();

		widget->setSize(widthValue, heightValue);
	}
}
