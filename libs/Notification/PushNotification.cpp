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
 * @file PushNotification.cpp
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

#define DEFAULT_APP_ICON_BADGE_NUMBER -1

#include <conprint.h>

#include "PushNotification.h"

namespace Notification
{

    /**
     * Constructor.
     */
    PushNotification::PushNotification() :
        mMessage(NULL),
        mSoundFileName(NULL),
        mApplicationIconBadgeNumber(DEFAULT_APP_ICON_BADGE_NUMBER)
    {

    }

    /**
     * Destructor.
     */
    PushNotification::~PushNotification()
    {
        delete mMessage;
        mMessage = NULL;

        delete mSoundFileName;
        mSoundFileName = NULL;
    }

    /**
     * Check if the push notification contains alert message.
     * Platform: iOS only.
     * @return True if the push notification contains alert message,
     * false otherwise.
     */
    bool PushNotification::containsMessage() const
    {
        if (mMessage)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * Get the alert message.
     * Check if the push notification contains an alert message by calling
     * containsAlertMessage() function.
     * @return The message displayed in the notification alert.
     * Can be NULL.
     */
    MAUtil::String* PushNotification::getMessage() const
    {
        return mMessage;
    }

    /**
     * Check if the push notification contains sound file name.
     * Platform: iOS  only.
     * @return True if the push notification contains sound file name,
     * false otherwise.
     */
    bool PushNotification::containsSoundFileName() const
    {
        if (mSoundFileName)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * Check if the push notification contains icon badge number.
     * Platform: iOS only.
     * @return True if the push notification contains icon badge number,
     * false otherwise.
     */
    bool PushNotification::containsIconBadgeNumber() const
    {
        return mApplicationIconBadgeNumber == DEFAULT_APP_ICON_BADGE_NUMBER ?
                false:true;
    }

    /**
     * Get the sound file name.
     * Platform: iOS  only.
     * Check if the push notification contains an sound file by calling
     * containsSoundFileName() function.
     * @return The name of the file containing the sound to play when an
     * alert is displayed.
     * Can be NULL.
     */
    MAUtil::String* PushNotification::getSoundFileName() const
    {
        return mSoundFileName;
    }

    /**
     * Get the application icon badge number.
     * Platform: iOS only.
     * Check if the push notification contains an icon badge number by
     * calling containsIconBadgeNumber() function.
     * @return The number to display as the application’s icon badge.
     */
    int PushNotification::getIconBadgeNumebr() const
    {
        return mApplicationIconBadgeNumber;
    }

    /**
     * Set the alert message.
     * @param alertMessage The message displayed in the notification alert.
     */
    void PushNotification::setMessage(const MAUtil::String& alertMessage)
    {
        delete mMessage;
        mMessage = new MAUtil::String(alertMessage);
    }

    /**
     * Set the sound file name.
     * @param soundFileName The sound file name for the current push
     * notification.
     */
    void PushNotification::setSoundFileName(const MAUtil::String& soundFileName)
    {
        delete mSoundFileName;
        mSoundFileName = new MAUtil::String(soundFileName);
    }

    /**
     * Set the application icon badge number.
     * @param iconBadgeNumber The application icon badge number for the
     * current push notification.
     */
    void PushNotification::setIconBadge(const int iconBadgeNumber)
    {
        mApplicationIconBadgeNumber = iconBadgeNumber;
    }

} // end of Notification namespace
