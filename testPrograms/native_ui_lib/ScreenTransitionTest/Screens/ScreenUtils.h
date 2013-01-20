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

/**
 * @file ScreenUtils.cpp
 * @author Mircea Vasiliniuc
 * @date 28 Nov 2012
 *
 * @brief Screen utils.
 */

#ifndef SCREEN_UTILS_H_
#define SCREEN_UTILS_H_

#define TITLE_FONT_SIZE 22
#define SPACER_HEIGHT 50
#define SPACER_WIDTH 15
#define FOOTER_HEIGHT 120
#define LIST_ITEM_HEIGHT 80

#define BLACK_COLOR 0x000000
#define WHITE_COLOR 0xffffff
#define SELECTED_COLOR 0x5b3f23

#include <MAUtil/Map.h>
#include <IX_WIDGET.h>

namespace NativeUI
{
	class VerticalLayout;
	class HorizontalLayout;
}

namespace ScreenTransitionTest
{
	namespace ScreenUtils
	{
		typedef enum
		{
			OS_IOS = 0,
			OS_ANDROID,
			OS_WIN,
			OS_UNKNOWN
		} PLATFORM_TYPE;

		/**
		 * Screen size.
		 */
		extern int gScreenWidth;
		extern int gScreenHeight;

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
		 * Retrieves the current platform.
		 */
		PLATFORM_TYPE getCurrentPlatform();

		/**
		 * Add vertical spacer.
		 */
		void addVerticalSpacerToLayout(NativeUI::VerticalLayout* aLayout, short aSpacerHeight);

		/**
		 * Add horizontal spacer.
		 */
		void addHorizontalSpacerToLayout(NativeUI::HorizontalLayout* aLayout, short aSpacerWidth);

		// Screen transition
		const int kiOSTransitionsArray[] = {MAW_TRANSITION_TYPE_NONE,
				MAW_TRANSITION_TYPE_FLIP_FROM_LEFT,
				MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT,
				MAW_TRANSITION_TYPE_CURL_UP,
				MAW_TRANSITION_TYPE_CURL_DOWN};

		const int kAndroidTransitionsArray[] = {MAW_TRANSITION_TYPE_NONE,
				MAW_TRANSITION_TYPE_SLIDE_LEFT,
				MAW_TRANSITION_TYPE_SLIDE_RIGHT,
				MAW_TRANSITION_TYPE_FADE_IN,
				MAW_TRANSITION_TYPE_FADE_OUT};

		const int kWindowsTransitionsArray[] = {MAW_TRANSITION_TYPE_NONE,
				MAW_TRANSITION_TYPE_SLIDE_LEFT,
				MAW_TRANSITION_TYPE_SLIDE_RIGHT,
				MAW_TRANSITION_TYPE_SWIVEL_IN,
				MAW_TRANSITION_TYPE_SWIVEL_OUT,
				MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD,
				MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD};

		/**
		 * Returns the transition name.
		 */
		const char* getTransitionName(MAWScreenTransitionType aTransitionsType);

		/**
		 * Returns symmetric transition.
		 */
		int getSimetricTransition(MAWScreenTransitionType aTransitionsType);

	} // end of ScreenUtils

} // end of ScreenTransitionTest

#endif //SCREEN_UTILS_H_
