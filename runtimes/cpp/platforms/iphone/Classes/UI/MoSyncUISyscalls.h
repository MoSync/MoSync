/* Copyright (C) 2011 MoSync AB
 
 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#ifndef _MOSYNC_UI_SYSCALLS_
#define _MOSYNC_UI_SYSCALLS_

#import <Foundation/Foundation.h>
#import "MoSyncUI.h"
#import "MoSyncUISyscalls.h"
#include <helpers/cpp_defs.h>

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController);
bool isNativeUIEnabled();

MAHandle maWidgetCreate(const char *widgetType);

void maWidgetDestroy(MAHandle handle) ;

// returns MA_WIDGET_ERROR on error
int maWidgetSetProperty(MAHandle handle, const char *property, const char* value);

// if maWidgetGetProperty returns <0 maxSize equals the size needed. removed pointer.
int maWidgetGetProperty(MAHandle handle, const char *property, char *value, int maxSize);
int maWidgetAddChild(MAHandle parentHandle, MAHandle childHandle);
int maWidgetRemoveChild(MAHandle parent, MAHandle child);
void maWidgetScreenShow(MAHandle screenHandle);
int maWidgetEvaluateScript(MAHandle widget, const char* script);
int maWidgetGetMessageData(int messageId, void* buffer, int bufferSize);

#endif // _MOSYNC_UI_SYSCALLS_
