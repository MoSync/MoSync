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

SYSCALL(int, maCameraStart());
SYSCALL(int, maCameraStop());
SYSCALL(int, maCameraSetPreview(MAHandle widgetHandle));
SYSCALL(int, maCameraSelect(MAHandle cameraNumber));
SYSCALL(int, maCameraNumber());
SYSCALL(int, maCameraSnapshot(int formatIndex, MAHandle placeholder));
SYSCALL(int, maCameraSnapshotAsync(MAHandle placeholder, int formatIndex));
SYSCALL(int, maCameraRecord(int stopStartFlag));
SYSCALL(int, maCameraSetProperty(const char *property, const char* value));
SYSCALL(int, maCameraGetProperty(const char *property, char *value, int maxSize));
SYSCALL(int, maCameraPreviewSize());
SYSCALL(int, maCameraPreviewEventEnable(int previewEventType, void* previewBuffer, const MARect* previewArea));
SYSCALL(int, maCameraPreviewEventDisable());
SYSCALL(int, maCameraPreviewEventConsumed());
