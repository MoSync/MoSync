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

/*
 * This is the syscall implementation for the native ui api.
 */

#import <Foundation/Foundation.h>
#import "MoSyncUI.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController);
bool isNativeUIEnabled();

MoSyncUI* getMoSyncUI();

MAWidgetHandle maWidgetCreate(const char *widgetType);

int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value);

int maWidgetGetProperty(MAWidgetHandle handle, const char *property, char *value, int maxSize);

int maWidgetPerformAction(MAWidgetHandle widget, const char *action, const char *param);

int maWidgetAddChild(MAWidgetHandle parentHandle, MAHandle childHandle);

int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index);

int maWidgetDestroy(MAWidgetHandle handle) ;

int maWidgetRemoveChild(MAWidgetHandle child);

int maWidgetStackScreenPush(MAWidgetHandle stackScreen, MAWidgetHandle screen);

int maWidgetStackScreenPop(MAWidgetHandle stackScreen);

int maWidgetScreenShow(MAWidgetHandle screenHandle);

int maWidgetModalDialogShow(MAWidgetHandle dialogHandle);

int maWidgetModalDialogHide(MAWidgetHandle dialogHandle);

#endif // _MOSYNC_UI_SYSCALLS_
