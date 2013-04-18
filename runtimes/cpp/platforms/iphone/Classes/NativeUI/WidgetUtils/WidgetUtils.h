/*
 Copyright (C) 2012 MoSync AB

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
 * @file WidgetUtils.h
 * @author Bogdan Iusco
 * @date 17 July 2012
 *
 * @brief Utility for NativeUI module.
 */

#include "Syscall.h"

@class ScreenWidget;

/**
 * If the value is negative it returns MAW_RES_INVALID_PROPERTY_VALUE constant.
 */
#define TEST_FOR_NEGATIVE_VALUE(value) if (0 > value)\
{\
return  MAW_RES_INVALID_PROPERTY_VALUE; \
}

// Used for converting milliseconds in seconds.
#define SECOND 1000.0

/**
 * Enums for widget size. Since iOS views are using only fixed sizes, the wrap and fill properties
 * are custom.
 */
typedef enum {
	WidgetAutoSizeFixed,
	WidgetAutoSizeFillParent,
	WidgetAutoSizeWrapContent
} WidgetAutoSizeType;


/**
 * "true" and "false" string constants used by Native UI widgets.
 */
extern NSString* const kWidgetTrueValue;
extern NSString* const kWidgetFalseValue;

namespace Base
{
    UIFont* getUIFontObject(MAHandle fontHandle);
};

#pragma mark Screen Transitions Utils

@interface ScreenTransitionsUtils : NSObject

/**
 * Switches two screen/views using animated transitions.
 *
 * @param aWindow The source UIWindow containg the current view.
 * @param aScreenWidget The screen that needs to be shown next using animated transition.
 * @param aScreenTransitionType The screen transition type used in the switch.
 * @param aScreenTransitionDuration The duration of the animated transitions in milliseconds.
 *
 * @returns Any of the following result codes:
 * - MAW_RES_OK if the screen could be shown.
 * - MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not available
 * on the running platform.
 * - MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the value representing the
 * duration of the screen transition is invalid.
 */
+(int) doScreenTransition:(UIWindow* )aWindow
                 toScreen:(ScreenWidget*) aScreenWidget
       withTransitionType:(NSNumber*) aScreenTransitionType
    andTransitionDuration:(NSNumber*) aScreenTransitionDuration;

@end
