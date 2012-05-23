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
 * @file MoSyncScreenOrientationModule.cs
 * @author Spiridon Alexandru
 * @date 26 Feb 2012
 *
 * @brief Provides support for screen orientation.
 * Contains implementation for screen orientation syscalls.
 */
using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace MoSync
{
    public class MoSyncScreenOrientationModule : IIoctlModule
    {
        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            /**
		     * Set supported screen orientations.
		     * Currently implemented on iOS and WindowsPhone 7.1.
		     * @param orientations A bitmask consisting of flags describing the supported screen orientations.
		     * The bitmask can be created using \link #MA_SCREEN_ORIENTATION_ MA_SCREEN_ORIENTATION \endlink
		     * values.
		     * @return One of the next constants:
		     * - #MA_SCREEN_ORIENTATION_RES_OK
		     * - #MA_SCREEN_ORIENTATION_RES_NOT_SUPPORTED
		     * - #MA_SCREEN_ORIENTATION_RES_INVALID_VALUE
		     */
            ioctls.maScreenSetSupportedOrientations = delegate(int orientations)
            {
                // the bitmask contains the flags if the following order:
                // PORTRAIT, PORTRAIT_UPSIDE_DOWN, LANDSCAPE_LEFT and LANDSCAPE_RIGHT
                bool isPortrait = false;
                bool isLandscape = false;

                // if the first or the second bit is 1, it means that the portrait orientation is
                // currently accepted (because wp7 doesn't let you specify the
                // type of a portrait orientation when writing the SupportedOrientations
                // we consider that portrait is supported if PORTRAIT or PORTRAIT_UPSIDE_DOWN are
                // supported

                // check the first bit (PORTRAIT)g
                // ((A & (1 << bit)) != 0) checks if the bit 'bit - 1' is 1
                // For example, let's check if the bit 4 (starting from the least signifiant
                // bit = bit one) for A = 11001100 is 0 or not
                // 1<<3 -> 1000
                // 11001100 &
                // 00001000
                // --------
                // 00001000 != 0 -> the bit at position 4 is 1
                UInt32 o = UInt32.Parse(orientations.ToString());
                if ((o & 1) != 0)
                {
                    isPortrait = true;
                }
                // check the second bit (PORTRAIT_UPSIDE_DOWN)
                if ((o & (1 << 1)) != 0)
                {
                    isPortrait = true;
                }
                // check the third bit (LANDSCAPE_LEFT)
                // we only need to check the Landscape
                if ((o & (1 << 2)) != 0)
                {
                    isLandscape = true;
                }
                // check the forth bit (LANDSCAPE_RIGHT)
                if ((o & (1 << 3)) != 0)
                {
                    isLandscape = true;
                }

                // after checking the portrait and landscape modes, it's time to set
                // the page SupportedOrientations property (we do this on the UI thread)
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);
                    if (isPortrait && isLandscape)
                    {
                        currentPage.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
                    }
                    else if (isPortrait && !isLandscape)
                    {
                        currentPage.SupportedOrientations = SupportedPageOrientation.Portrait;
                    }
                    else if (!isPortrait && isLandscape)
                    {
                        currentPage.SupportedOrientations = SupportedPageOrientation.Landscape;
                    }
                });

                return MoSync.Constants.MA_SCREEN_ORIENTATION_RES_OK;
            };

            /**
		     * Set the screen orientation.
		     * Currently implemented only on Android.
		     * @param orientation One of the \link #SCREEN_ORIENTATION_LANDSCAPE
		     * #SCREEN_ORIENTATION_PORTRAIT #SCREEN_ORIENTATION_DYNAMIC \endlink
		     * constants.
		     * @return \< 0 on error.
		     */
            ioctls.maScreenSetOrientation = delegate(int orientation)
            {
                // there are only three options: SCREEN_ORIENTATION_LANDSCAPE(1), SCREEN_ORIENTATION_PORTRAIT(2),
                // SCREEN_ORIENTATION_DYNAMIC(3)
                if (orientation < 1 || orientation > 3)
                {
                    // error - not a valid input
                    return -1;
                }

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    // the orientation will be set on the current page
                    PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);

                    switch (orientation)
                    {
                        case MoSync.Constants.SCREEN_ORIENTATION_PORTRAIT:
                            currentPage.SupportedOrientations = SupportedPageOrientation.Portrait;
                            break;
                        case MoSync.Constants.SCREEN_ORIENTATION_LANDSCAPE:
                            currentPage.SupportedOrientations = SupportedPageOrientation.Landscape;
                            break;
                        default:
                            // we consider the default case as being MoSync.Constants.SCREEN_ORIENTATION_DYNAMIC
                            // based on the device sensor
                            currentPage.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
                            break;
                    }
                });

                return MoSync.Constants.MAW_RES_OK;
            };

            /**
		     * Get supported screen orientations.
		     * Currently implemented on iOS and WindowsPhone 7.1.
		     * @return A bitmask consisting of flags describing the supported screen orientations.
		     * The bitmask is created using \link #MA_SCREEN_ORIENTATION_ MA_SCREEN_ORIENTATION \endlink
		     * values.
		     */
            ioctls.maScreenGetSupportedOrientations = delegate()
            {
                int suportedOrientations = 0;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);
                    // based on the SupportedOrientations property, we create
                    // a bitmask containing the suported orientations. The bitmask will contain
                    // the orientations in the following order:
                    // PORTRAIT, PORTRAIT_UPSIDE_DOWN, LANDSCAPE_LEFT and LANDSCAPE_RIGHT
                    switch (currentPage.SupportedOrientations)
                    {
                        case SupportedPageOrientation.PortraitOrLandscape:
                            suportedOrientations = suportedOrientations |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
                            break;
                        case SupportedPageOrientation.Landscape:
                            suportedOrientations = suportedOrientations |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
                            break;
                        case SupportedPageOrientation.Portrait:
                            suportedOrientations = suportedOrientations |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT |
                                               MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
                            break;
                        default:
                            break;
                    }
                });

                return suportedOrientations;
            };

            /**
		     * Get current screen orientation.
		     * Currently implemented on iOS and WindowsPhone 7.1.
		     * @return One of the \link #MA_SCREEN_ORIENTATION_ MA_SCREEN_ORIENTATION \endlink constants.
		     */
            ioctls.maScreenGetCurrentOrientation = delegate()
            {
                int currentOrientation = 0;

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);
                    switch (currentPage.Orientation)
                    {
                        case PageOrientation.LandscapeRight:
                            currentOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
                            break;
                        case PageOrientation.LandscapeLeft:
                            currentOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
                            break;
                        case PageOrientation.PortraitUp:
                            currentOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT;
                            break;
                        case PageOrientation.PortraitDown:
                            currentOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
                            break;
                        default:
                            break;
                    }
                });

                return currentOrientation;
            };
        }
    }
}
