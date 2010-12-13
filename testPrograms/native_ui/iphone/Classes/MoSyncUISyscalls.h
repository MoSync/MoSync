//
//  MoSyncUISyscalls.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef _MOSYNC_UI_SYSCALLS_
#define _MOSYNC_UI_SYSCALLS_

#import <Foundation/Foundation.h>
#import "MoSyncUI.h"
#import "MoSyncUISyscalls.h"
#include <helpers/cpp_defs.h>

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController);

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
