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

#import <Foundation/Foundation.h>
#include <helpers/cpp_defs.h>

#include "Syscall.h"

SYSCALL(int, maNotificationLocalCreate());
SYSCALL(int, maNotificationLocalDestroy(MAHandle notificationHandle));
SYSCALL(int, maNotificationLocalSetProperty(MAHandle notificationHandle,
											const char* property, const char* value));
SYSCALL(int, maNotificationLocalGetProperty(MAHandle notificationHandle,
											const char* property, char* value,
											const int bufSize));
SYSCALL(int, maNotificationLocalSchedule(MAHandle notificationHandle));
SYSCALL(int, maNotificationLocalUnschedule(MAHandle notificationHandle));
SYSCALL(int, maNotificationPushRegister(MAHandle pushNotificationType,
										const char* accountID));
SYSCALL(int, maNotificationPushUnregister());
SYSCALL(int, maNotificationPushGetData(MAHandle pushNotificationHandle,
									   MAPushNotificationData* pushNotificationData));
SYSCALL(int, maNotificationPushGetRegistration(char* buffer, const int size));
SYSCALL(int, maNotificationPushDestroy(MAHandle pushNotificationHandle));
SYSCALL(void, maNotificationSetIconBadge(const int applicationIconBadgeNumber));
SYSCALL(int, maNotificationGetIconBadge());
