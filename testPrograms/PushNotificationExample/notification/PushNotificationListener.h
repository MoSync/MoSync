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

/**
 * @file PushNotificationListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for push notification events.
 */

#ifndef NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_
#define NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_

namespace Notification
{

    // Forward declaration
    class PushNotification;

    /**
     * \brief Listener for push notification events.
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
         * Called when application has been registered for push notification.
         */
        virtual void didApplicationRegistered(MAUtil::String& token) {};

        /**
         * Called if the application did not registered for push notification.
         */
        virtual void didFaildToRegister(
            const int code,
            MAUtil::String& error){};
    };

} // namespace Notification

#endif /* NOTIFICATION_PUSH_NOTIFICATION_LISTENER_H_ */
