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

void initMoSyncUISyscalls();

MAHandle maWidgetCreate(const char *widgetType);

void maWidgetDestroy(MAHandle handle) ;

// returns MA_WIDGET_ERROR on error
int maWidgetSetProperty(MAHandle handle, const char *property, const char* value);

// if maWidgetGetProperty returns <0 maxSize equals the size needed.
int maWidgetGetProperty(MAHandle handle, const char *property, char *value, int* maxSize);
void maWidgetAddChild(MAHandle parentHandle, MAHandle childHandle);
void maWidgetRemoveChild(MAHandle parent, MAHandle child);
void maWidgetScreenShow(MAHandle screenHandle);

#endif // _MOSYNC_UI_SYSCALLS_
