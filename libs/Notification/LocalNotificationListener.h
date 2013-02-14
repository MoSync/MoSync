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

/*! \addtogroup NotificationLib
 *  @{
 */

/**
 *  @defgroup NotificationLib Notification Library
 *  @{
 */

/**
 * @file LocalNotificationListener.h
 * @author Emma Tresanszki and Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Listener for local notification events.
 * @platform Android, iOS.
 */

#ifndef NOTIFICATION_LOCAL_NOTIFICATION_LISTENER_H_
#define NOTIFICATION_LOCAL_NOTIFICATION_LISTENER_H_

/**
* \brief MoSync Notification API classes.
*/
namespace Notification
{
    // Forward declaration.
    class LocalNotification;

    /**
     * \brief Listener for local notification events.
     */
    class LocalNotificationListener
    {
    public:
        /**
         * Called when the application receives a local notification.
         * Note: this event will be received only if the application
         * was not stopped in the interval from when the notification
         * is scheduled until it's actually triggered.
         * @param localNotification The received local notification.
         */
        virtual void didReceiveLocalNotification(
            LocalNotification& localNotification) = 0;
    };

} // namespace Notification

#endif /* NOTIFICATION_LOCAL_NOTIFICATION_LISTENER_H_ */

/*! @} */
