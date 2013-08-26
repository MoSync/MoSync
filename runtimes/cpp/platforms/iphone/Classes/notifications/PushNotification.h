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
