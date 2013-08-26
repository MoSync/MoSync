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

#import <Foundation/Foundation.h>
#include <helpers/cpp_defs.h>

#include "Syscall.h"

SYSCALL(int, maCaptureSetProperty(const char* property, const char* value));
SYSCALL(int, maCaptureGetProperty(const char* property, char* value, const int bufSize));
SYSCALL(int, maCaptureAction(const int action));
SYSCALL(int, maCaptureWriteImage(const int handle, const char* fullPath, const int fullPathSize));
SYSCALL(int, maCaptureGetVideoPath(const int handle, char* buffer, const int bufferSize));
SYSCALL(int, maCaptureDestroyData(const int handle));
