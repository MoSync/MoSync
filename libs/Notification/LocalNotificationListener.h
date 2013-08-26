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
