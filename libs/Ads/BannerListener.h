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

/*! \addtogroup AdvertisingLib
 *  @{
 */

/**
 *  @defgroup AdvertisingLib Advertising Library
 *  @{
 */

/**
 * @file BannerListener.h
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Listener for banner events.
 */

#ifndef ADS_BANNER_LISTENER_H_
#define ADS_BANNER_LISTENER_H_

/**
 * \brief MoSync Advertising API.
 */
namespace Ads
{
    // Forward declaration.
    class Banner;

    /**
     * @brief Listener for banner events.
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

/*! @} */
