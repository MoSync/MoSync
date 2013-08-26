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
