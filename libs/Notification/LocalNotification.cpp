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
 * @file LocalNotification.cpp
 * @author Emma Tresanszki and Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Local notifications are used for displaying alerts, playing sounds
 * and for badging application icon.
 * The operating system is responsible for delivering the notification at the
 * proper time; the application does not have to be running for this to happen.
 * When you create a local notification, you must specify when the system
 * should deliver the notification(see fire date property).
 * @platform Android, iOS.
 */

#include <maapi.h>
#include <mastdlib.h>

#include "LocalNotification.h"
#include "NotificationManager.h"
#include "LocalNotificationListener.h"

namespace Notification
{

    // Usual size for a buffer.
    const int BUF_SIZE = 256;

    /**
     * Constructor.
     */
    LocalNotification::LocalNotification():
        mHandle(0),
        mNotificationManager(NotificationManager::getInstance())
    {
        mHandle = maNotificationLocalCreate();
        mNotificationManager->addEventListener(this);
    }

    /**
     * Destructor.
     */
    LocalNotification::~LocalNotification()
    {
        mNotificationManager->removeEventListener(this);
        mLocalNotificationListeners.clear();
        maNotificationLocalDestroy(mHandle);
    }

    /**
     * @return The handle of the local notification.
     */
    MAHandle LocalNotification::getHandle() const
    {
        return mHandle;
    }

    /**
     * Set a local notification string property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE if the property value
     * was invalid.
     */
    int LocalNotification::setProperty(
        const MAUtil::String& property,
        const MAUtil::String& value)
    {
        int resultCode = maNotificationLocalSetProperty(
            mHandle,
            property.c_str(),
            value.c_str());
        return resultCode;
    }

    /**
     * Set a local notification integer property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE if the property value
     *  was invalid.
     */
    int LocalNotification::setPropertyInt(
        const MAUtil::String& property,
        const int value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%d", value);
        int resultCode = maNotificationLocalSetProperty(
            mHandle,
            property.c_str(),
            buffer);

        return resultCode;
    }

    /**
     * Get a local notification property value as an integer, setting also
     * the result code.
     * @param property A string representing which property to set.
     * @param resultCode Will contain the result code of the syscall.
     * Can be any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if no error occurred.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name was
     * invalid.
     * - #MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE if the buffer was
     * too small.
     * @return The property value.
     */
    int LocalNotification::getPropertyInt(
        const MAUtil::String& property,
        int& resultCode) const
    {
        char buffer[BUF_SIZE];
        resultCode = maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if (0 <= resultCode)
        {
            resultCode = MA_NOTIFICATION_RES_OK;
        }
        return atoi(buffer);
    }

    /**
     * Get a local notification property value as an integer.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    int LocalNotification::getPropertyInt(
        const MAUtil::String& property) const
    {
        char buffer[BUF_SIZE];
        maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        return atoi(buffer);
    }

    /**
     * Get a local notification property as a string, setting also the
     * result code.
     * @param property A string representing which property to set.
     * @param resultCode Will contain the result code of the syscall.
     *                   Can be any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if no error occurred.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name was
     * invalid.
     * - #MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE if the buffer was
     * too small.
     * @return The property value.
     */
    MAUtil::String LocalNotification::getPropertyString(
        const MAUtil::String& property,
        int& resultCode) const
    {
        char buffer[BUF_SIZE];
        resultCode = maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if (0 >= resultCode)
        {
            resultCode = MA_NOTIFICATION_RES_OK;
        }

        return buffer;
    }

    /**
     * Get a local notification property as a string.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    MAUtil::String LocalNotification::getPropertyString(
        const MAUtil::String& property) const
    {
        int resultCode;
        return getPropertyString(property,resultCode);
    }

    /**
     * Set the number displayed on the application's icon badge.
     * Platform: iOS.
     * @param badgeNumber The number that will be displayed on the
     * application's icon badge. Must be a positive value.
     */
    void LocalNotification::setBadgeNumber(const int badgeNumber)
    {
        this->setPropertyInt(MA_NOTIFICATION_LOCAL_BADGE_NUMBER, badgeNumber);
    }

    /**
     * Get the number displayed on the application's icon badge.
     * Platform: iOS.
     * @return the number displayed on the application's icon badge.
     */
    int LocalNotification::getBadgeNumber() const
    {
        return getPropertyInt(MA_NOTIFICATION_LOCAL_BADGE_NUMBER);
    }

    /**
     * Set the message displayed in the notification alert.
     * @param text The given text that will be displayed in notification.
     */
    void LocalNotification::setContentBody(MAUtil::String& text)
    {
        this->setProperty(MA_NOTIFICATION_LOCAL_CONTENT_BODY, text);
    }

    /**
     * Get the text displayed in the notification alert.
     * @return The text displayed in the notification alert.
     */
    MAUtil::String LocalNotification::getContentBody() const
    {
        return this->getPropertyString(MA_NOTIFICATION_LOCAL_CONTENT_BODY);
    }

    /**
     * Set the title of the action button or slider.
     * Platform: iOS.
     * @param alertAction The given title.
     */
    void LocalNotification::setAlertAction(MAUtil::String& alertAction)
    {
        this->setProperty(MA_NOTIFICATION_LOCAL_ALERT_ACTION, alertAction);
    }

    /**
     * Get the title of the action button or slider.
     * Platform: iOS.
     * @return The title of the action button or slider.
     */
    MAUtil::String LocalNotification::getAlertAction() const
    {
        return this->getPropertyString(MA_NOTIFICATION_LOCAL_ALERT_ACTION);
    }

    /**
     * Enable/disable the sound played when an alert is displayed.
     * Platform: iOS.
     * @param playSound If true the notification will play a sound when
     * it's shown.
     */
    void LocalNotification::setPlaySound(bool playSound)
    {
        MAUtil::String value;
        if (playSound)
        {
            value = "true";
        }
        else
        {
            value = "false";
        }
        this->setProperty(MA_NOTIFICATION_LOCAL_PLAY_SOUND, value);
    }

    /**
     * Check if the local notification is playing sound.
     * @return True if the local notification is playing sound when it's
     * shown, false otherwise.
     */
    bool LocalNotification::isPlayingSound() const
    {
        MAUtil::String value =
            this->getPropertyString(MA_NOTIFICATION_LOCAL_PLAY_SOUND);
        if (0 == strcmp(value.c_str(), "true"))
        {
            return true;
        }
        return false;
    }

    /**
     * Set the date and time when the system should deliver the notification.
     * @param tm A date and time struct that specifies when the system
     * should deliver the notification.
     */
    void LocalNotification::setFireDate(struct tm* time)
    {
        int dateMillisec = mktime(time);
        this->setPropertyInt(MA_NOTIFICATION_LOCAL_FIRE_DATE, dateMillisec);
    }

    /**
     * Get the date and time when the system will deliver the notification.
     * @return A date and time struct that specifies when the system will
     * deliver the notification.
     */
    struct tm LocalNotification::getFireDate() const
    {
        int minDateMilliseconds =
            this->getPropertyInt(MA_NOTIFICATION_LOCAL_FIRE_DATE);
         tm timeStruct;
         split_time(minDateMilliseconds, &timeStruct);
         return timeStruct;
    }

    /**
     * Add an event listener for this banner.
     * @param listener The listener that will receive
     * local notification events.
     */
    void LocalNotification::addNotificationListener(
        LocalNotificationListener* listener)
    {
        for (int i = 0; i < mLocalNotificationListeners.size(); i++)
        {
            if (listener == mLocalNotificationListeners[i])
            {
                return;
            }
        }

        mLocalNotificationListeners.add(listener);
    }

    /**
     * Remove the event listener for this local notification.
     * @param listener The listener that receives local notification events.
     */
    void LocalNotification::removeNotificationListener(
        LocalNotificationListener* listener)
    {
        for (int i = 0; i < mLocalNotificationListeners.size(); i++)
        {
            if (listener == mLocalNotificationListeners[i])
            {
                mLocalNotificationListeners.remove(i);
                break;
            }
        }
    }

    /**
     * This method is called when there is an event for this local
     * notification.
     * @param eventData The event data for this local notification.
     */
    void LocalNotification::handleLocalNotificationEvent(
        const MAEvent& eventData)
    {
        for (int i = 0; i < mLocalNotificationListeners.size(); i++)
        {
            mLocalNotificationListeners[i]->didReceiveLocalNotification(this);
        }
    }

} // namespace Notification
