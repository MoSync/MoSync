/* Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import <Foundation/Foundation.h>

#import "helpers/cpp_defs.h"

/**
 * Design pattern: singleton.
 * Notifications are used for displaying alerts, playing sounds, and badging application icons.
 * UILocalNotification type objects are used for local notifications.
 * Instances of UILocalNotification represent notifications that an application can schedule
 * for presentation to its users at specific dates and times.
 * The operating system is responsible for delivering the notification at the proper time.
 * The application does not have to be running for this to happen.
 */
@interface Notification :  NSObject{
    /**
     * Stores pairs of:
     * - UILocalNotification type objects(the object).
     * - NSNumber type objects(the key).
     */
    NSMutableDictionary* mLocalNotificationDictionary;

    /**
     * Counts the number of created local notifications.
     */
    int mLocalNotificationHandleCount;

    /**
     * Stores pairs of:
     * - PushNotification type objects(the object).
     * - NSNumber type objects(the key).
     */
    NSMutableDictionary* mPushNotificationDictionary;

    /**
     * Counts the number of created local notifications.
     */
    int mPushNotificationHandleCount;

    /**
     * Set if the push notification are enabled.
     */
    bool mIsPushNotificationEnabled;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(Notification*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Creates a new local notification object.
 * @return:
 * - MA_NOTIFICATION_RES_UNSUPPORTED if the notifications are not supported on current system.
 * - MA_NOTIFICATION_RES_ERROR if a error occurred while creating the notification object.
 * - a handle to a notification object.
 */
-(MAHandle) createNotificationObject;

/**
 * Destroy a local notification object.
 * @param notificationHandle Handle to a notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) destroyNotificationObject:(MAHandle) notificationHandle;

/**
 * Set a local notification property.
 * @param notificationHandle Handle to a local notification object.
 * @param property A string representing which property to set.
 * @param value The value that will be assigned to the property.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 * - MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
 */
-(int) notificationSetProperty:(MAHandle) notificationHandle
                      property:(const char*) propertyName
                         value:(const char*) value;

/**
 * Retrieves a specified property from a given local notificaiton object.
 * @param notificationHandle Handle to a local notification object.
 * @param property A string representing for which property to get the value.
 * @param value A buffer that will hold the value of the property, represented as a string.
 * @param bufSize Size of the buffer.
 * @return One of the next result codes:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 * - MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
 */
-(int) notificationGetProperty:(MAHandle) notificationHandle
                      property:(const char*) property
                         value:(char*) value
                          size:(int) maxSize;

/**
 * Schedules a local notification for delivery at its encapsulated date and time.
 * @param notificationHandle Handle to a local notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) registerLocalNotification:(MAHandle) notificationHandle;

/**
 * Cancels the delivery of the specified scheduled local notification.
 * @param notificationHandle Handle to a local notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) unregisterLocalNotification:(MAHandle) notificationHandle;

/**
 * Call this method only when a local notification is received.
 * @param notification A local notification that encapsulates details about
 * the notification, potentially including custom data.
 * Sends MoSync event containing a handle to the local notification.
 */
- (void) didReceiveLocalNotification:(UILocalNotification *)notification;

/**
 * Get the handle assocciated with a given notification.
 * @param notification The given notification.
 * @return The handle or MA_NOTIFICATION_RES_INVALID_HANDLE if the given
 * notification is not registered to this class.
 */
-(MAHandle) getNotificationHandle:(UILocalNotification*) notification;

/**
 * Registers the current application for receiving push notifications.
 * @param pushNotificationType A bit mask specifying the types of notifications
 * the application accepts. See MA_NOTIFICATION_PUSH_TYPE for valid bit-mask values.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_ALREADY_REGISTERED if the application is already registered for
 * receiving push notifications.
 */
-(int) registerPushNotification:(MAHandle) pushNotificationType;

/**
 * Unregister application for push notifications.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_NOT_REGISTERED if the application was not registered for receiving
 * push notification.
 */
-(int) unregisterPushNotification;

/**
 * Fills pushNotificationData struct with the values from a given push notification.
 * @param pushNotificationHandle Handle to a given push notification.
 * @param pushNotificationData A struct that will contain the values for a given push notification.
 * @return One of the next contants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the pushNotificationHandle is invalid.
 * - MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE if at least one of the buffers from
 * pushNotificationData are too small.
 */
-(int) getPushNotificationData:(MAHandle) pushNotificationHandle
                          data:(MAPushNotificationData*) pushNotificationData;

/**
 * Set the number currently set as the badge of the application icon in Springboard.
 * @param applicationIconBadgeNumber Set to zero to hide the badge number. The default is zero.
 * If this value is negative the method will do nothing.
 */
-(void) setApplicationIconBadgeNumber:(const int) applicationIconBadgeNumber;

/**
 * Get the number currently set as the badge of the application icon in Springboard.
 * @return The number displayed as the badge of the application.
 */
-(int) getApplicationIconBadgeNumber;

/**
 * Called when a running application receives a push notification.
 * @param pushNotification A dictionary that contains information related to the push notification,
 * potentially including a badge number for the application icon, an alert sound, an alert message
 * to display to the user, a notification identifier, and custom data.
 * Sends mosync events containing push notification data.
 */
-(void) didReceivePushNotification:(NSDictionary*) pushNotification;

@end

/**
 * Called when the application receives a local notification.
 * @param localNotification A local notification that encapsulates details about the notification,
 * potentially including custom data.
 */
void MoSync_DidReceiveLocalNotification(UILocalNotification* localNotification);

/**
 * Called when a running application receives a push notification.
 * @param pushNotification A dictionary that contains information related to the push notification,
 * potentially including a badge number for the application icon, an alert sound, an alert message
 * to display to the user, a notification identifier, and custom data.
 */
void MoSync_DidReceivePushNotification(NSDictionary* pushNotification);