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
 * @file PushNotification.h
 * @author Emma Tresanszki and Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Push notifications are used for displaying alerts, playing sounds
 * and for badging application icon.
 *
 * A push notification is send from a server and received by an application.
 * An application can register for receiving push notifications using
 * registerPushNotification function(NotificationManager class).
 * @platform Android, iOS.
 */

#ifndef NOTIFICATION_PUSH_NOTIFICATION_H_
#define NOTIFICATION_PUSH_NOTIFICATION_H_

#include <MAUtil/String.h>

/**
* \brief MoSync Notification API classes.
*/
namespace Notification
{
    // Forward declaration
    class NotificationManager;

    /**
     * @brief Push notifications are used for displaying alerts, playing sounds
     * and for badging application icon.
     * A push notification is send from a server and received by an application.
     * An application can register for receiving push notifications using
     * registerPushNotification function(NotificationManager class).
     * On iOS platform a push notification can contain a badge number for the
     * application icon, an alert sound and an alert message to display to the
     * user. Check what contains a notification by calling containsMessage(),
     * containsSoundFileName() and containsIconBadgeNumber() functions.
     * On Android platform a push notification can contain a message, a title
     * and a ticker text: the text that flows by in the status bar when the
     * notification first activates.
     */
    class PushNotification
    {
    public:
        /**
         * Constructor.
         */
        PushNotification();

        /**
         * Destructor.
         */
        virtual ~PushNotification();

        /**
         * Check if the push notification contains alert message.
         * @return True if the push notification contains alert message,
         * false otherwise.
         */
        bool containsMessage() const;

        /**
         * Get the alert message.
         * Check if the push notification contains an alert message by calling
         * containsMessage() function.
         * @return The message displayed in the notification alert.
         * Can be NULL.
         */
        MAUtil::String* getMessage() const;

        /**
         * Check if the push notification contains sound file name.
         * Platform: iOS  only.
         * @return True if the push notification contains sound file name,
         * false otherwise.
         */
        bool containsSoundFileName() const;

        /**
         * Get the sound file name.
         * Platform: iOS  only.
         * Check if the push notification contains an sound file by calling
         * containsSoundFileName() function.
         * @return The name of the file containing the sound to play when an
         * alert is displayed.
         * Can be NULL.
         */
        MAUtil::String* getSoundFileName() const;

        /**
         * Check if the push notification contains icon badge number.
         * Platform: iOS only.
         * @return True if the push notification contains icon badge number,
         * false otherwise.
         */
        bool containsIconBadgeNumber() const;

        /**
         * Get the application icon badge number.
         * Platform: iOS only.
         * Check if the push notification contains an icon badge number by
         * calling containsIconBadgeNumber() function.
         * @return The number to display as the application’s icon badge.
         */
        int getIconBadgeNumebr() const;

    protected:
        /**
         * Set the alert message.
         * @param alertMessage The message displayed in the notification alert.
         */
        void setMessage(const MAUtil::String& alertMessage);

        /**
         * Set the sound file name.
         * @param soundFileName The sound file name for the current push
         * notification.
         */
        void setSoundFileName(const MAUtil::String& soundFileName);

        /**
         * Set the application icon badge number.
         * @param iconBadgeNumber The application icon badge number for the
         * current push notification.
         */
        void setIconBadge(const int iconBadgeNumber);

    private:
        /**
         * The message displayed in the notification.
         */
        MAUtil::String* mMessage;

        /**
         * The name of the file containing the sound to play when an alert is
         * displayed.
         * Platform: iOS only.
         */
        MAUtil::String* mSoundFileName;

        /**
         * The number to display as the application’s icon badge.
         * Platform: iOS only.
         */
        int mApplicationIconBadgeNumber;

        // Let the NotificationManager set values for a push notification.
        friend class NotificationManager;
    };
}

#endif /* NOTIFICATION_PUSH_NOTIFICATION_H_ */

/*! @} */
