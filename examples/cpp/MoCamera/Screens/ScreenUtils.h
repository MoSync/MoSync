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
