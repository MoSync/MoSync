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
#include "iphone_helpers.h"
#include "Platform.h"
#include "Syscall.h"

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
	self = [super init];
	if (self)
	{
		mAllowedScreenOrientations = MA_SCREEN_ORIENTATION_PORTRAIT;
		mCurrentScreenOrientation = UIInterfaceOrientationPortrait;
		mSupportedOrientations = UIInterfaceOrientationMaskPortrait;
	}
    return self;
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
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
		mSupportedOrientations = 0;
		mSupportedOrientations |= mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_PORTRAIT ?
		    UIInterfaceOrientationMaskPortrait : mSupportedOrientations;
		mSupportedOrientations |= mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN ?
            UIInterfaceOrientationMaskPortraitUpsideDown : mSupportedOrientations;
        mSupportedOrientations |= mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT ?
            UIInterfaceOrientationMaskLandscapeLeft : mSupportedOrientations;
		mSupportedOrientations |= mAllowedScreenOrientations & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT ?
		    UIInterfaceOrientationMaskLandscapeRight : mSupportedOrientations;
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

/**
 * Get the supported orientations.
 * Used in iOS 6.0 and later.
 * @return A bit mask specifying which orientations are supported.
 */
-(UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return mSupportedOrientations;
}

/**
 * Get the screen size based on the current orientation.
 * @return Screen size in pixels.
 */
-(CGSize)screenSize
{
	int width, height;
	CGSize screenSize;
	getScreenResolution(width, height);
	UIInterfaceOrientation interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
	if(UIInterfaceOrientationIsPortrait(interfaceOrientation))
	{
		screenSize = CGSizeMake(width, height);
	}
	else
	{
		screenSize = CGSizeMake(height, width);
	}
	return screenSize;
}

@end

/**
 * Return a boolean value indicating whether the view controller supports the specified orientation.
 * Deprecated in iOS 6.0.
 * @param interfaceOrientation The orientation of the app’s user interface after the rotation.
 * @return YES if the view controller auto-rotates its view to the specified orientation, otherwise NO.
 */
BOOL MoSync_IsInterfaceOrientationSupported(UIInterfaceOrientation interfaceOrientation)
{
	return [[ScreenOrientation getInstance] isInterfaceOrientationSupported:interfaceOrientation];
}

/**
 * Returns all of the interface orientations that the view controller supports.
 * Available in iOS 6.0 and later.
 * @return A mask with supported orientations.
 */
NSUInteger MoSync_SupportedInterfaceOrientations()
{
	return [[ScreenOrientation getInstance] supportedInterfaceOrientations];
}

/**
 * Check if the current screen size has changed. If so send EVENT_TYPE_SCREEN_CHANGED event.
 * It's send only for non NativeUI applications. Once the NativeUI module is used
 * this event is not sent.
 * Usually the screen size changes when rotating device from portrait to landscape
 * and the other way around.
 * @param fromOrientation The old orientation of the user interface.
 */
void MoSync_OrientationChanged(UIInterfaceOrientation fromOrientation)
{
	UIInterfaceOrientation currentOrientation = [UIApplication sharedApplication].statusBarOrientation;
	if ((UIInterfaceOrientationIsPortrait(fromOrientation) &&
		UIInterfaceOrientationIsLandscape(currentOrientation)) ||
		(UIInterfaceOrientationIsPortrait(currentOrientation) &&
		UIInterfaceOrientationIsLandscape(fromOrientation)))
	{
		Base::gSyscall->deviceOrientationChanged();

		MAEvent event;
		event.type = EVENT_TYPE_SCREEN_CHANGED;
		Base::gEventQueue.put(event);
	}
}
