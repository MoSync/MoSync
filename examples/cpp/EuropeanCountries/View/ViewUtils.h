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
 * @file ViewUtils.h
 * @author Bogdan Iusco
 *
 * @brief Util methods and constants for NativeUI widgets.
 */

#ifndef EC_VIEW_UTILS_H_
#define EC_VIEW_UTILS_H_

#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xFFFFFF
#define COLOR_LABEL_INFO 0x00FF00
#define COLOR_LABEL_DATA 0xFFFFFF

#define SPACER_HEIGHT 30

#include <IX_WIDGET.h>
#include <MAUtil/String.h>

namespace NativeUI
{
	class Label;
	class VerticalLayout;
}

namespace EuropeanCountries
{
	/**
	 * Screen size.
	 */
	extern int gScreenWidth;
	extern int gScreenHeight;

	/**
	 * Platform related values.
	 */
	enum PlatformType
	{
		PlatformTypeAndroid = 0,
		PlatformTypeiOS,
		PlatformTypeWP7
	};

	extern PlatformType gPlatformType;

	/**
	 * Background color for layouts.
	 */
	extern char* gLayoutBackgroundColor;

	/**
	 * Background color for title layout.
	 */
	extern char* gTitleBackgroundColor;

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
		const int width = 0,
		const int height = 0);

	/**
	 * Init platform type global variable.
	 */
	void initPlatformType();

	/**
	 * Create an NativeUI Label object with given values.
	 * @param text Text to set.
	 * @param fontColor Text font color.
	 * @param width Label's width in pixels or size constant.
	 * @param height Label's height in pixels or size constant.
	 * @return The created label object. Its ownership is passed to the caller.
	 */
	NativeUI::Label* createLabel(
		const MAUtil::String& text = "",
		const int fontColor = COLOR_LABEL_DATA,
		const int width =  MAW_CONSTANT_FILL_AVAILABLE_SPACE,
		const int height = MAW_CONSTANT_WRAP_CONTENT);


	/**
	 * Create an empty transparent VerticalLayout widget.
	 * @param height Layout's height.
	 * @param width Layout's width.
	 * @return The layout. Its ownership is passed to the caller.
	 */
	NativeUI::VerticalLayout* createSpacer(
		const int height = SPACER_HEIGHT,
		const int width = MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	/**
	 * Detects if the current platform is Android.
	 * @return true if the platform is Android, false otherwise.
	 */
	bool isAndroid();

	/**
	 * Detects if the current platform is iOS.
	 * @return true if the platform is iOS, false otherwise.
	 */
	bool isIOS();

	/**
	 * Detects if the current platform is Windows Phone.
	 * @return true if the platform is Windows Phone, false otherwise.
	 */
	bool isWindowsPhone();

} // end of EuropeanCountries

#endif /* EC_VIEW_UTILS_H_ */
