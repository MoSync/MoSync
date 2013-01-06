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

namespace Transitions {

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
	}
}
