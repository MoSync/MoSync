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

	/**
	 * Constants indicating the flags
	 * Platform: Android only.
	 */
	enum NotificationFlags
	{
		// Indicates that the audio will be repeated until the notification is
		// canceled or the notification window is opened.
		NOTIFICATION_FLAG_INSISTENT = 4,
		// Indicates that the notification should not be canceled when the user clicks the
		// Clear all button.
		NOTIFICATION_FLAG_NO_CLEAR = 32,
		// Indicates that this notification represents a high-priority event that may be
		// shown to the user even if notifications are otherwise unavailable (that is, when
		// the status bar is hidden).
		NOTIFICATION_FLAG_HIGH_PRIORITY = 128,
		// Indicates that the notification should be canceled when it is clicked by the user.
		NOTIFICATION_FLAG_AUTO_CANCEL = 16
	};

	/**
	 * Flashing LED lights.
	 * Define color and pattern.
	 * Platform: Android only.
	 */
	struct NotificationFlashLights{
		int ledARGB;
		// Length of time, in milliseconds, to keep the light on.
		int ledOnMS;
		// Length of time, in milliseconds, to keep the light off.
		int ledOffMS;
		NotificationFlashLights(int color, int on, int off):
			ledARGB(color), ledOnMS(on), ledOffMS(off){};
		NotificationFlashLights(){};
	};

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
        virtual void setContentBody(const MAUtil::String& text);

        /**
         * Get the text displayed in the notification alert.
         * @return The text displayed in the notification alert.
         */
        virtual MAUtil::String getContentBody() const;

        /**
         * Set the title that goes in the expanded entry of the notification.
         * Platform: Android.
         * @param text The given text that will be displayed in the notification.
         */
        virtual void setContentTitle(const MAUtil::String text);

        /**
         * Get the title that goes in the expanded entry of the notification.
         * Platform: Android.
         * @return The text displayed in the notification.
         */
        virtual MAUtil::String getContentTitle() const;

        /**
         * Set the text that flows by in the status bar when the
         * notification first activates.
         * Platform: Android.
         * @param text The text that flows by in the status bar.
         */
        virtual void setTickerText(const MAUtil::String text);

        /**
         * Get the text that flows by in the status bar when the
         * notification first activates.
         * Platform: Android.
         * @return The ticker text.
         */
        virtual MAUtil::String getTickerText() const;

        /**
         * Set the flags applied to the local notification.
         * Platform: Android.
         * @param flag One of the constants:
         *  - #NOTIFICATION_FLAG_INSISTENT
         *  - #NOTIFICATION_FLAG_NO_CLEAR
         *  - #NOTIFICATION_FLAG_HIGH_PRIORITY
         *  - #NOTIFICATION_FLAG_AUTO_CANCEL
         */
        virtual void setFlag(const int flag);

        /**
         * Set the title of the action button or slider.
         * Platform: iOS.
         * @param alertAction The given title.
         */
        virtual void setAlertAction(const MAUtil::String& alertAction);

        /**
         * Get the title of the action button or slider.
         * Platform: iOS.
         * @return The title of the action button or slider.
         */
        virtual MAUtil::String getAlertAction() const;

        /**
         * Enable/disable the sound played when an alert is displayed.
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
         * Set the sound to play when an alert is displayed.
         * Could be ignored if using setPlaySound(false).
         * Platform: Android.
         * @param path A valid path to an audio file.
         */
        virtual void setSound(const MAUtil::String path);

        /**
         * Enable/disable the the default vibration when an alert is displayed.
         * By default, the vibration is disabled.
         * If set to true, it will use the default notification vibrate. This will
         * ignore any given vibrate.
         * Using phone vibration requires the VIBRATE permission.
         * Platform: Android.
         * @param vibrate If true the user will be alerted with a vibration when
         * the local notification is shown.
         */
        virtual void setVibrate(bool vibrate);

        /**
         * Check if the local notification has vibrate enabled.
         * Platform: Android.
         * @return True if the user will be alerted with a vibration,
         * false otherwise.
         */
        virtual bool isVibrateEnabled() const;

        /**
         * Set the vibration duration when an alert is displayed.
         * This setting is ignored if setVibrate is disabled.
         * Using phone vibration requires the VIBRATE permission.
         * Platform: Android.
         * @param duration The number of milliseconds to vibrate.
         */
        virtual void setVibrateDuration(const int duration);

        /**
         * Enable/Disable the default notification LED lights.
         * This will ignore the setFlashLightsPattern().
         * Not every color in the spectrum is supported by the device LEDs, and not
         * every device supports the same  colors, so the hardware estimates to the
         * best of its ability. Green is the most common notification color.
         * Platform: Android.
         * @param flahsing If set to true the user will be alerted by the default
         * light pattern.
         */
        virtual void setFlashLights(bool flashing);

        /**
         * Define your own color and pattern for the lights.
         * Not every color in the spectrum is supported by the device LEDs, and not every
         * device supports the same  colors, so the hardware estimates to the best of its
         * ability. Green is the most common notification color.
         * Also, not all Android devices support this feature.
         * Platform: Android.
         * @param lightPattern a NotificationFlashLights struct.
         */
        virtual void setFlashLightsPattern(const NotificationFlashLights lightPattern);

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

    private:
        /**
         * Handle that identifies the local notification.
         */
        MAHandle mHandle;
    };
} // namespace Notification

#endif /* NOTIFICATION_LOCAL_NOTIFICATION_H_ */
