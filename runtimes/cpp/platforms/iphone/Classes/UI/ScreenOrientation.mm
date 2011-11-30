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
-(int) setSupportedOrientations(const int orientations)
{
    // Check if orientations param is valid.
    if ( !(orientations & MA_SCREEN_ORIENTATION_PORTRAIT) &&
         !(orientations & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN) &&
         !(orientations & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT) &&
         !(orientations & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) )
    {
        return MA_SCREEN_ORIENTATION_RES_INVALID_VALUE;
    }

    // Store allowed screen orientations.
    mAllowedScreenOrientations = orientations;
    return MA_SCREEN_ORIENTATION_RES_OK;
}

/**
 * Get supported screen orientations.
 * @return A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask is created using MA_SCREEN_ORIENTATION values.
 */
-(int) getSupportedOrientations()
{
    return mAllowedScreenOrientations;
}

@end
