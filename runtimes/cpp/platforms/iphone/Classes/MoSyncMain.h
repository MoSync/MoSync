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

