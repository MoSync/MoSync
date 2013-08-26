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

/*
 * Utils.h
 *
 *  Created on: Jan 28, 2013
 *      Author: mvasiliniuc
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <NativeUI/Layout.h>

using namespace NativeUI;

namespace TestUtils
{
	/**
	 * Adds a vertical spacer of height 5.
	 * @param aLayout
	 */
	void addVerticalSpacerToLayout(Layout* aLayout)
	{
		VerticalLayout* spacer = new VerticalLayout();
		spacer->setHeight(5);
		aLayout->addChild(spacer);
	}

	/**
	 * Adds a horizontal spacer of height 5.
	 * @param aLayout
	 */
	void addHorizontalSpacerToLayout(Layout* aLayout)
	{
		HorizontalLayout* spacer = new HorizontalLayout();
		spacer->setWidth(5);
		aLayout->addChild(spacer);
	}

	/**
	 * The getScreenSize function. This function returns the width of the device.
	 * When the device is in LandScape mode, this function returns the smallest
	 * of the two (length or width).
	 * @return The screen width/height.
	 */
	int getScreenSize() {
		int size = 0;
		MAExtent scrSize = maGetScrSize();
		if (EXTENT_X(scrSize) > EXTENT_Y(scrSize) ) {
			size = EXTENT_Y(scrSize);
		} else {
			size = EXTENT_X(scrSize);
		}
		return size;
	}

	/**
	 * The getFontSize function. This function returns an integer, based on the ScreenSize.
	 * Big ScreenSizes need big FontSizes.
	 * @return The font size suited for the ScreenSize of the device.
	 */
	int getFontSize() {
		int returnSize = 12;
		int width = getScreenSize();
		if (width >= 720) {
			returnSize = 32;
		} else if (width >= 400) {
			returnSize = 24;
		} else if (width >= 320) {
			returnSize = 18;
		}
		return returnSize;
	}
}

#endif /* UTILS_H_ */
