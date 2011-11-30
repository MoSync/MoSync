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
 * @file ScreenOrientation.mm
 * @author Bogdan Iusco
 * @date 30 Nov 2011
 *
 * @brief Provides support for screen orientation.
 * Contains implementation for screen orientation syscalls.
 */

#include <helpers/cpp_defs.h>

#import "ScreenOrientation.h"

@implementation ScreenOrientation

static ScreenOrientation *sharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(ScreenOrientation*) getInstance
{
    if (!sharedInstance)
    {
        sharedInstance = [[ScreenOrientation alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

/**
 * Init method.
 */
-(id) init
{
    mAllowedScreenOrientations = MA_SCREEN_ORIENTATION_PORTRAIT;
    mCurrentScreenOrientation = UIInterfaceOrientationPortrait;
    return [super init];
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    [super dealloc];
}

/**
 * Set supported screen orientations.
 * @param orientations A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask can be created using MA_SCREEN_ORIENTATION values.
 * @return One of the next constants:
 * - #MA_SCREEN_ORIENTATION_RES_OK
 * - #MA_SCREEN_ORIENTATION_RES_INVALID_VALUE
 */
-(int) setSupportedOrientations:(const int) orientations
{
    // Check if orientations param is valid.
    if ( !(orientations & MA_SCREEN_ORIENTATION_PORTRAIT) &&
         !(orientations & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN) &&
         !(orientations & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT) &&
         !(orientations & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) )
    {
        return MA_SCREEN_ORIENTATION_RES_INVALID_VALUE;
    }
    else
    {
        // Store allowed screen orientations.
        mAllowedScreenOrientations = orientations;
        return MA_SCREEN_ORIENTATION_RES_OK;
    }
}

/**
 * Get supported screen orientations.
 * @return A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask is created using MA_SCREEN_ORIENTATION values.
 */
-(int) getSupportedOrientations
{
    return mAllowedScreenOrientations;
}

/**
 * Check if a given orientation is supported by the application.
 * @param orientation The given orientation.
 * @return True if the orientation is supported by the application, false oftherwise.
 */
-(bool) isInterfaceOrientationSupported:(UIInterfaceOrientation) orientation
{
    bool returnValue = false;
    switch (orientation)
    {
        case UIInterfaceOrientationPortrait:
            returnValue = [self isPortraitModeSupported];
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            returnValue = [self isPortraitUpsideDownModeSupported];
            break;
        case UIInterfaceOrientationLandscapeLeft:
            returnValue = [self isLandscapeLeftModeSupported];
            break;
        case UIInterfaceOrientationLandscapeRight:
            returnValue = [self isLandscapeRightModeSupported];
            break;
    }
    return returnValue;
}

/**
 * Check if portrait mode is supported by the application.
 * @return True if portrait mode is supported by the application, false otherwise.
 */
-(bool) isPortraitModeSupported
{
    return mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_PORTRAIT ? true : false;
}

/**
 * Check if portrait upside down mode is supported by the application.
 * Device is in portrait upside down mode if it's in portrait mode but upside down,
 * with the device held upright and the home button at the top.
 * @return True if portrait upside down mode is supported by the application, false otherwise.
 */
-(bool) isPortraitUpsideDownModeSupported
{
    return mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN ? true : false;
}

/**
 * Check if landscape left mode is supported by the application.
 * The device is in landscape left mode if it's held upright and the home button on the left side.
 * @return True if landscape left mode is supported by the application, false otherwise.
 */
-(bool) isLandscapeLeftModeSupported
{
    return mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT ? true : false;
}

/**
 * Check if landscape right mode is supported by the application.
 * The device is in landscape right mode if it's held upright and the home button on the right side.
 * @return True if landscape right mode is supported by the application, false otherwise.
 */
-(bool) isLandscapeRightModeSupported
{
    return mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT ? true : false;
}

/**
 * Called when the screen orientation has changed.
 * @param orientation New screen orientation.
 */
-(void) currentOrientationChanged:(UIInterfaceOrientation) orientation
{
    mCurrentScreenOrientation = orientation;
}

/**
 * Get current screen orientation.
 * @return One of the next constants:
 * - MA_SCREEN_ORIENTATION_PORTRAIT
 * - MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
 * - MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT
 * - MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT
 */
-(int) getCurrentScreenOrientation
{
    int returnValue = MA_SCREEN_ORIENTATION_PORTRAIT;
    switch (mCurrentScreenOrientation)
    {
        case UIInterfaceOrientationPortrait:
            returnValue = MA_SCREEN_ORIENTATION_PORTRAIT;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            returnValue = MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            returnValue = MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
            break;
        case UIInterfaceOrientationLandscapeRight:
            returnValue = MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
            break;
    }

    return returnValue;
}

@end
