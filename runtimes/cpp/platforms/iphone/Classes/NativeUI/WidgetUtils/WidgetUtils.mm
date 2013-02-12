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
 * @file WidgetUtils.mm
 * @author Bogdan Iusco
 * @date 17 July 2012
 *
 * @brief Utility for NativeUI module.
 */


#import "WidgetUtils.h"
#import "ScreenWidget.h"

/**
 * "true" and "false" string constants used by Native UI widgets.
 */
NSString* const kWidgetTrueValue = @"true";
NSString* const kWidgetFalseValue = @"false";

#pragma mark -
#pragma mark Screen Transitions Utils

namespace ScreenTransitionsUtils {
int doScreenTransition(UIWindow* aWindow, ScreenWidget* aScreenWidget, NSNumber* aScreenTransitionType, NSNumber* aScreenTransitionDuration)
{
    if ( 0 > [aScreenTransitionDuration integerValue]) {
        return MAW_RES_INVALID_SCREEN_TRANSITION_DURATION;
    }

    //Obtain screen transition option or return error code if not available on iOS.
    UIViewAnimationOptions options;
    switch ([aScreenTransitionType integerValue]) {
        case MAW_TRANSITION_TYPE_NONE:
            options = UIViewAnimationOptionTransitionNone;
            break;
        case MAW_TRANSITION_TYPE_FLIP_FROM_LEFT:
            options = UIViewAnimationOptionTransitionFlipFromLeft;
            break;
        case MAW_TRANSITION_TYPE_FLIP_FROM_RIGHT:
            options = UIViewAnimationOptionTransitionFlipFromRight;
            break;
        case MAW_TRANSITION_TYPE_CURL_UP:
            options = UIViewAnimationOptionTransitionCurlUp;
            break;
        case MAW_TRANSITION_TYPE_CURL_DOWN:
            options = UIViewAnimationOptionTransitionCurlDown;
            break;
        default:
            return MAW_RES_INVALID_SCREEN_TRANSITION_TYPE;
    }

    // after animation block
    void (^handleTransitionDoneBlock)(BOOL) = ^(BOOL isTransFinished) {
        if ( isTransFinished ) {
            // If needed in the future.
        }
    };

    // animation block
    void (^doAnimationBlock)(void) = ^(void) {
        if ( aWindow && aScreenWidget )
        {
            aWindow.rootViewController = [aScreenWidget getController];
        }
    };

    // convert milliseconds to seconds for duration parameter of transition.
    NSTimeInterval screenTransitionDuration = 0.001 * [aScreenTransitionDuration doubleValue];

    [UIView transitionWithView:aWindow
                      duration:screenTransitionDuration
                       options:options
                    animations:doAnimationBlock completion:handleTransitionDoneBlock];

    return MAW_RES_OK;
}
}