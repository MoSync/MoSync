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
