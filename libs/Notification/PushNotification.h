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
 * @file PushNotification.h
 * @author Emma Tresanszki and Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Push notifications are used for displaying alerts, playing sounds
 * and for badging application icon.
 * A push notification is send from a server and received by an application.
 * An application can register for receiving push notifications using
 * registerPushNotification function(NotificationManager class).
 * @platform Android, iOS.
 */

#ifndef NOTIFICATION_PUSH_NOTIFICATION_H_
#define NOTIFICATION_PUSH_NOTIFICATION_H_

#include <conprint.h>
#include <MAUtil/String.h>

namespace Notification
{

    // Forward declaration
    class NotificationManager;

    /**
     * Push notifications are used for displaying alerts, playing sounds
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
         * Platform: iOS only.
         * @return True if the push notification contains alert message,
         * false otherwise.
         */
        virtual bool containsMessage() const;

        /**
         * Get the alert message.
         * Check if the push notification contains an alert message by calling
         * containsMessage() function.
         * @return The message displayed in the notification alert.
         */
        virtual MAUtil::String getMessage() const;

        /**
         * Check if the push notification contains sound file name.
         * Platform: iOS  only.
         * @return True if the push notification contains sound file name,
         * false otherwise.
         */
        virtual bool containsSoundFileName() const;

        /**
         * Get the sound file name.
         * Platform: iOS  only.
         * Check if the push notification contains an sound file by calling
         * containsSoundFileName() function.
         * @return The name of the file containing the sound to play when an
         * alert is displayed.
         */
        virtual MAUtil::String getSoundFileName() const;

        /**
         * Check if the push notification contains icon badge number.
         * Platform: iOS only.
         * @return True if the push notification contains icon badge number,
         * false otherwise.
         */
        virtual bool containsIconBadgeNumber() const;

        /**
         * Get the application icon badge number.
         * Platform: iOS only.
         * Check if the push notification contains an icon badge number by
         * calling containsIconBadgeNumber() function.
         * @return The number to display as the application’s icon badge.
         */
        virtual int getIconBadgeNumebr() const;

    protected:
        /**
         * Set the alert message.
         * @param alertMessage The message displayed in the notification alert.
         */
        virtual void setMessage(const MAUtil::String& alertMessage);

        /**
         * Set the sound file name.
         * @param soundFileName The sound file name for the current push
         * notification.
         */
        virtual void setSoundFileName(const MAUtil::String& soundFileName);

        /**
         * Set the application icon badge number.
         * @param iconBadgeNumber The application icon badge number for the
         * current push notification.
         */
        virtual void setIconBadge(const int iconBadgeNumber);

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
