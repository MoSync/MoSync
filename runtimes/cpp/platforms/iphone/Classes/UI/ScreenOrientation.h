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
-(int) setSupportedOrientations(const int orientations);

/**
 * Get supported screen orientations.
 * @return A bitmask consisting of flags describing the supported screen orientations.
 * The bitmask is created using MA_SCREEN_ORIENTATION values.
 */
-(int) getSupportedOrientations();
@end
