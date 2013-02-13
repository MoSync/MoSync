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
