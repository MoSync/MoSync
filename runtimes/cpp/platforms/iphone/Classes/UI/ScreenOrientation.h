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
 * @file ScreenOrientation.h
 * @author Bogdan Iusco
 * @date 30 Nov 2011
 *
 * @brief Provides support for screen orientation.
 * Contains implementation for screen orientation syscalls.
 */

#import <Foundation/Foundation.h>

/**
 * @brief Singleton pattern.
 * Provides support for screen orientation.
 * Contains implementation for screen orientation syscalls.
 * Default value for allowed screen orientation is portrait mode.
 */
@interface ScreenOrientation : NSObject
{
    /**
     * Store allowed screen orientations.
     * A bitmask created using the next values:
     * - MA_SCREEN_ORIENTATION_PORTRAIT
     * - MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
     * - MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
     * - MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
     */
    int mAllowedScreenOrientations;

    /**
     * Store the current screen orientation.
     */
    UIInterfaceOrientation mCurrentScreenOrientation;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(ScreenOrientation*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Set supported screen orientations.
 * @param orientations A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask can be created using MA_SCREEN_ORIENTATION values.
 * @return One of the next constants:
 * - #MA_SCREEN_ORIENTATION_RES_OK
 * - #MA_SCREEN_ORIENTATION_RES_INVALID_VALUE
 */
-(int) setSupportedOrientations:(const int) orientations;

/**
 * Get supported screen orientations.
 * @return A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask is created using MA_SCREEN_ORIENTATION values.
 */
-(int) getSupportedOrientations;

/**
 * Check if a given orientation is supported by the application.
 * @param orientation The given orientation.
 * @return True if the orientation is supported by the application, false oftherwise.
 */
-(bool) isInterfaceOrientationSupported:(UIInterfaceOrientation) orientation;

/**
 * Check if portrait mode is supported by the application.
 * @return True if portrait mode is supported by the application, false otherwise.
 */
-(bool) isPortraitModeSupported;

/**
 * Check if portrait upside down mode is supported by the application.
 * Device is in portrait upside down mode if it's in portrait mode but upside down,
 * with the device held upright and the home button at the top.
 * @return True if portrait upside down mode is supported by the application, false otherwise.
 */
-(bool) isPortraitUpsideDownModeSupported;

/**
 * Check if landscape left mode is supported by the application.
 * The device is in landscape left mode if it's held upright and the home button on the left side.
 * @return True if landscape left mode is supported by the application, false otherwise.
 */
-(bool) isLandscapeLeftModeSupported;

/**
 * Check if landscape right mode is supported by the application.
 * The device is in landscape right mode if it's held upright and the home button on the right side.
 * @return True if landscape right mode is supported by the application, false otherwise.
 */
-(bool) isLandscapeRightModeSupported;

/**
 * Called when the screen orientation has changed.
 * @param orientation New screen orientation.
 */
-(void) currentOrientationChanged:(UIInterfaceOrientation) orientation;

/**
 * Get current screen orientation.
 * @return One of the next constants:
 * - MA_SCREEN_ORIENTATION_PORTRAIT
 * - MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
 * - MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
 * - MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
 */
-(int) getCurrentScreenOrientation;

@end
