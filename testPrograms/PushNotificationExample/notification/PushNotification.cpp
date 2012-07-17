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
 * @file PushNotification.cpp
 * @author Bogdan Iusco
 *
 * \brief Push notifications are used for displaying alerts, playing sounds
 * and for badging application icon.
 * A push notification is send from a server and received by an application.
 * An application can register for receiving push notifications using
 * registerPushNotification function(NotificationManager class).
 */

#define DEFAULT_APP_ICON_BADGE_NUMBER -1

#include "PushNotification.h"

namespace Notification
{

	/**
	 * Constructor.
	 */
	PushNotification::PushNotification() :
		mAlertMessage(NULL),
		mSoundFileName(NULL),
		mApplicationIconBadgeNumber(DEFAULT_APP_ICON_BADGE_NUMBER)
	{

	}

	/**
	 * Destructor.
	 */
	PushNotification::~PushNotification()
	{
		if (mAlertMessage)
		{
			delete mAlertMessage;
		}

		if (mSoundFileName)
		{
			delete mSoundFileName;
		}
	}

	/**
	 * Check if the push notification contains alert message.
	 * Platform: iOS only.
	 * @return True if the push notification contains alert message,
	 * false otherwise.
	 */
	bool PushNotification::containsAlertMessage() const
	{
		if (mAlertMessage)
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
	 * Platform: iOS only.
	 * Check if the push notification contains an alert message by calling
	 * containsAlertMessage() function.
	 * @return The message displayed in the notification alert.
	 */
	MAUtil::String PushNotification::getAlertMessage() const
	{
		if (mAlertMessage)
		{
			return *mAlertMessage;
		}
		else
		{
			return "";
		}
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
	 */
	MAUtil::String PushNotification::getSoundFileName() const
	{
		if (mSoundFileName)
		{
			return *mSoundFileName;
		}
		else
		{
			return "";
		}
	}

	/**
	 * Get the the application icon badge number.
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
	void PushNotification::setAlertMessage(const MAUtil::String& alertMessage)
	{
		mAlertMessage = new MAUtil::String(alertMessage);
	}

	/**
	 * Set the sound file name.
	 * @param soundFileName The sound file name for the current push
	 * notification.
	 */
	void PushNotification::setSoundFileName(const MAUtil::String& soundFileName)
	{
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
