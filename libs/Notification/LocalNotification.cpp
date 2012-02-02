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
 * @file LocalNotification.cpp
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

#include <maapi.h>
#include <mastdlib.h>
#include <conprint.h>
#include <matime.h>
#include <MAUtil/util.h>

#include "LocalNotification.h"

namespace Notification
{

    // Usual size for a buffer.
    const int BUF_SIZE = 256;

    /**
     * Constructor.
     */
    LocalNotification::LocalNotification():
        mHandle(0)
    {
        mHandle = maNotificationLocalCreate();
    }

    /**
     * Destructor.
     */
    LocalNotification::~LocalNotification()
    {
        maNotificationLocalDestroy(mHandle);
    }

    /**
     * @return The handle of the local notification.
     */
    MAHandle LocalNotification::getHandle() const
    {
        return mHandle;
    }

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
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     */
    int LocalNotification::setProperty(
        const MAUtil::String& property,
        const MAUtil::String& value)
    {
        int resultCode = maNotificationLocalSetProperty(
            mHandle,
            property.c_str(),
            value.c_str());
        return resultCode;
    }

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
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     */
    int LocalNotification::setPropertyInt(
        const MAUtil::String& property,
        const int value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%d", value);
        int resultCode = maNotificationLocalSetProperty(
            mHandle,
            property.c_str(),
            buffer);

        return resultCode;
    }

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
    int LocalNotification::getPropertyInt(
        const MAUtil::String& property,
        int& resultCode) const
    {
        char buffer[BUF_SIZE];
        resultCode = maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if (0 <= resultCode)
        {
            resultCode = MA_NOTIFICATION_RES_OK;
        }
        return atoi(buffer);
    }

    /**
     * Get a local notification property value as an integer.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    int LocalNotification::getPropertyInt(
        const MAUtil::String& property) const
    {
        char buffer[BUF_SIZE];
        maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        return atoi(buffer);
    }

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
    MAUtil::String LocalNotification::getPropertyString(
        const MAUtil::String& property,
        int& resultCode) const
    {
        char buffer[BUF_SIZE];
        resultCode = maNotificationLocalGetProperty(
            mHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if (0 >= resultCode)
        {
            resultCode = MA_NOTIFICATION_RES_OK;
        }

        return buffer;
    }

    /**
     * Get a local notification property as a string.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    MAUtil::String LocalNotification::getPropertyString(
        const MAUtil::String& property) const
    {
        int resultCode;
        return getPropertyString(property,resultCode);
    }

    /**
     * Set the number displayed on the application's icon badge.
     * Platform: iOS.
     * @param badgeNumber The number that will be displayed on the
     * application's icon badge. Must be a positive value.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     */
    int LocalNotification::setBadgeNumber(const int badgeNumber)
    {
        return this->setPropertyInt(MA_NOTIFICATION_LOCAL_BADGE_NUMBER, badgeNumber);
    }

    /**
     * Get the number displayed on the application's icon badge.
     * Platform: iOS.
     * @return the number displayed on the application's icon badge.
     */
    int LocalNotification::getBadgeNumber() const
    {
        return getPropertyInt(MA_NOTIFICATION_LOCAL_BADGE_NUMBER);
    }

    /**
     * Set the message displayed in the notification alert.
     * @param text The given text that will be displayed in notification.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     */
    int LocalNotification::setContentBody(const MAUtil::String& text)
    {
        return this->setProperty(MA_NOTIFICATION_LOCAL_CONTENT_BODY, text);
    }

    /**
     * Get the text displayed in the notification alert.
     * @return The text displayed in the notification alert.
     */
    MAUtil::String LocalNotification::getContentBody() const
    {
        return this->getPropertyString(MA_NOTIFICATION_LOCAL_CONTENT_BODY);
    }

    /**
     * Set the title that goes in the expanded entry of the notification.
     * Platform: Android.
     * @param text The given text that will be displayed in the notification.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setContentTitle(const MAUtil::String text)
    {
		return this->setProperty(MA_NOTIFICATION_LOCAL_CONTENT_TITLE, text);
    }

    /**
     * Get the title that goes in the expanded entry of the notification.
     * Platform: Android.
     * @return The text displayed in the notification.
     */
    MAUtil::String LocalNotification::getContentTitle() const
    {
		return this->getPropertyString(MA_NOTIFICATION_LOCAL_CONTENT_TITLE);
    }

    /**
     * Set the title of the action button or slider.
     * Platform: iOS.
     * @param alertAction The given title.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setAlertAction(const MAUtil::String& alertAction)
    {
        return this->setProperty(MA_NOTIFICATION_LOCAL_ALERT_ACTION, alertAction);
    }

    /**
     * Get the title of the action button or slider.
     * Platform: iOS.
     * @return The title of the action button or slider.
     */
    MAUtil::String LocalNotification::getAlertAction() const
    {
        return this->getPropertyString(MA_NOTIFICATION_LOCAL_ALERT_ACTION);
    }

    /**
     * Set the text that flows by in the status bar when the
     * notification first activates.
     * Platform: Android.
     * @param text The text that flows by in the status bar.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setTickerText(const MAUtil::String text)
    {
		return this->setProperty(MA_NOTIFICATION_LOCAL_TICKER_TEXT, text);
    }

    /**
     * Get the text that flows by in the status bar when the
     * notification first activates.
     * Platform: Android.
     * @return The ticker text.
     */
    MAUtil::String LocalNotification::getTickerText() const
    {
		return this->getPropertyString(MA_NOTIFICATION_LOCAL_TICKER_TEXT);
    }

    /**
     * Set the flags applied to the local notification.
     * Platform: Android.
     * @param flag One of the constants:
     *  - #NOTIFICATION_FLAG_INSISTENT
     *  - #NOTIFICATION_FLAG_NO_CLEAR
     *  - #NOTIFICATION_FLAG_HIGH_PRIORITY
     *  - #NOTIFICATION_FLAG_AUTO_CANCEL
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setFlag(const NotificationFlag flag)
    {
		return this->setPropertyInt(MA_NOTIFICATION_LOCAL_FLAG, flag);
    }

    /**
     * Set the display flags applied to the local notification.
     * Note that regardless of this setting, the didReceiveLocalNotification
     * callback will be made for each incoming notification.
     * #NOTIFICATION_DISPLAY_ONLY_IF_IN_BACKGROUND is enabled by default.
     * Platform: Android.
     * @param displayFlag  is the required state of the application for
     * a notification to be displayed. One of the constants:
     *  - #NOTIFICATION_DISPLAY_ONLY_IF_IN_BACKGROUND
     *  - #NOTIFICATION_DISPLAY_ANYTIME.
     *  @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setDisplayFlag(const NotificationDisplayFlag displayFlag)
    {
		return this->setPropertyInt(MA_NOTIFICATION_LOCAL_DISPLAY_FLAG, displayFlag);
    }

    /**
     * Enable/disable the sound played when an alert is displayed.
     * @param playSound If true the notification will play a sound when
     * it's shown.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setPlaySound(bool playSound)
    {
        MAUtil::String value = playSound ? "true" : "false";
        return this->setProperty(MA_NOTIFICATION_LOCAL_PLAY_SOUND, value);
    }

    /**
     * Check if the local notification is playing sound.
     * @return True if the local notification is playing sound when it's
     * shown, false otherwise.
     */
    bool LocalNotification::isPlayingSound() const
    {
        MAUtil::String value =
            this->getPropertyString(MA_NOTIFICATION_LOCAL_PLAY_SOUND);

        return (0 == strcmp(value.c_str(), "true")) ? true : false;
    }

    /**
     * Set the sound to play when an alert is displayed.
     * Could be ignored if using setPlaySound(false).
     * The sound file should be saved in a notifications folder under sdcard storage.
     * Note that some devices can only play notification sounds stored in the
     * internal storage, in the \system\media\audio\notifications folder.
     * Platform: Android.
     * @param path A valid path to an audio file.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setSound(const MAUtil::String path)
    {
        return this->setProperty(MA_NOTIFICATION_LOCAL_SOUND_PATH, path);
    }

    /**
     * Enable/disable the the default vibration when an alert is displayed.
     * By default, the vibration is disabled.
     * If set to true, it will use the default notification vibrate. This will
     * ignore any given vibrate.
     * Using phone vibration requires the VIBRATE permission.
     * Platform: Android.
     * @param vibrate If true the user will be alerted with a vibration when
     * the local notification is shown.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setVibrate(bool vibrate)
    {
        MAUtil::String value = vibrate ? "true" : "false";

        return this->setProperty(MA_NOTIFICATION_LOCAL_VIBRATE, value);
    }

    /**
     * Check if the local notification has vibrate enabled.
     * Platform: Android.
     * @return True if the user will be alerted with a vibration,
     * false otherwise.
     */
    bool LocalNotification::isVibrateEnabled() const
    {
        MAUtil::String value =
            this->getPropertyString(MA_NOTIFICATION_LOCAL_VIBRATE);

        return (0 == strcmp(value.c_str(), "true")) ? true : false;
    }

    /**
     * Set the vibration duration when an alert is displayed.
     * This setting is ignored if setVibrate is disabled.
     * Using phone vibration requires the VIBRATE permission.
     * Platform: Android.
     * @param duration The number of seconds to vibrate.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setVibrateDuration(const int duration)
    {
		return this->setPropertyInt(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, duration * 1000);
    }

    /**
     * Enable/Disable the default notification LED lights.
     * This will ignore the setFlashLightsPattern().
     * Not every color in the spectrum is supported by the device LEDs, and not
     * every device supports the same  colors, so the hardware estimates to the
     * best of its ability. Green is the most common notification color.
     * Platform: Android.
     * @param flashing If set to true the user will be alerted by the default
     * light pattern.
     * @return One of the following result codes:
     * - #MA_NOTIFICATION_RES_ERROR if the current device doesn't support flashing LED.
     * - #MA_NOTIFICATION_RES_OK.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setFlashLights(bool flashing)
    {

        MAUtil::String value = flashing ? "true" : "false";
        return this->setProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS, value);
    }

    /**
     * Define your own color and pattern for the lights.
     * Not every color in the spectrum is supported by the device LEDs, and not every
     * device supports the same  colors, so the hardware estimates to the best of its
     * ability. Green is the most common notification color.
     * Also, not all Android devices support this feature.
     * Platform: Android.
     * @param lightPattern a NotificationFlashLights struct.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     * - #MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name
     * was invalid for the target platform.
     */
    int LocalNotification::setFlashLightsPattern(
        const NotificationFlashLights lightPattern)
    {
		MAUtil::String pattern = "";
		pattern = MAUtil::integerToString(lightPattern.ledARGB) + ",";
		pattern += MAUtil::integerToString(lightPattern.ledOnMS * 1000) + ",";
		pattern += MAUtil::integerToString(lightPattern.ledOffMS * 1000);

        return this->setProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN, pattern);
    }

    /**
     * Set the date and time when the system should deliver the notification.
     * @param tm A date and time struct that specifies when the system
     * should deliver the notification.
     * @return Any of the following result codes:
     * - #MA_NOTIFICATION_RES_OK if the property could be set.
     * - #MA_NOTIFICATION_RES_ALREADY_SCHEDULED if the notification was
     *  scheduled, and it cannot be altered anymore.
     */
    int LocalNotification::setFireDate(struct tm* time)
    {
        int dateSec = mktime(time);
        return this->setPropertyInt(MA_NOTIFICATION_LOCAL_FIRE_DATE, dateSec);
    }

    /**
     * Get the date and time when the system will deliver the notification.
     * @return A date and time struct that specifies when the system will
     * deliver the notification.
     */
    struct tm LocalNotification::getFireDate() const
    {
        int minDateMilliseconds =
            this->getPropertyInt(MA_NOTIFICATION_LOCAL_FIRE_DATE);
         tm timeStruct;
         split_time(minDateMilliseconds, &timeStruct);
         return timeStruct;
    }

} // namespace Notification
