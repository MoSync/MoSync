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

//
//  MoSyncGraphics.h
//  MoSync
//
//  Created by Bogdan Iusco on 2/6/13.
//
//

#include <helpers/cpp_defs.h>
#include "Syscall.h"

namespace Base
{
	extern bool gClosing;
}

extern Surface* gBackbuffer;
extern Surface* gDrawTarget;
extern MAHandle gDrawTargetHandle;

extern uint realColor;
extern uint currentColor;
extern float currentRed, currentGreen, currentBlue;
extern uint oldColor;
extern int gWidth, gHeight;
extern unsigned char *gBackBufferData;
extern CRITICAL_SECTION exitMutex;


extern void MoSyncGraphicsCreateBackbuffer();
SYSCALL(void, maSetClipRect(int left, int top, int width, int height));
SYSCALL(void, maGetClipRect(MARect *rect));
SYSCALL(int, maSetColor(int argb));
SYSCALL(void, maPlot(int posX, int posY));
SYSCALL(void, maLine(int x0, int y0, int x1, int y1));
SYSCALL(void, maFillRect(int left, int top, int width, int height));
SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count));
SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count));
SYSCALL(void, maUpdateScreen());
SYSCALL(void, maDrawImage(MAHandle image, int left, int top));
SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect,
						int scanlength));
SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft,
								int transformMode));
SYSCALL(MAExtent, maGetImageSize(MAHandle image));
SYSCALL(MAExtent, maGetImageSize(MAHandle image));
SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength));
SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle));
SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size));
SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha));
SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height));
SYSCALL(int, maFrameBufferGetInfo(MAFrameBufferInfo *info));
SYSCALL(int, maFrameBufferInit(const void *data));
SYSCALL(int, maFrameBufferClose());

//Text related
int stringLength(const wchar_t* str);

SYSCALL(void, maDrawTextW(int left, int top, const wchar* str));
SYSCALL(void, maDrawText(int left, int top, const char* str));
SYSCALL(MAExtent, maGetTextSizeW(const wchar* str));
SYSCALL(MAExtent, maGetTextSize(const char* str));
//}
