/*
 Copyright (C) 2013 MoSync AB

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

#include <helpers/cpp_defs.h>
#include "Syscall.h"
#import <vector>

void initTimeStamps();

SYSCALL(int, maTime());

SYSCALL(int, maLocalTime());

SYSCALL(int, maGetMilliSecondCount());

SYSCALL(void, maWait(int timeout));

SYSCALL(int, maVibrate(int ms));

SYSCALL(int, maGetBatteryCharge());

SYSCALL(int, maPlatformRequest(const char* url));

SYSCALL(int, maGetKeys());

SYSCALL(int, maSendTextSMS(const char* dst, const char* msg));

int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints);