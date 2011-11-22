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
 * @file Banner.cpp
 * @author Bogdan Iusco
 *
 * \brief Provides a widget that displays advertisement to the user.
 * When the user taps a banner, it triggers an action programmed into
 * the advertisement.
 * Your application is notified when an action starts or stops.
 * The banner works with NativeUI library.
 * The banner can be added only to a NativeUI layout object.
 */

#include <maapi.h>
#include <mastdlib.h>

#include "Banner.h"
#include "BannerListener.h"
#include "BannerManager.h"

namespace Ads
{

	// Usual size for a buffer.
	const int BUF_SIZE = 256;

    /**
     * Constructor.
     * @param publisherID Publisher ID(only for Android platform).
     * @param bannerSize The size of the banner.
     */
    Banner::Banner(
        MAUtil::String publiserID,
        BannerSize bannerSize) :
			mBannerManager(BannerManager::getInstance())
	{
		mHandle = maAdsBannerCreate(bannerSize, publiserID.c_str());
		mBannerManager->registerBanner(this);
	}

    /**
     * Destructor.
     */
	Banner::~Banner()
    {
		mBannerManager->unregisterBanner(this);
        mBannerEventListeners.clear();

        if (NULL != mHandle)
        {
            maAdsBannerDestroy(mHandle);
            mHandle = NULL;
        }
    }

    /**
     * @return The handle of the banner.
     */
    MAHandle Banner::getHandle() const
    {
        return mHandle;
    }

    /**
     * Set a banner string property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MA_ADS_RES_OK if the property could be set.
     * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     */
    int Banner::setProperty(
        const MAUtil::String& property,
        const MAUtil::String& value)
    {
		int resultCode = maAdsBannerSetProperty(
            mHandle,
            property.c_str(),
            value.c_str());

		return resultCode;
    }

    /**
     * Set a banner integer property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MA_ADS_RES_OK if the property could be set.
     * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     */
    int Banner::setPropertyInt(
        const MAUtil::String& property,
        const int value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%d", value);
        int resultCode = maAdsBannerSetProperty(
            mHandle,
            property.c_str(),
            buffer);

        return resultCode;
    }

    /**
     * Set a banner float property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MA_ADS_RES_OK if the property could be set.
     * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     */
    int Banner::setPropertyFloat(
        const MAUtil::String& property,
        float value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%f", value);
        int resultCode = maAdsBannerSetProperty(
            mHandle,
            property.c_str(),
            buffer);

        return resultCode;
    }

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
    int Banner::getPropertyInt(
        const MAUtil::String& property,
        int& resultCode)
    {
        char buffer[BUF_SIZE];
        resultCode = maAdsBannerGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if ( resultCode >= 0 )
        {
            resultCode = MA_ADS_RES_OK;
        }
        return atoi(buffer);
    }

    /**
     * Get a widget property value as an integer.
     * @param property A string representing which property to set.
     * @return The property value.
     */
    int Banner::getPropertyInt(
        const MAUtil::String& property)
    {
        char buffer[BUF_SIZE];
        maAdsBannerGetProperty(
			mHandle,
			property.c_str(),
			buffer,
			BUF_SIZE);

        return atoi(buffer);
    }

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
    MAUtil::String Banner::getPropertyString(
        const MAUtil::String& property,
        int& resultCode)
    {
        char buffer[BUF_SIZE];
        resultCode = maAdsBannerGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if ( resultCode >= 0 )
        {
            resultCode = MA_ADS_RES_OK;
        }

        return buffer;
    }

    /**
     * Get a widget property as a string.
     * @param property A string representing which property to set.
     * @return The property value.
     */
    MAUtil::String Banner::getPropertyString(
        const MAUtil::String& property)
    {
        int resultCode;
        return getPropertyString(property,resultCode);
    }

    /**
     * Get the width of the banner.
     * @return The width value.
     */
    int Banner::getWidth()
    {
        return getPropertyInt(MA_ADS_WIDTH);
    }

    /**
     * Get the height of the banner.
     * @return The height value.
     */
    int Banner::getHeight()
    {
        return getPropertyInt(MA_ADS_HEIGHT);
    }

    /**
     * Show or hide the banner.
     * @param visible If true shows the banner, otherwise hides it.
     */
    void Banner::setVisible(const bool visible)
    {
        setProperty(MA_ADS_VISIBLE, (visible ? "true" : "false") );
    }

    /**
     * Check if the banner is visible.
     * @return True if is visible, false otherwise.
     */
    bool Banner::isVisible()
    {
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MA_ADS_VISIBLE));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
    }

    /**
     * Enable or disable the banner.
     * If a banner is disabled then the user cannot tap on it.
     * @param enable If true enables the banner, otherwise disables it.
     */
    void Banner::setEnabled(bool enable)
    {
        setProperty(MA_ADS_ENABLED, (enable ? "true" : "false") );
    }

    /**
     * Check if the banner is enabled.
     * @return True if enabled, false otherwise.
     */
    bool Banner::isEnabled()
    {
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MA_ADS_ENABLED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
    }

    /**
     * Add an event listener for this banner.
     * @param listener The listener that will receive
     * banner events for this banner.
     */
    void Banner::addBannerListener(BannerListener* listener)
    {
        for (int i = 0; i < mBannerEventListeners.size(); i++)
        {
            if (listener == mBannerEventListeners[i])
            {
                return;
            }
        }

        mBannerEventListeners.add(listener);
    }

    /**
     * Remove the event listener for this banner.
     * @param listener The listener that receives banner events
     * for this banner.
     */
    void Banner::removeBannerListener(BannerListener* listener)
    {
        for (int i = 0; i < mBannerEventListeners.size(); i++)
        {
            if (listener == mBannerEventListeners[i])
            {
                mBannerEventListeners.remove(i);
                break;
            }
        }
    }

    /**
     * This method is called when there is an event for this banner.
     * @param eventData The data for the banner event.
     */
    void Banner::handleBannerEvent(const MAEvent& eventData)
    {
        if (MA_ADS_EVENT_FAILED == eventData.adsData.bannerEventType)
        {
            for (int i = 0; i < mBannerEventListeners.size(); i++)
            {
                mBannerEventListeners[i]->bannerFailedLoad(
                    this,
                    eventData.adsData.bannerErrorCode);
            }
        }
        else if (MA_ADS_EVENT_LOADED == eventData.adsData.bannerEventType)
        {
            for (int i = 0; i < mBannerEventListeners.size(); i++)
            {
                mBannerEventListeners[i]->bannerLoaded(this);
            }
        }
        else if (MA_ADS_EVENT_ON_DISMISS == eventData.adsData.bannerEventType)
        {
            for (int i = 0; i < mBannerEventListeners.size(); i++)
            {
                mBannerEventListeners[i]->bannerOnDismiss(this);
            }
        }
        else if (MA_ADS_EVENT_ON_LEAVE_APPLICATION ==
                eventData.adsData.bannerEventType)
        {
            for (int i = 0; i < mBannerEventListeners.size(); i++)
            {
                mBannerEventListeners[i]->bannerOnLeaveApplication(this);
            }
        }
    }

} // namespace Ads
