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
