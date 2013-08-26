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
 * @file ViewUtils.cpp
 * @author Bogdan Iusco
 *
 * @brief Util methods and constants for NativeUI widgets.
 */

#define LABEL_MAX_LINES 100

#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>
#include <mactype.h>

#include "ViewUtils.h"

namespace EuropeanCountries
{

	/**
	 * Screen size.
	 */
	int gScreenWidth = 0;
	int gScreenHeight = 0;

	/**
	 * Platform type.
	 */
	PlatformType gPlatformType;

	/**
	 * Background color for layouts.
	 */
	char* gLayoutBackgroundColor;

	/**
	 * Background color for title layout.
	 */
	char* gTitleBackgroundColor;

	/**
	 * Init global screen size constants.
	 * On Android and WP7 platforms should be used with default param value,
	 * while on iOS should be called with screen width and height values.
	 * @param width Width to set. If it's zero, maGetScrSize() syscall will be used
	 * to get the real value.
	 * @param height Height to set. If it's zero, maGetScrSize() syscall will be used
	 * to get the real value.
	 */
	void initScreenSizeConstants(
		const int width,
		const int height)
	{
		MAExtent screenSize = maGetScrSize();
		gScreenWidth = width;
		if  (gScreenWidth == 0)
		{
			gScreenWidth = EXTENT_X(screenSize);
		}

		gScreenHeight = height;
		if (gScreenHeight == 0)
		{
			gScreenHeight = EXTENT_Y(screenSize);
		}
	}

	/**
	 * Init platform type global variable.
	 */
	void initPlatformType()
	{
		char platform[NativeUI::BUF_SIZE];
		maGetSystemProperty("mosync.device.OS", platform, NativeUI::BUF_SIZE);

		for (unsigned int i = 0; i < strlen(platform); i++)
		{
			platform[i] = tolower(platform[i]);
		}

		if (strcmp(platform,"android") == 0)
		{
			gPlatformType = PlatformTypeAndroid;
		}
		else if (strstr(platform,"iphone") != NULL)
		{
			gPlatformType = PlatformTypeiOS;
		}
		else
		{
			gPlatformType = PlatformTypeWP7;
		}

		// Set background color variables.
		gLayoutBackgroundColor = "00000000";
		gTitleBackgroundColor  = "50000000";
	}

	/**
	 * Create an NativeUI Label object with given values.
	 * @param text Text to set.
	 * @param fontColor Text font color.
	 * @param width Label's width in pixels or size constant.
	 * @param height Label's height in pixels or size constant.
	 * @return The created label object. Its ownership is passed to the caller.
	 */
	NativeUI::Label* createLabel(
		const MAUtil::String& text,
		const int fontColor,
		const int width,
		const int height)
	{
		NativeUI::Label* label = new NativeUI::Label();
		label->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
		label->setText(text);
		label->setWidth(width);
		label->setHeight(height);
		label->setFontColor(fontColor);
		label->setMaxNumberOfLines(LABEL_MAX_LINES);
		return label;
	}

	/**
	 * Create an empty transparent VerticalLayout widget.
	 * @param height Layout's height.
	 * @param width Layout's width.
	 * @return The layout. Its ownership is passed to the caller.
	 */
	NativeUI::VerticalLayout* createSpacer(
		const int height,
		const int width)
	{
		NativeUI::VerticalLayout* layout = new NativeUI::VerticalLayout();
		layout->setHeight(height);
		layout->setWidth(width);
		if (!isWindowsPhone())
		{
			layout->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gLayoutBackgroundColor);
		}
		return layout;
	}

	/**
	 * Detects if the current platform is Android.
	 * @return true if the platform is Android, false otherwise.
	 */
	bool isAndroid()
	{
		return gPlatformType == PlatformTypeAndroid ? true : false;
	}

	/**
	 * Detects if the current platform is iOS.
	 * @return true if the platform is iOS, false otherwise.
	 */
	bool isIOS()
	{
		return gPlatformType == PlatformTypeiOS ? true : false;
	}

	/**
	 * Detects if the current platform is Windows Phone.
	 * @return true if the platform is Windows Phone, false otherwise.
	 */
	bool isWindowsPhone()
	{
		return gPlatformType == PlatformTypeWP7 ? true : false;
	}

} // end of EuropeanCountries
