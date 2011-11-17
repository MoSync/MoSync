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
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Store the push notification JSON data received from APNS.
 */

/**
 * @brief Store the push notification JSON data received from APNS.
 */
@interface PushNotification : NSObject{
    /**
     * The message displayed in the push notification alert.
     * This object is nil if the JSON push notification data does not contain
     * an alert message.
     */
    NSString* mAlertMessage;

    /**
     * The name of the file containing the sound to play when an alert is displayed.
     * This object is nil if the JSON push notification data does not contain
     * an sound file name.
     */
    NSString* mSoundFileName;

    /**
     * The number to display as the application’s icon badge.
     * This object is nil if the JSON push notification data does not contain
     * a value for icon badge number.
     */
    NSNumber* mBadgeIcon;
}

/**
 * Init function.
 */
- (id)init;

@property(retain) NSString* alertMessage;
@property(retain) NSString* soundFileName;
@property(retain) NSNumber* badgeIcon;

@end
