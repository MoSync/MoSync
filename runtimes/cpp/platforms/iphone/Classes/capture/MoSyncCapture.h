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

SYSCALL(int, maCaptureSetProperty(const char* property, const char* value));
SYSCALL(int, maCaptureGetProperty(const char* property, char* value, const int bufSize));
SYSCALL(int, maCaptureAction(const int action));
SYSCALL(int, maCaptureWriteImage(const int handle, const char* fullPath, const int fullPathSize));
SYSCALL(int, maCaptureGetVideoPath(const int handle, char* buffer, const int bufferSize));
SYSCALL(int, maCaptureDestroyData(const int handle));
