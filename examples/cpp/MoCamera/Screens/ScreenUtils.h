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

/**
 * @file ScreenUtils.h
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Utilities used in UI.
 */

#ifndef SCREENUTILS_H_
#define SCREENUTILS_H_

// Usual size for a buffer.
#define UTILS_BUF_SIZE 256

#define BLACK_COLOR 0x000000
#define SCREEN_TRANSITION_DURATION 300

#define ACTIVITY_INDICATOR_WIDTH 100 // Width of the activity indicator
#define ACTIVITY_INDICATOR_HEIGHT 100 // Height of the activity indicator

#include <maapi.h>

namespace NativeUI
{
	class ImageButton;
	class ButtonListener;
	class Widget;
}

namespace ScreenUtils
{
	/**
	 * Configures an ImageButton with the given parameters.
	 *
	 * @param imageButton the image button pointer.
	 * If null the function will fill this with a new ImageButton
	 * @param btnListener a lister for the given image button.
	 * @param normalImage the handle of the image that will appear
	 * as background image.
	 * @param pressedImage the handle of the image that will appear
	 * as background image when the button is pressed.
	 */
	void setupImageButton(NativeUI::ImageButton* imageButton,
			NativeUI::ButtonListener* btnListener,
			MAHandle normalImage,
			MAHandle pressedImage);

    /**
     * Set the size of each widget according
     * to the size of the container given and
     * widget/container width ratio.
     *
     * \param widget the widget that needs resize.
     * \param containerSizet size of the container.
     * \param ratio widget/container width ratio.
     */
    void resizeWidget(NativeUI::Widget* widget,
			const MAExtent& containerSize, float widthRatio);
} //ScreenUtils

#endif /* SCREENUTILS_H_ */
