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

// TODO Make proper files/containers for every categry.

// Time related
void initTimeStamps();

SYSCALL(int, maTime());

SYSCALL(int, maLocalTime());

SYSCALL(int, maGetMilliSecondCount());

SYSCALL(void, maWait(int timeout));

// Platform related
SYSCALL(int, maWakeLock(int flag));

SYSCALL(void, maResetBacklight());

SYSCALL(int, maVibrate(int ms));

SYSCALL(int, maGetBatteryCharge());

SYSCALL(int, maPlatformRequest(const char* url));

SYSCALL(int, maGetKeys());

int maGetSystemProperty(const char *key, char *buf, int size);

// UI related
SYSCALL(MAExtent, maGetScrSize());

SYSCALL(void, maMessageBox(const char* title, const char* message));

SYSCALL(void, maImagePickerOpen());

SYSCALL(void, maImagePickerOpenWithEventReturnType(int returnType));

SYSCALL(int, maSaveImageToDeviceGallery(MAHandle imageDataHandle, const char* imageName));

SYSCALL(void, maAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3));

SYSCALL(void, maOptionsBox(const wchar* title, const wchar* destructiveButtonTitle, const wchar* cancelButtonTitle,
                           const void* otherButtonTitles, const int otherButtonTitlesSize));

int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints);

// File related
SYSCALL(int, maFileSetProperty(const char* path, int property, int value));

// Mobility
SYSCALL(int, maSendTextSMS(const char* dst, const char* msg));

// MoSync basic
int maReportResourceInformation();

SYSCALL(int, maFreeObjectMemory());

SYSCALL(int, maTotalObjectMemory());

SYSCALL(int, maSyscallPanicsEnable());

SYSCALL(int, maSyscallPanicsDisable());

SYSCALL(void, maPanic(int result, const char* message));

// MoSync Extensions
SYSCALL(MAExtensionModule, maExtensionModuleLoad(const char* name, int hash));

SYSCALL(MAExtensionFunction, maExtensionFunctionLoad(MAHandle module, int index));

SYSCALL(longlong, maExtensionFunctionInvoke(int, int, int, int MA_IOCTL_ELLIPSIS));
