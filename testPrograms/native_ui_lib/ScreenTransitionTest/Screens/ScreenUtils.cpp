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
 * @file ScreenUtils.cpp
 * @author Mircea Vasiliniuc
 * @date 28 Nov 2012
 *
 * @brief Screen utils.
 */

#include "ScreenUtils.h"

#include <NativeUI/VerticalLayout.h>
#include <NativeUI/HorizontalLayout.h>

namespace ScreenTransitionTest {

	namespace ScreenUtils {

		PLATFORM_TYPE currentPlatform = OS_UNKNOWN;
		/**
		 * Screen size.
		 */
		int gScreenWidth = 0;
		int gScreenHeight = 0;

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

		PLATFORM_TYPE getCurrentPlatform()
		{
			if ( OS_UNKNOWN == currentPlatform )
			{
				// Usual size for a buffer.
				int BUF_SIZE = 256;

				// Used in determining the platform.
				char platform[BUF_SIZE];

				maGetSystemProperty("mosync.device.OS", platform, BUF_SIZE);

				if (strcmp(platform, "Android") == 0)
				{
					currentPlatform = OS_ANDROID;
				}
				else if (strcmp(platform, "iPhone OS") == 0)
				{
					currentPlatform = OS_IOS;
				}
				else
				{
					currentPlatform = OS_WIN;
				}
			}
			return currentPlatform;
		}

		void addVerticalSpacerToLayout(NativeUI::VerticalLayout* aLayout, short aSpacerHeight)
		{
			if ( NULL != aLayout )
			{
				NativeUI::VerticalLayout *spacerTitle = new NativeUI::VerticalLayout();
				spacerTitle->setSize(aLayout->getWidth(), aSpacerHeight);
				aLayout->addChild(spacerTitle);
			}
		}

		void addHorizontalSpacerToLayout(NativeUI::HorizontalLayout* aLayout, short aSpacerWidth)
		{
			if ( NULL != aLayout )
			{
				NativeUI::HorizontalLayout *spacerTitle = new NativeUI::HorizontalLayout();
				spacerTitle->setSize(aLayout->getHeight(), aSpacerWidth);
				aLayout->addChild(spacerTitle);
			}
		}

		// Screen transition
		MAUtil::Map<int, char*> mScreenTransitionNameMap;
		MAUtil::Map<int, int> mScreenTransitionSymmetricMap;

		void fillScreenTransitionMaps()
		{
			// Fill name map
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_NONE, "None");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_FLIP_FROM_LEFT, "Flip from left");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT, "Flip from right");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_CURL_UP, "Curl up");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_CURL_DOWN, "Curl down");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_SLIDE_LEFT, "Slide left");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_SLIDE_RIGHT, "Slide right");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_FADE_IN, "Fade in");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_FADE_OUT, "Fade out");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_SWIVEL_IN, "Swivel in");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_SWIVEL_OUT, "Swivel out");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD, "Turnstile foreward");
			mScreenTransitionNameMap.insert(MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD, "Turnstile backward");
			// Fill symmetric map
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_NONE, MAW_TRANSITION_TYPE_NONE);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_FLIP_FROM_LEFT, MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT, MAW_TRANSITION_TYPE_FLIP_FROM_LEFT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_CURL_UP, MAW_TRANSITION_TYPE_CURL_DOWN);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_CURL_DOWN, MAW_TRANSITION_TYPE_CURL_UP);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_SLIDE_LEFT, MAW_TRANSITION_TYPE_SLIDE_RIGHT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_SLIDE_RIGHT, MAW_TRANSITION_TYPE_SLIDE_LEFT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_FADE_IN, MAW_TRANSITION_TYPE_FADE_OUT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_FADE_OUT, MAW_TRANSITION_TYPE_FADE_IN);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_SWIVEL_IN, MAW_TRANSITION_TYPE_SWIVEL_OUT);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_SWIVEL_OUT, MAW_TRANSITION_TYPE_SWIVEL_IN);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD, MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD);
			mScreenTransitionSymmetricMap.insert(MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD, MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD);
		}

		/**
		 * Returns the transition name.
		 */
		const char* getTransitionName(MAWScreenTransitionType aTransitionsType)
		{
			if ( mScreenTransitionNameMap.size() <= 0 )
			{
				fillScreenTransitionMaps();
			}
			return mScreenTransitionNameMap[aTransitionsType];
		}

		/**
		 * Returns symmetric transition.
		 */
		int getSimetricTransition(MAWScreenTransitionType aTransitionsType)
		{
			if ( mScreenTransitionSymmetricMap.size() <= 0 )
			{
				fillScreenTransitionMaps();
			}
			return mScreenTransitionSymmetricMap[aTransitionsType];
		}
	}
}
