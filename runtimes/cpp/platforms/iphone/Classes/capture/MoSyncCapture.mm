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

#import "MoSyncCapture.h"
#import "Capture.h"

SYSCALL(int, maCaptureSetProperty(const char* property, const char* value))
{
	return [[Capture getInstance] setProperty:property withValue:value];
}

SYSCALL(int, maCaptureGetProperty(const char* property, char* value, const int bufSize))
{
	return [[Capture getInstance] getProperty:property value:value maxSize:bufSize];
}

SYSCALL(int, maCaptureAction(const int action))
{
	return [[Capture getInstance] action:action];
}

SYSCALL(int, maCaptureWriteImage(const int handle, const char* fullPath, const int fullPathSize))
{
	return [[Capture getInstance] writeImage:handle withPath:fullPath maxSize:fullPathSize];
}

SYSCALL(int, maCaptureGetVideoPath(const int handle, char* buffer, const int bufferSize))
{
	return [[Capture getInstance] getVideoPath:handle buffer:buffer maxSize:bufferSize];
}

SYSCALL(int, maCaptureDestroyData(const int handle))
{
	return [[Capture getInstance] destroyData:handle];
}
