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
 * @file LocalNotification.h
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

#ifndef NOTIFICATION_LOCAL_NOTIFICATION_H_
#define NOTIFICATION_LOCAL_NOTIFICATION_H_

#include <ma.h>
#include <conprint.h>
#include <matime.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Environment.h>
#include <MAUtil/util.h>

namespace Notification
{

    // Forward declaration.
    class NotificationManager;
    class LocalNotificationListener;

    /**
     * @brief Local notifications are used for displaying alerts, playing sounds
     * and for badging application icon.
     * The operating system is responsible for delivering the notification at
     * the proper time; the application does not have to be running for this to
     * happen.
     * When you create a local notification, you must specify when the system
     * should deliver the notification(see fire date property).
	 * If you don't specify a fire date, the default System time will be set.
     */
    class LocalNotification
    {
    public:
        /**
         * Constructor.
         */
        LocalNotification();

        /**
         * Destructor.
         */
        virtual ~LocalNotification();

        /**
         * @return The handle of the local notification.
         */
        virtual MAHandle getHandle() const;

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
        virtual int setProperty(
            const MAUtil::String& property,
            const MAUtil::String& value);

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
        virtual int setPropertyInt(
            const MAUtil::String& property,
            const int value);

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
        virtual int getPropertyInt(
            const MAUtil::String& property,
            int& resultCode) const;

        /**
         * Get a local notification property value as an integer.
         * @param property A string representing which property to get.
         * @return The property value.
         */
        virtual int getPropertyInt(
            const MAUtil::String& property) const;

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
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property,
            int& resultCode) const;

        /**
         * Get a local notification property as a string.
         * @param property A string representing which property to get.
         * @return The property value.
         */
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property) const;

        /**
         * Set the number displayed on the application's icon badge.
         * Platform: iOS.
         * @param badgeNumber The number that will be displayed on the
         * application's icon badge. Must be a positive value.
         */
        virtual void setBadgeNumber(const int badgeNumber);

        /**
         * Get the number displayed on the application's icon badge.
         * Platform: iOS.
         * @return the number displayed on the application's icon badge.
         */
        virtual int getBadgeNumber() const;

        /**
         * Set the message displayed in the notification alert.
         * @param text The given text that will be displayed in notification.
         */
        virtual void setContentBody(MAUtil::String& text);

        /**
         * Get the text displayed in the notification alert.
         * @return The text displayed in the notification alert.
         */
        virtual MAUtil::String getContentBody() const;

        /**
         * Set the title of the action button or slider.
         * Platform: iOS.
         * @param alertAction The given title.
         */
        virtual void setAlertAction(MAUtil::String& alertAction);

        /**
         * Get the title of the action button or slider.
         * Platform: iOS.
         * @return The title of the action button or slider.
         */
        virtual MAUtil::String getAlertAction() const;

        /**
         * Enable/disable the sound played when an alert is displayed.
         * Platform: iOS.
         * @param playSound If true the notification will play a sound when
         * it's shown.
         */
        virtual void setPlaySound(bool playSound);

        /**
         * Check if the local notification is playing sound.
         * @return True if the local notification is playing sound when it's
         * shown, false otherwise.
         */
        virtual bool isPlayingSound() const;

        /**
         * Set the date and time when the system should deliver the notification.
         * @param tm A date and time struct that specifies when the system
         * should deliver the notification.
         */
        virtual void setFireDate(struct tm* time);

        /**
         * Get the date and time when the system will deliver the notification.
         * @return A date and time struct that specifies when the system will
         * deliver the notification.
         */
        virtual struct tm getFireDate() const;

        /**
         * Add an event listener for this local notification.
         * @param listener The listener that will receive
         * local notification events.
         */
        virtual void addNotificationListener(
            LocalNotificationListener* listener);

        /**
         * Remove the event listener for this local notification.
         * @param listener The listener that receives local notification events.
         */
        virtual void removeNotificationListener(
            LocalNotificationListener* listener);

        /**
         * This method is called when there is an event for this local
         * notification.
         * @param eventData The event data for this local notification.
         */
        virtual void handleLocalNotificationEvent(const MAEvent& eventData);

    private:
        /**
         * Handle that identifies the local notification.
         */
        MAHandle mHandle;

        /**
         * Pointer to the notification manager.
         * Not own.
         */
        NotificationManager* mNotificationManager;

        /**
         * Array with local notification listeners.
         */
        MAUtil::Vector<LocalNotificationListener*> mLocalNotificationListeners;
    };
} // namespace Notification

#endif /* NOTIFICATION_LOCAL_NOTIFICATION_H_ */
