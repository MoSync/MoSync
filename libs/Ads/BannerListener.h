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
 * @file BannerListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for banner events.
 */

#ifndef ADS_BANNER_LISTENER_H_
#define ADS_BANNER_LISTENER_H_

namespace Ads
{
    // Forward declaration.
    class Banner;

    /**
     * \brief Listener for banner events.
     */
    class BannerListener
    {
    public:
        /**
         * Called when a banner view fails to load a new advertisement.
         * @param banner The banner view that failed to load an advertisement.
         * @param error Describes the problem.
         */
        virtual void bannerFailedLoad(
            Banner* banner,
            int error) {};

        /**
         * Called when a new banner advertisement is loaded.
         * @param banner The banner that failed to load an advertisement.
         */
        virtual void bannerLoaded(Banner* banner) {};

        /**
         * Called when the user taps the banner view.
         * The application is moved to background.
		 * @param banner The banner that the user tapped.
         */
        virtual void bannerOnLeaveApplication(Banner* banner) {};

        /**
         * Called after a banner view finishes executing an action that
		 * covered your application's user interface.
		 * @param banner The banner view that finished executing an action.
         */
        virtual void bannerOnDismiss(Banner* banner) {};
    };

} // namespace Ads

#endif /* ADS_BANNER_LISTENER_H_ */
