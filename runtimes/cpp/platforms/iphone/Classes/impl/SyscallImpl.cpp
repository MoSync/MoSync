/* Copyright (C) 2009 Mobile Sorcery AB

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

#define WIN32_LEAN_AND_MEAN

#include "config_platform.h"

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <math.h>
#include <helpers/fifo.h>
#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/smartie.h>
#include <helpers/cpp_defs.h>
#include <MemStream.h>
#include <FileStream.h>
#include "Syscall.h"

#include <helpers/CPP_IX_GUIDO.h>
#include "netImpl.h"

#define NETWORKING_H
#include "networking.h"
#include <bluetooth/discovery.h>
#include <base_errors.h>
using namespace MoSyncError;

#include <core/core.h>

extern ThreadPool gThreadPool;

#define NOT_IMPLEMENTED BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED)

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************
	static void MAUpdateScreen();

	Syscall* gSyscall;

	uint realColor;
	uint currentColor;
	uint oldColor;

	int gWidth, gHeight;
	CGContextRef gBackbuffer;
	unsigned char *gBackBufferData;
	
	static ResourceArray gResourceArray;

	static CircularFifo<MAEvent, EVENT_BUFFER_SIZE> gEventFifo;
	static bool gEventOverflow	= false;
	static bool gClosing		= false;

	int gVolume = -1;

	bool exited = false;
	CRITICAL_SECTION exitMutex;
	
	void MALibQuit();

	//***************************************************************************
	// Resource loading
	//***************************************************************************
	
	Image* Syscall::loadImage(MemStream& s) 
	{
		return NULL;
	}
	
	Image* Syscall::loadSprite(void* surface, ushort left, ushort top,
		ushort width, ushort height, ushort cx, ushort cy) 
	{
		return NULL;
	}
	
	//***************************************************************************
	// Helpers
	//***************************************************************************

	static bool MALibInit() {

		InitializeCriticalSection(&exitMutex);
		
		gBackBufferData = new unsigned char[gWidth*4*gHeight];
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		gBackbuffer = 
		CGBitmapContextCreate(gBackBufferData, gWidth, gHeight, 8, gWidth*4, colorSpace, kCGBitmapByteOrderDefault);
		CGColorSpaceRelease(colorSpace);
		CGContextTranslateCTM(gBackbuffer, 0, gHeight);
		CGContextScaleCTM(gBackbuffer, 1.0, -1.0);
		return true;
	}

	void MALibQuit() {
		DeleteCriticalSection(&exitMutex);
		
	}


	bool MAProcessEvents() //returns true if maWait should return
	{
		return false;
	}
	
	//***************************************************************************
	// Syscall class
	//***************************************************************************

	Syscall::Syscall(int w, int h)
	{
		gWidth = w;
		gHeight = h;
		gSyscall = this;
		bool ret = MALibInit();
		if(ret == false) 
		{
			// if program is already open or something failed...
			exit(0);
		}

	}

	void Syscall::platformDestruct() {
		MALibQuit();
	}

	//***************************************************************************
	// Proper syscalls
	//***************************************************************************
	SYSCALL(void, maSetClipRect(int left, int top, int width, int height))
	{
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maGetClipRect(MARect *rect))
	{
		gSyscall->ValidateMemRange(rect, sizeof(MARect));		
		NOT_IMPLEMENTED;
	}

#define CONVERT_TO_NATIVE_COLOR_FROM_ARGB(col) \
	((((col)&0x00ff0000)>>(16-(redShift-(8-redBits))))&redMask) | \
	((((col)&0x0000ff00)>>(8-(greenShift-(8-greenBits))))&greenMask) | \
	((((col)&0x000000ff)>>(0-(blueShift-(8-blueBits))))&blueMask)

#define CONVERT_TO_ARGB_FROM_NATIVE_COLOR(col) \
	rgb16_to_rgb32(col, redMask, greenMask, blueMask, redShift, greenShift, blueShift, redBits, greenBits, blueBits)
//	((((((col)&redMask)>>redShift)<<(8-redBits))<<16)&0x00ff0000)|\
//	((((((col)&greenMask)>>greenShift)<<(8-greenBits))<<8)&0x0000ff00)|\
//	((((((col)&blueMask)>>blueShift)<<(8-blueBits)))&0x000000ff)\
//	(0xff000000)

	SYSCALL(int, maSetColor(int argb)) {
		oldColor = currentColor;
		currentColor = argb;
		//realColor =	CONVERT_TO_NATIVE_COLOR_FROM_ARGB(argb);
		int red = (argb&0x00ff0000)>>16;
		int green = (argb&0x0000ff00)>>8;
		int blue = (argb&0xff);
		CGContextSetRGBStrokeColor(gBackbuffer, red, green, blue, 1);
		CGContextSetRGBFillColor(gBackbuffer, red, green, blue, 1);
		return oldColor;
	}

	SYSCALL(void, maPlot(int posX, int posY)) {
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maLine(int x0, int y0, int x1, int y1)) {
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		CGContextFillRect(gBackbuffer, CGRectMake(left, top, width, height));

	}

	SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		NOT_IMPLEMENTED;
	}

	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(MAExtent, maGetTextSizeW(const char* str)) {
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		CGContextSelectFont(gBackbuffer, "Helvetica", 24.0, kCGEncodingMacRoman);
		CGContextSetTextDrawingMode(gBackbuffer, kCGTextFill);
		CGContextSetRGBFillColor(gBackbuffer, 0, 255, 255, 1);
		
		CGAffineTransform xform = CGAffineTransformMake(
														1.0,  0.0,
														0.0, -1.0,
														0.0,  0.0);
		CGContextSetTextMatrix(gBackbuffer, xform);
		
		CGContextShowTextAtPoint(gBackbuffer, left, top, str, strlen(str));
	}

	SYSCALL(void, maDrawTextW(int left, int top, const wchar* str)) {
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maUpdateScreen()) {
		if(gClosing)
			return;
		NOT_IMPLEMENTED;		
		//MAProcessEvents();
		//MAUpdateScreen();
	}

	SYSCALL(void, maResetBacklight()) {
		NOT_IMPLEMENTED;
	}

	SYSCALL(MAExtent, maGetScrSize()) {
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		Image* img = gSyscall->resources.get_RT_IMAGE(image);	
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect,
		int scanlength)) {
		gSyscall->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(srcRect, sizeof(MARect));
		gSyscall->ValidateMemRange(src, scanlength*srcRect->height*4);
		NOT_IMPLEMENTED;
	}

	SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft,
		int transformMode))
	{
		gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(src, sizeof(MARect));	
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		NOT_IMPLEMENTED;
	}

	SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		NOT_IMPLEMENTED;
		return 0;
	}

#define BIG_PHAT_SOURCE_RECT_ERROR {BIG_PHAT_ERROR(WCEERR_SOURCE_RECT_OOB)}

	SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		Image* img = gSyscall->resources.get_RT_IMAGE(image);
		int x = src->left;
		int y = src->top;
		int width = src->width;
		int height = src->height;
		gSyscall->ValidateMemRange(dst, src->height*scanlength);

		NOT_IMPLEMENTED;
	}

	SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size)) {
		MYASSERT(size>0, ERR_DATA_OOB);
		Stream* stream = gSyscall->resources.get_RT_BINARY(data);
		//return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
		NOT_IMPLEMENTED;		
	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha)) {
		int width = EXTENT_X(size); int height = EXTENT_Y(size);
		gSyscall->ValidateMemRange(src, width*height*4);
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(void, maCloseStore(MAHandle store, int del)) 
	{
		NOT_IMPLEMENTED;
	}

	SYSCALL(int, maGetKeys()) 
	{
		if(gClosing)
			return 0;
		//MAProcessEvents();
		//return currentKeyState;
		NOT_IMPLEMENTED;
		return 0;
	}


	SYSCALL(int, maGetEvent(MAEvent *dst)) 
	{
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));	
		MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		//MAProcessEvents();
		NOT_IMPLEMENTED;
		/*
		if(!gClosing)
			gEventOverflow = false;
		if(gEventFifo.count() == 0)
			return 0;
		*dst = gEventFifo.get();

		#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(dst->type == eventType) {\
			memcpy(Core::GetCustomEventPointer(gCore), dst->data, sizeof(dataType));\
			delete (dataType*)dst->data;\
			dst->data = (void*)(int(Core::GetCustomEventPointer(gCore)) - int(gCore->mem_ds)); }

		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);
		*/
		
		return 1;
	}

	SYSCALL(void, maWait(int timeout)) 
	{
		if(gClosing)
			return;

		if(gEventFifo.count() != 0)
			return;
		
		NOT_IMPLEMENTED;
	}

	SYSCALL(int, maTime()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maLocalTime()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maGetMilliSecondCount()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maFreeObjectMemory()) {
		NOT_IMPLEMENTED;
		return 0;
	}
	SYSCALL(int, maTotalObjectMemory()) {
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maVibrate(int ms)) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maPanic(int result, char* message)) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maPlatformRequest(const char* url)) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size)) 
	{ 
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(void, maSoundStop()) 
	{
		NOT_IMPLEMENTED;
	}

	SYSCALL(int, maSoundIsPlaying()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	SYSCALL(void, maSoundSetVolume(int vol)) 
	{
		NOT_IMPLEMENTED;
	}

	SYSCALL(int, maSoundGetVolume()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}
	
	SYSCALL(int, maGetBatteryCharge()) 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	int maKeypadIsLocked() 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	int maLockKeypad() 
	{
		NOT_IMPLEMENTED;
		return 0;
	}

	int maUnlockKeypad() 
	{
		NOT_IMPLEMENTED;
		return 0;
	}



	SYSCALL(int, maSendTextSMS(const char* dst, const char* msg)) {
		NOT_IMPLEMENTED;
		return 1;
	}

	SYSCALL(int, maInvokeExtension(int, int, int, int)) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}

	SYSCALL(int, maIOCtl(int function, int a, int b, int c)) 
	{
		switch(function) {

		case maIOCtl_maCheckInterfaceVersion:
			return Base::maCheckInterfaceVersion(a);

		case maIOCtl_maWriteLog:
			LOGBIN(gSyscall->GetValidatedMemRange(a, b), b);
			return 0;
				
		}
		
		return IOCTL_UNAVAILABLE;
	}
} // namespace Base

void MoSyncExit(int r) 
{
	LOG("MoSyncExit(%d)\n", r);

	EnterCriticalSection(&exitMutex);
	if(!exited) {
		exited = true;
		LeaveCriticalSection(&exitMutex);
		exit(r);
		EnterCriticalSection(&exitMutex);
	}
	LeaveCriticalSection(&exitMutex);
}

void MoSyncErrorExit(int errorCode) 
{
	LOG("ErrorExit %i\n", errorCode);
	char buffer[256];
	char* ptr = buffer + sprintf(buffer, "MoSync Panic\np%i.", errorCode);
	if(gCore) {
#ifdef PUBLIC_DEBUG
		if(SYSCALL_NUMBER_IS_VALID(gCore->currentSyscallId)) {
			ptr += sprintf(ptr, "s%i.", gCore->currentSyscallId);
		}
		if(gRunning) {
			ptr += sprintf(ptr, "i%x.", Core::GetIp(gCore));
		}
#endif
		if(gRunning) {
			char appCode[5];
			for(int i=0; i<4; i++) {
				appCode[i] = (byte)(gCore->Head.AppCode >> ((3-i)*8));
			}
			appCode[4] = 0;
			sprintf(ptr, "%s", appCode);
		}
	}
	
	MoSyncExit(errorCode);
}
	