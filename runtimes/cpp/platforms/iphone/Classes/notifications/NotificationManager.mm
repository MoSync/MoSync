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
 * @file NotificationManager.mm
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Design pattern: singleton.
 * Notifications are used for displaying alerts, playing sounds, and badging application icons.
 * UILocalNotification type objects are used for local notifications.
 * Instances of UILocalNotification represent notifications that an application can schedule
 * for presentation to its users at specific dates and times.
 * The operating system is responsible for delivering the notification at the proper time.
 * The application does not have to be running for this to happen.
 */

/**
 * An example for a received push notification JSON data:
 *
 * aps =
 * {
 *   alert = "This is my message.";
 *   badge = 1;
 *   sound = "mysound.mp3";
 *  };
 *
 */
// Dictionary key name for push notification JSON data.
#define NOTIFICATION_KEY @"aps"

// Dictionary key names for the push notification values.
#define BADGE_ICON_KEY @"badge"
#define ALERT_MESSAGE_KEY @"alert"
#define SOUND_KEY @"sound"

#import "NotificationManager.h"

#import "IWidget.h"
#import "helpers/cpp_defs.h"
#import "Syscall.h"
#import "PushNotification.h"

@implementation NotificationManager

static NotificationManager *sharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(NotificationManager*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[NotificationManager alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

/**
 * Init function.
 */
- (id)init
{
    mLocalNotificationHandleCount = 0;
    mPushNotificationHandleCount = 0;
    mLocalNotificationDictionary = [[NSMutableDictionary alloc] init];
    mPushNotificationDictionary = [[NSMutableDictionary alloc] init];
    mIsPushNotificationEnabled = false;

    return [super init];
}

/**
 * Creates a new local notification object.
 * @return:
 * - MA_NOTIFICATION_RES_UNSUPPORTED if the notifications are not supported on current system.
 * - MA_NOTIFICATION_RES_ERROR if a error occurred while creating the notification object.
 * - a handle to a notification object.
 */
-(MAHandle) createNotificationObject
{
    mLocalNotificationHandleCount++;
    if (INT32_MAX == mLocalNotificationHandleCount)
    {
        return MA_NOTIFICATION_RES_ERROR;
    }

    // Check if notifications are supported on current platform.
    Class cls = NSClassFromString(@"UILocalNotification");
    if (cls)
    {
        UILocalNotification *notification = [[cls alloc] init];
        if (!notification)
        {
            return MA_NOTIFICATION_RES_ERROR;
        }

        notification.timeZone = [NSTimeZone systemTimeZone];
        notification.fireDate = [NSDate date];
        [mLocalNotificationDictionary setObject:notification
                                    forKey:[NSNumber numberWithInt:mLocalNotificationHandleCount]];
    }
    else
    {
        return MA_NOTIFICATION_RES_UNSUPPORTED;
    }
    return mLocalNotificationHandleCount;
}

/**
 * Destroy a notification object.
 * @param notificationHandle Handle to a notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) destroyNotificationObject:(MAHandle) notificationHandle
{
    NSNumber* key = [NSNumber numberWithInt:notificationHandle];
    UILocalNotification* notification = [mLocalNotificationDictionary objectForKey:key];
    if (!notification)
    {
        NSLog(@"destroyNotificationObject invalid handle");
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    [mLocalNotificationDictionary removeObjectForKey:key];
    [notification release];
    return MA_NOTIFICATION_RES_OK;
}

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
                      property:(const char*) propertyNameChar
                         value:(const char*) valueChar
{
    NSNumber* key = [NSNumber numberWithInt:notificationHandle];
    UILocalNotification* notification = [mLocalNotificationDictionary objectForKey:key];
    if (!notification)
    {
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    NSString* propertyName = [NSString stringWithUTF8String:propertyNameChar];
    NSString* value = [NSString stringWithUTF8String:valueChar];
    if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_BADGE_NUMBER])
    {
        notification.applicationIconBadgeNumber = [value intValue];
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_FIRE_DATE])
    {
        // Notification fire date must be in GMT format.
        double seconds = [value doubleValue];
        NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];
        seconds -= [localTimeZone secondsFromGMT];
        NSDate* date = [NSDate dateWithTimeIntervalSince1970:seconds];
        notification.fireDate = date;
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_CONTENT_BODY])
    {
        notification.alertBody = value;
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_ALERT_ACTION])
    {
        notification.alertAction = value;
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_PLAY_SOUND])
    {
        if ([value isEqualToString:@"true"])
        {
            notification.soundName = UILocalNotificationDefaultSoundName;
        }
        else if ([value isEqualToString:@"false"])
        {
            notification.soundName = nil;
        }
        else
        {
            return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else
    {
        return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
    }

    return MA_NOTIFICATION_RES_OK;
}

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
                          size:(int) maxSize
{
    NSNumber* key = [NSNumber numberWithInt:notificationHandle];
    UILocalNotification* notification = [mLocalNotificationDictionary objectForKey:key];
    if (!notification)
    {
        NSLog(@"notificationGetProperty invalid handle");
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    NSString* propertyName = [NSString stringWithUTF8String:property];
    NSString* retVal = nil;
    if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_BADGE_NUMBER])
    {
        retVal = [[NSString alloc] initWithFormat:@"%d",notification.applicationIconBadgeNumber];
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_FIRE_DATE])
    {
        // Notification's fire date is in GMT format.
        // We need to convert it to local time.
        NSDate* fireDate = notification.fireDate;
        double seconds = [fireDate timeIntervalSince1970];
        NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];
        seconds += [localTimeZone secondsFromGMT];
        retVal = [[NSString alloc] initWithFormat:@"%d",(int)seconds];
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_CONTENT_BODY])
    {
        if (notification.alertBody)
        {
            retVal = [[NSString alloc] initWithString:notification.alertBody];
        }
        else
        {
            retVal = [[NSString alloc] init];
        }
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_ALERT_ACTION])
    {
        if (notification.alertAction)
        {
            retVal = [[NSString alloc] initWithString:notification.alertAction];
        }
        else
        {
            retVal = [[NSString alloc] init];
        }
    }
    else if ([propertyName isEqualToString:@MA_NOTIFICATION_LOCAL_PLAY_SOUND])
    {
        if (notification.soundName)
        {
            retVal = [[NSString alloc] initWithString:@"true"];
        }
        else
        {
            retVal = [[NSString alloc] initWithString:@"false"];
        }
    }
    else
    {
        return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
    }

    if(!retVal)
    {
        NSLog(@"notificationGetProperty invalid property name");
        return MA_ADS_RES_INVALID_PROPERTY_NAME;
    }

	int length = maxSize;
	int realLength = [retVal length];
	if(realLength > length)
    {
        NSLog(@"notificationGetProperty invalid buffer size");
		return MA_ADS_RES_INVALID_STRING_BUFFER_SIZE;
	}

    [retVal getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	[retVal release];
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Schedules a local notification for delivery at its encapsulated date and time.
 * @param notificationHandle Handle to a local notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) registerLocalNotification:(MAHandle) notificationHandle
{
    NSNumber* key = [NSNumber numberWithInt:notificationHandle];
    UILocalNotification* notification = [mLocalNotificationDictionary objectForKey:key];
    if (!notification)
    {
        NSLog(@"registerLocalNotification invalid handle");
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Cancels the delivery of the specified scheduled local notification.
 * @param notificationHandle Handle to a local notification object.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
 */
-(int) unregisterLocalNotification:(MAHandle) notificationHandle
{
    NSNumber* key = [NSNumber numberWithInt:notificationHandle];
    UILocalNotification* notification = [mLocalNotificationDictionary objectForKey:key];
    if (!notification)
    {
        NSLog(@"unregisterLocalNotification invalid handle");
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    [[UIApplication sharedApplication] cancelLocalNotification:notification];
    [notification release];
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Call this method only when a local notification is received.
 * @param notification A local notification that encapsulates details about
 * the notification, potentially including custom data.
 * Sends MoSync event containing a handle to the local notification.
 */
- (void) didReceiveLocalNotification:(UILocalNotification *)notification
{
    NSLog(@"Notification::didReceiveLocalNotification");
    MAHandle handle = [self getNotificationHandle:notification];
    if (MA_NOTIFICATION_RES_INVALID_HANDLE != handle)
    {
        MAEvent event;
        event.type = EVENT_TYPE_LOCAL_NOTIFICATION;
        event.localNotificationHandle = handle;
        Base::gEventQueue.put(event);
    }
}

/**
 * Get the handle assocciated with a given category.
 * @param notification The given notification.
 * @return The handle or MA_NOTIFICATION_RES_INVALID_HANDLE if the given
 * notification is not registered to this class.
 */
-(MAHandle) getNotificationHandle:(UILocalNotification*) notification
{
    NSArray* allKeys = [mLocalNotificationDictionary allKeys];
    for (NSNumber* key in allKeys)
    {
        UILocalNotification* localNotification = [mLocalNotificationDictionary objectForKey:key];
        if (localNotification == notification)
        {
            return [key intValue];
        }
    }

    return MA_NOTIFICATION_RES_INVALID_HANDLE;
}

/**
 * Registers the current application for receiving push notifications.
 * @param pushNotificationType A bit mask specifying the types of notifications
 * the application accepts. See MA_NOTIFICATION_PUSH_TYPE for valid bit-mask values.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_ALREADY_REGISTERED if the application is already registered for
 * receiving push notifications.
 */
-(int) registerPushNotification:(MAHandle) pushNotificationType
{
    if (mIsPushNotificationEnabled)
    {
        return MA_NOTIFICATION_RES_ALREADY_REGISTERED;
    }

    int type = UIRemoteNotificationTypeNone;
    if (pushNotificationType & MA_NOTIFICATION_PUSH_TYPE_BADGE)
    {
        type = UIRemoteNotificationTypeBadge;
    }
    if (pushNotificationType & MA_NOTIFICATION_PUSH_TYPE_SOUND)
    {
        type = type | UIRemoteNotificationTypeSound;
    }
    if (pushNotificationType & MA_NOTIFICATION_PUSH_TYPE_ALERT)
    {
        type = type | UIRemoteNotificationTypeAlert;
    }

    [[UIApplication sharedApplication] registerForRemoteNotificationTypes: (UIRemoteNotificationType)type];
    mIsPushNotificationEnabled = true;
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Unregister application for push notifications.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if no error occurred.
 * - MA_NOTIFICATION_RES_NOT_REGISTERED if the application was not registered for receiving
 * push notification.
 */
-(int) unregisterPushNotification
{
    if (!mIsPushNotificationEnabled)
    {
        return MA_NOTIFICATION_RES_NOT_REGISTERED;
    }
    mIsPushNotificationEnabled = false;
    [[UIApplication sharedApplication] unregisterForRemoteNotifications];
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Set the number currently set as the badge of the application icon in Springboard.
 * @param applicationIconBadgeNumber Set to zero to hide the badge number. The default is zero.
 * If this value is negative the method will do nothing.
 */
-(void) setApplicationIconBadgeNumber:(const int) applicationIconBadgeNumber
{
    if (0 <= applicationIconBadgeNumber)
    {
        [[UIApplication sharedApplication] setApplicationIconBadgeNumber:applicationIconBadgeNumber];
    }
}

/**
 * Get the number currently set as the badge of the application icon in Springboard.
 * @return The number displayed as the badge of the application.
 */
-(int) getApplicationIconBadgeNumber
{
    return [[UIApplication sharedApplication] applicationIconBadgeNumber];
}

/**
 * Called when a running application receives a push notification.
 * @param pushNotification A dictionary that contains information related to the push notification,
 * potentially including a badge number for the application icon, an alert sound, an alert message
 * to display to the user, a notification identifier, and custom data.
 * Sends mosync events containing push notification data.
 */
- (void) didReceivePushNotification:(NSDictionary*) pushNotification
{
    NSDictionary* apsDict = [pushNotification objectForKey:NOTIFICATION_KEY];
    if (!apsDict)
    {
        NSLog(@"The push notification object is invalid.");
        return;
    }

    // Extract values from dictionary.
    NSString* alertMessage = [apsDict objectForKey:ALERT_MESSAGE_KEY];
    NSString* sound = [apsDict objectForKey:SOUND_KEY];
    NSNumber* badge = [apsDict objectForKey:BADGE_ICON_KEY];

    PushNotification* pushNotificationObj = [[PushNotification alloc] init];
    if (alertMessage)
    {
        pushNotificationObj.alertMessage = alertMessage;
    }
    if (sound)
    {
        pushNotificationObj.soundFileName = sound;
    }
    if (badge)
    {
        pushNotificationObj.badgeIcon = badge;
    }

    // Store the push notification object into dictionary.
    mPushNotificationHandleCount++;
    [mPushNotificationDictionary setObject:pushNotificationObj
                                    forKey:[NSNumber numberWithInt:mPushNotificationHandleCount]];

    // Send push notification event.
    MAEvent event;
    event.type = EVENT_TYPE_PUSH_NOTIFICATION;
    event.pushNotificationHandle = mPushNotificationHandleCount;
    Base::gEventQueue.put(event);
}

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
                          data:(MAPushNotificationData*) pushNotificationData
{
    PushNotification* pushNotification = [mPushNotificationDictionary objectForKey:
                                          [NSNumber numberWithInt:pushNotificationHandle]];
    if (!pushNotificationHandle)
    {
        return MA_NOTIFICATION_RES_INVALID_HANDLE;
    }

    int pushNotificationType = 0;
    int maxLength = pushNotificationData->alertMessageSize;
    int address = (int) pushNotificationData->alertMessage;
    char* charAddress = (char*) Base::gSyscall->GetValidatedMemRange(address, maxLength);

    NSString* messageAlert = pushNotification.alertMessage;
    if (messageAlert)
    {
        [messageAlert getCString:charAddress maxLength:maxLength encoding:NSASCIIStringEncoding];
        pushNotificationType = pushNotificationType | MA_NOTIFICATION_PUSH_TYPE_ALERT;
    }

    maxLength = pushNotificationData->soundFileNameSize;
    address = (int) pushNotificationData->soundFileName;
    charAddress = (char*) Base::gSyscall->GetValidatedMemRange(address, maxLength);

    NSString* soundFileName = pushNotification.soundFileName;
    if (soundFileName)
    {
        [soundFileName getCString:charAddress maxLength:maxLength encoding:NSASCIIStringEncoding];
        pushNotificationType = pushNotificationType | MA_NOTIFICATION_PUSH_TYPE_SOUND;
    }

    NSNumber* iconBadge = pushNotification.badgeIcon;
    if (iconBadge)
    {
        pushNotificationData->badgeIcon = [iconBadge intValue];
        pushNotificationType = pushNotificationType | MA_NOTIFICATION_PUSH_TYPE_BADGE;
    }
    else
    {
        pushNotificationData->badgeIcon = 0;
    }

    pushNotificationData->type = pushNotificationType;
    return MA_NOTIFICATION_RES_OK;
}

/**
 * Gets the latest registration response.
 * @param messageAddress The registrationID if the registration was successfull,
 * or the error messsage otherwise.
 * @param size The size of the buffer.
 * @return One of the next constants:
 * - MA_NOTIFICATION_RES_OK if the application registered successfully.
 * - MA_NOTIFICATION_RES_REGISTRATION_MESSAGE_BUF_TOO_SMALL if the buffer is too small.
 * - MA_NOTIFICATION_RES_ERROR in case of error.
 */
-(int) getPushRegistrationData:(char*) messageAddress
                          size:(const int) size
{
    int result = MA_NOTIFICATION_RES_OK;
    NSString* messageString;
    if (!mRegistrationError)
    {
        messageString = mDeviceToken;
    }
    else
    {
        messageString = mRegistrationError;
        result = MA_NOTIFICATION_RES_REGISTRATION_SERVICE_NOT_AVAILABLE;
    }

    if ([messageString length] > size)
    {
        return MA_NOTIFICATION_RES_REGISTRATION_MESSAGE_BUF_TOO_SMALL;
    }

    [messageString getCString:messageAddress maxLength:size encoding:NSASCIIStringEncoding];
    return result;
}

/**
 * Called when application registration response for push notification has been received.
 * @param deviceToken The device token received from the server.
 */
-(void) didRegisterForPushNotification:(NSString*) deviceToken
{
    [mDeviceToken release];
    mDeviceToken = [deviceToken retain];
}

/**
 * Called when Apple Push Service cannot successfully complete the registration process.
 * @param error Error's localized description.
 */
-(void) didFailToRegisterForPushNotification:(NSString*) error
{
    [mRegistrationError release];
    mRegistrationError = [error retain];
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [mLocalNotificationDictionary release];
    [mPushNotificationDictionary release];
    [mDeviceToken release];
    [mRegistrationError release];

    [super dealloc];
}

@end

/**
 * Called when the application receives a local notification.
 * @param localNotification A local notification that encapsulates details about the notification,
 * potentially including custom data.
 */
void MoSync_DidReceiveLocalNotification(UILocalNotification* localNotification)
{
    [[NotificationManager getInstance] didReceiveLocalNotification:localNotification];
}

/**
 * Called when a running application receives a push notification.
 * @param pushNotification A dictionary that contains information related to the push notification,
 * potentially including a badge number for the application icon, an alert sound, an alert message
 * to display to the user, a notification identifier, and custom data.
 */
void MoSync_DidReceivePushNotification(NSDictionary* pushNotification)
{
    [[NotificationManager getInstance] didReceivePushNotification:pushNotification];
}

/**
 * Called when application registration response for push notification has been received.
 * @param errorCode Zero if the application has been successfully registered, or the error code otherwise.
 * @param text The device token if the application has been registered, or the localized error otherwise.
 */
void MoSync_ApplicationRegistration(NSInteger errorCode, NSString* text)
{
    if (0 == errorCode)
    {
        [[NotificationManager getInstance] didRegisterForPushNotification: text];
    }
    else
    {
        [[NotificationManager getInstance] didFailToRegisterForPushNotification: text];
    }

    MAEvent event;
    event.type = EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION;
    Base::gEventQueue.put(event);
}