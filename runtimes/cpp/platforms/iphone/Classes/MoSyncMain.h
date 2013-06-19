/* Copyright (C) 2010 MoSync AB

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

#ifndef _MOSYNCMAIN_H_
#define _MOSYNCMAIN_H_

#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>
#include <AVFoundation/AVFoundation.h>
#include "Platform.h"
#include "MosyncView.h"

// true for native library approach
extern bool gNativeMode;

void MoSync_Main(int width, int height, MoSyncView* mosyncView);

void MoSync_UpdateView(CGImageRef ref);
void MoSync_DoneUpdatingView();

void MoSync_Exit();

void MoSync_ReloadProgram(MAHandle data, int reload);

void MoSync_AddTouchPressedEvent(int x, int y, int touchId);
void MoSync_AddTouchMovedEvent(int x, int y, int touchId);
void MoSync_AddTouchReleasedEvent(int x, int y, int touchId);
void MoSync_AddScreenChangedEvent();
void MoSync_AddCloseEvent();
void MoSync_AddFocusLostEvent();
void MoSync_AddFocusGainedEvent();

void* MoSync_GetCustomEventData();
void* MoSync_GetCustomEventDataMoSyncPointer();
void MoSync_AddLayerToView(CALayer* layer);

/*
void MoSync_AddLocationProviderEvent(int state);
void MoSync_AddLocationEvent(int state, const MALocation& location);
*/

#endif

