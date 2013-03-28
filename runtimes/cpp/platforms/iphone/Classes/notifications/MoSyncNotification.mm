/*
 Copyright (C) 2012 MoSync AB

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
