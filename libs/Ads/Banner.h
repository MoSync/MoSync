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
 * @file Banner.h
 * @author Bogdan Iusco
 *
 * \brief Provides a widget that displays advertisement to the user.
 * When the user taps a banner, it triggers an action programmed into
 * the advertisement.
 * Your application is notified when an action starts or stops.
 * The banner works with NativeUI library.
 * The banner can be added only to a NativeUI layout object.
 */

#ifndef ADS_BANNER_H_
#define ADS_BANNER_H_

#include <ma.h>
#include <conprint.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Environment.h>
#include <MAUtil/util.h>

namespace Ads
{

	// Forward declaration.
	class BannerManager;
	class BannerListener;

    /**
     * Size constants for a banner.
     */
    enum BannerSize
    {
        // Default banner of size 320 * 50.
        // Available on iOS and Android platforms.
        BANNER_SIZE_DEFAULT = 0,
        // Standard medium of size 300 * 250.
        // Available only on Android platform.
        BANNER_SIZE_RECT,
        // Full size banner of size 468 * 60.
        // Available only on Android platform.
        BANNER_SIZE_IAB,
        // Size 728 * 90.
        // Available only on Android platform.
        BANNER_SIZE_LEADERBOARD
    };

    /**
	 * \brief Provides a widget that displays advertisement to the user.
	 * When the user taps a banner, it triggers an action programmed into
	 * the advertisement.
	 * Your application is notified when an action starts or stops.
	 * The banner works with NativeUI library.
	 * The banner can be added only to a NativeUI layout object.
     */
    class Banner
    {
    public:
        /**
         * Constructor.
         * @param publisherID Publisher ID(only for Android platform).
         * @param bannerSize The size of the banner.
         */
        Banner(
            MAUtil::String publiserID = "",
            BannerSize bannerSize = BANNER_SIZE_DEFAULT);

        /**
         * Destructor.
         */
        virtual ~Banner();

        /**
         * @return The handle of the banner.
         */
        virtual MAHandle getHandle() const;

        /**
         * Set a banner string property.
         * @param property A string representing which property to set.
         * @param value The string value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MA_ADS_RES_OK if the property could be set.
         * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         */
        virtual int setProperty(
            const MAUtil::String& property,
            const MAUtil::String& value);

        /**
         * Set a banner integer property.
         * @param property A string representing which property to set.
         * @param value The string value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MA_ADS_RES_OK if the property could be set.
         * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         */
        virtual int setPropertyInt(
            const MAUtil::String& property,
            const int value);

        /**
         * Set a banner float property.
         * @param property A string representing which property to set.
         * @param value The string value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MA_ADS_RES_OK if the property could be set.
         * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         */
        virtual int setPropertyFloat(
            const MAUtil::String& property,
            float value);

        /**
         * Get a widget property value as an integer, setting also the result code.
         * @param property A string representing which property to set.
         * @param resultCode Will contain the result code of the syscall.
         * Can be any of the following result codes:
         * - #MA_ADS_RES_OK if the property could be set.
         * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MA_ADS_RES_INVALID_STRING_BUFFER_SIZE if the buffer was too small.
         * @return The property value.
         */
        virtual int getPropertyInt(
            const MAUtil::String& property,
            int& resultCode);

        /**
         * Get a widget property value as an integer.
         * @param property A string representing which property to set.
         * @return The property value.
         */
        virtual int getPropertyInt(
            const MAUtil::String& property);

        /**
         * Get a widget property as a string, setting also the result code.
         * @param property A string representing which property to set.
         * @param resultCode Will contain the result code of the syscall.
         *                   Can be any of the following result codes:
         * - #MA_ADS_RES_OK if the property could be set.
         * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MA_ADS_RES_INVALID_STRING_BUFFER_SIZE if the buffer was too small.
         * @return The property value.
         */
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property,
            int& resultCode);

        /**
         * Get a widget property as a string.
         * @param property A string representing which property to set.
         * @return The property value.
         */
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property);

        /**
         * Get the width of the banner.
         * @return The width value.
         */
        virtual int getWidth();

        /**
         * Get the height of the banner.
         * @return The height value.
         */
        virtual int getHeight();

        /**
         * Show or hide the banner.
         * @param visible If true shows the banner, otherwise hides it.
         */
        virtual void setVisible(const bool visible);

        /**
         * Check if the banner is visible.
         * @return True if is visible, false otherwise.
         */
        virtual bool isVisible();

        /**
         * Enable or disable the banner.
         * If a banner is disabled then the user cannot tap on it.
         * @param enable If true enables the banner, otherwise disables it.
         */
        virtual void setEnabled(bool enable);

        /**
         * Check if the banner is enabled.
         * @return True if enabled, false otherwise.
         */
        virtual bool isEnabled();

        /**
         * Add an event listener for this banner.
         * @param listener The listener that will receive
         * banner events for this banner.
         */
        virtual void addBannerListener(BannerListener* listener);

        /**
         * Remove the event listener for this banner.
         * @param listener The listener that receives banner events
         * for this banner.
         */
        virtual void removeBannerListener(BannerListener* listener);

        /**
         * This method is called when there is an event for this banner.
         * @param eventData The data for the banner event.
         */
        virtual void handleBannerEvent(const MAEvent& eventData);

    private:
        /**
         * Handle that identifies the banner.
         */
        MAHandle mHandle;

        /**
         * Pointer to a banner manager.
         */
        BannerManager* mBannerManager;

        /**
         * Array with banner listeners.
         */
        MAUtil::Vector<BannerListener*> mBannerEventListeners;
    };
} // namespace Ads

#endif /* ADS_BANNER_H_ */
