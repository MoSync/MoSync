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

#import "MoSyncNotification.h"
#import "NotificationManager.h"

SYSCALL(int, maNotificationLocalCreate())
{
	return [[NotificationManager getInstance] createLocalNotificationObject];
}

SYSCALL(int, maNotificationLocalDestroy(MAHandle notificationHandle))
{
	return [[NotificationManager getInstance] destroyLocalNotificationObject:notificationHandle];
}

SYSCALL(int, maNotificationLocalSetProperty(MAHandle notificationHandle, const char* property, const char* value))
{
	return [[NotificationManager getInstance] localNotificationSetProperty:notificationHandle
																  property:property
																	 value:value];
}

SYSCALL(int, maNotificationLocalGetProperty(MAHandle notificationHandle, const char* property,
											char* value, const int bufSize))
{
	return [[NotificationManager getInstance] localNotificationGetProperty:notificationHandle
																  property:property
																	 value:value
																	  size:bufSize];
}

SYSCALL(int, maNotificationLocalSchedule(MAHandle notificationHandle))
{
	return [[NotificationManager getInstance] registerLocalNotification:notificationHandle];
}

SYSCALL(int, maNotificationLocalUnschedule(MAHandle notificationHandle))
{
	return [[NotificationManager getInstance] unregisterLocalNotification:notificationHandle];
}

SYSCALL(int, maNotificationPushRegister(MAHandle pushNotificationType, const char* accountID))
{
	return [[NotificationManager getInstance] registerPushNotification:pushNotificationType];
}

SYSCALL(int, maNotificationPushUnregister())
{
	return [[NotificationManager getInstance] unregisterPushNotification];
}

SYSCALL(int, maNotificationPushGetData(MAHandle pushNotificationHandle,
									   MAPushNotificationData* pushNotificationData))
{
	return [[NotificationManager getInstance] getPushNotificationData:pushNotificationHandle
																 data:pushNotificationData];
}

SYSCALL(int, maNotificationPushGetRegistration(char* buffer, const int size))
{
	return [[NotificationManager getInstance] getPushRegistrationData:buffer size:size];
}

SYSCALL(int, maNotificationPushDestroy(MAHandle pushNotificationHandle))
{
	return [[NotificationManager getInstance] pushNotificationDestroy:pushNotificationHandle];
}

SYSCALL(void, maNotificationSetIconBadge(const int applicationIconBadgeNumber))
{
	[[NotificationManager getInstance] setApplicationIconBadgeNumber:applicationIconBadgeNumber];
}

SYSCALL(int, maNotificationGetIconBadge())
{
	return [[NotificationManager getInstance] getApplicationIconBadgeNumber];
}
