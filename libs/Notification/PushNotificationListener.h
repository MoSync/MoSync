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
 * @file PushNotificationListener.h
 * @author Emma Tresanszki and Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Listener for push notification events.
 * @platform Android, iOS.
 */

#ifndef NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_
#define NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_

namespace Notification
{

    // Forward declaration
    class PushNotification;

    /**
     * @brief Listener for push notification events.
     */
    class PushNotificationListener
    {
    public:
        /**
         * Called when the application receives a push notification.
         * @param pushNotification The received push notification.
         */
        virtual void didReceivePushNotification(
            PushNotification& pushNotification) = 0;

        /**
         * Called when application has been registered for push notifications.
         */
        virtual void didApplicationRegistered(MAUtil::String& token) {};

        /**
         * Called when the application has been unregistered for push notifications.
         * Platform: Android only.
         */
        virtual void didApplicationUnregister() {};

        /**
         * Called if the application did not registered for push notifications.
         */
        virtual void didFaildToRegister(
            MAUtil::String& error){};
    };

} // namespace Notification

#endif /* NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_ */
