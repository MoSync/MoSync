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

#include "MoSyncMain.h"

#include "iphone_helpers.h"
#include "CMGlyphDrawing.h"

#include <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

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
	float currentRed, currentGreen, currentBlue;
	uint oldColor;

	int gWidth, gHeight;
	//CGContextRef gBackbuffer;
	Surface* gBackbuffer;
	Surface* gDrawTarget;
	MAHandle gDrawTargetHandle = HANDLE_SCREEN;
	
	unsigned char *gBackBufferData;
	
	double gTimeConversion;
	
	//static ResourceArray gResourceArray;

	EventQueue gEventQueue;
	static bool gEventOverflow	= false;
	static bool gClosing		= false;

	int gVolume = -1;

	bool exited = false;
	CRITICAL_SECTION exitMutex;
	
	static CGFontRef sUnicodeFont;
	
	void MALibQuit();

	//***************************************************************************
	// Resource loading
	//***************************************************************************
	enum ImageFormat {
		JPEG = 0,
		PNG = 1,
		UNKNOWN = 2
	};

	// this must be fixed in base.. too tired
	Surface* Syscall::loadImage(MemStream& s) {
		return loadImage((MemStreamC&)s);
	}
	
	Surface* Syscall::loadImage(MemStreamC& s) 
	{
		int len;
		TEST(s.length(len));
		const unsigned char *data = (const unsigned char*)s.ptrc();
		ImageFormat format;
#define E(x, y) (data[x]==y)
		if(len>3 && E(0, 0xff) && E(1, 0xd8)) {
			format = JPEG;
		}
		else if(len>7 && E(0, 0x89) && E(1, 0x50) && E(2, 0x4e) && E(3, 0x47) && E(4, 0x0d) && E(5, 0x0a) && E(6, 0x1a) && E(7, 0x0a) ) {
			format = PNG;
		}
		else {
			return NULL;
		}
		//CGDataProviderRef dpr = CGDataProviderCreateWithData(NULL, data, len ,NULL);
		CFDataRef png_data = CFDataCreate (NULL,data,len);
		CGDataProviderRef dpr  = CGDataProviderCreateWithCFData (png_data);
		
		CGImageRef imageRef;
		switch(format) {
			case JPEG: imageRef = CGImageCreateWithJPEGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
			case PNG: imageRef = CGImageCreateWithPNGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
		}
		   
		CGDataProviderRelease(dpr);
		
		/*
		Surface* newSurface = new Surface(CGImageGetWidth(imageRef), CGImageGetHeight(imageRef), NULL);
		CGContextSaveGState(newSurface->context);
		CGContextTranslateCTM(newSurface->context, 0, CGImageGetHeight(imageRef));
		CGContextScaleCTM(newSurface->context, 1, -1);
		CGContextDrawImage(newSurface->context, newSurface->rect, imageRef);
		CGContextRestoreGState(newSurface->context);
		CGImageRelease(imageRef);		
		
		return newSurface;
		*/
		
		return new Surface(imageRef);
	}
	
	Surface* Syscall::loadSprite(void* surface, ushort left, ushort top,
		ushort width, ushort height, ushort cx, ushort cy) 
	{
		return NULL;
	}
	
	//***************************************************************************
	// Helpers
	//***************************************************************************

	static bool MALibInit() {

		InitializeCriticalSection(&exitMutex);
		
		//gBackBufferData = new unsigned char[gWidth*4*gHeight];
		/*
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		gBackbuffer = 
		CGBitmapContextCreate(NULL, gWidth, gHeight, 8, gWidth*4, colorSpace, kCGImageAlphaNoneSkipFirst);
		CGColorSpaceRelease(colorSpace);
		//CGContextTranslateCTM(gBackbuffer, 0, gHeight);
		//CGContextScaleCTM(gBackbuffer, 1.0, -1.0);
		*/
		gBackbuffer = new Surface(gWidth, gHeight);
		CGContextRestoreGState(gBackbuffer->context);		
		CGContextTranslateCTM(gBackbuffer->context, 0, gHeight);
		CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
		CGContextSaveGState(gBackbuffer->context);
		
		// init font
		//CGContextSelectFont(gBackbuffer->context, "Arial", FONT_HEIGHT, kCGEncodingMacRoman);
		CFStringRef str = CFStringCreateWithCStringNoCopy(NULL, "Helvetica", kCFStringEncodingUTF8, NULL);
		sUnicodeFont = CGFontCreateWithFontName(str);
		CFRelease(str);
				
		gDrawTarget = gBackbuffer;
		
		mach_timebase_info_data_t machInfo;
		mach_timebase_info( &machInfo );
		gTimeConversion = 1e-6 * (double)machInfo.numer/(double)machInfo.denom;
		
		
		MANetworkInit();		
		
		return true;
	}

	void MALibQuit() {
		DeleteCriticalSection(&exitMutex);
		MANetworkClose();
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
		CGContextRestoreGState(gDrawTarget->context);
		CGContextSaveGState(gDrawTarget->context);
		CGContextClipToRect(gDrawTarget->context, CGRectMake(left, top, width, height));
		
		gDrawTarget->mImageDrawer->clipRect.x = left;
		gDrawTarget->mImageDrawer->clipRect.y = top;
		gDrawTarget->mImageDrawer->clipRect.width = width;
		gDrawTarget->mImageDrawer->clipRect.height = height;
		
	}

	SYSCALL(void, maGetClipRect(MARect *rect))
	{
		gSyscall->ValidateMemRange(rect, sizeof(MARect));		
		CGRect cr = CGContextGetClipBoundingBox(gDrawTarget->context);
		rect->left = cr.origin.x;
		rect->top = cr.origin.y;
		rect->width = cr.size.width;
		rect->height = cr.size.height;	
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
		currentRed =   (float)((argb&0x00ff0000)>>16)/255.0f;
		currentGreen = (float)((argb&0x0000ff00)>>8)/255.0f;
		currentBlue =  (float)((argb&0x000000ff))/255.0f;
		
		// hmmmm I don't know why I have to do this :)
		realColor = (argb&0xff00ff00)|((argb&0x00ff0000)>>16)|((argb&0x000000ff)<<16);
		
		return oldColor;
	}

	SYSCALL(void, maPlot(int posX, int posY)) {
		if(!gDrawTarget->data) DEBIG_PHAT_ERROR;
		/*
		CGRect cr = CGContextGetClipBoundingBox(gDrawTarget->context);
		if(posX<cr.origin.x || posY<cr.origin.y || posX>cr.size.width || posY>cr.size.height) return;
		int *p = (int*)&gDrawTarget->data[CGBitmapContextGetBytesPerRow(gDrawTarget->context)*posY+posX];
		*p = currentColor;
		*/
		gDrawTarget->mImageDrawer->drawPoint(posX, posY, realColor);
		   
	}

	SYSCALL(void, maLine(int x0, int y0, int x1, int y1)) {
		/*
		CGContextSetRGBStrokeColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);	
		CGContextBeginPath(gDrawTarget->context);
		CGContextMoveToPoint(gDrawTarget->context, x0, y0);
		CGContextAddLineToPoint(gDrawTarget->context, x1, y1);
		CGContextStrokePath(gDrawTarget->context);	
		*/
		gDrawTarget->mImageDrawer->drawLine(x0, y0, x1, y1, realColor);
	}

	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);			
		//CGContextFillRect(gDrawTarget->context, CGRectMake(left, top, width, height));
		gDrawTarget->mImageDrawer->drawFilledRect(left, top, width, height, realColor);
	}

	SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			gDrawTarget->mImageDrawer->drawTriangle(
											 points[i-2].x,
											 points[i-2].y,
											 points[i-1].x,
											 points[i-1].y,
											 points[i].x,
											 points[i].y,
											 realColor);
		}
	}
	
	SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
		CHECK_INT_ALIGNMENT(points);
		MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
		for(int i = 2; i < count; i++) {
			gDrawTarget->mImageDrawer->drawTriangle(
											 points[0].x,
											 points[0].y,
											 points[i-1].x,
											 points[i-1].y,
											 points[i].x,
											 points[i].y,
											 realColor);
		}
	}

	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);		
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		CGContextShowTextAtPoint(gDrawTarget->context, 0, 0, str, strlen(str));
		CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
		int width = after.x;
		return EXTENT(width, FONT_HEIGHT);
	}

	SYSCALL(MAExtent, maGetTextSizeW(const wchar_t* str)) {
		//NOT_IMPLEMENTED;
/*
		const char *s = unicodeToAscii(str);
		MAExtent e = maGetTextSize(s);
*/
		int numGlyphs = wcslen(str)*2;
		
		CGGlyph* glyphs = new CGGlyph[numGlyphs];
		CMFontGetGlyphsForUnichars(sUnicodeFont, (const UniChar*)str, glyphs, numGlyphs);
		
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);
		
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		//CGContextShowTextAtPoint(gDrawTarget->context, 0, 0, str, strlen(str));
		CGContextShowGlyphsAtPoint(gDrawTarget->context, 0, 0, glyphs, numGlyphs); 
		CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
		int width = after.x;
		delete glyphs;	
		return EXTENT(width, FONT_HEIGHT);
	}

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);					
		
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
		
		//CGAffineTransform xform =  CGAffineTransformMakeRotation(3.14159);
		//CGAffineTransform xform = CGAffineTransformScale(xform, 1, -1);
		//CGContextSetTextMatrix(gBackbuffer, xform);	
		
		//CGContextTranslateCTM(gDrawTarget->context, 0, gDrawTarget->height);
		//CGContextScaleCTM(gDrawTarget->context, 1, -1);
		CGContextShowTextAtPoint(gDrawTarget->context, left, top+FONT_HEIGHT, str, strlen(str));
		//CGContextTranslateCTM(gDrawTarget->context, 0, gDrawTarget->height);
		//CGContextScaleCTM(gDrawTarget->context, 1, -1);
	}

	SYSCALL(void, maDrawTextW(int left, int top, const wchar_t* str)) {
		//NOT_IMPLEMENTED;
		/*
		const char *s = unicodeToAscii(str);
		maDrawText(left, top, s);
		*/
		int numGlyphs = wcslen(str)*2;
		
		CGGlyph* glyphs = new CGGlyph[numGlyphs];
		CMFontGetGlyphsForUnichars(sUnicodeFont, (const UniChar*)str, glyphs, numGlyphs);
		
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);					
		
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
		
		//CGContextTranslateCTM(gDrawTarget->context, 0, gDrawTarget->height);
		//CGContextScaleCTM(gDrawTarget->context, 1, -1);
		CGContextShowGlyphsAtPoint(gDrawTarget->context, left, top+FONT_HEIGHT, glyphs, numGlyphs);
		//CGContextTranslateCTM(gDrawTarget->context, 0, gDrawTarget->height);
		//CGContextScaleCTM(gDrawTarget->context, 1, -1);		
		
		delete glyphs;
	}

	SYSCALL(void, maUpdateScreen()) {
		if(gClosing)
			return;
		//MAProcessEvents();
		//MAUpdateScreen();
		UpdateMoSyncView(gBackbuffer->image);
	}

	SYSCALL(void, maResetBacklight()) {
		// do nothing, it can't be reset as far as I can tell.. still a private api.
	}

	SYSCALL(MAExtent, maGetScrSize()) {
		//int width = CGImageGetWidth(gBackbuffer->image);
		//int height = CGImageGetHeight(gBackbuffer->image);
		return EXTENT(gWidth, gHeight);
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);	
		int width = CGImageGetWidth(img->image);
		int height = CGImageGetHeight(img->image);
		
		gDrawTarget->mImageDrawer->drawImage(left, top, img->mImageDrawer);	
		
		/*
		if(img->context) {
			CGContextDrawImage (gDrawTarget->context, CGRectMake(left, top, width, height), img->image);		
		} else {
			CGContextTranslateCTM(gDrawTarget->context, 0, height+top);
			CGContextScaleCTM(gDrawTarget->context, 1.0, -1.0);		
			CGContextDrawImage (gDrawTarget->context, CGRectMake(left, 0, width, height), img->image);	
			CGContextTranslateCTM(gDrawTarget->context, 0, height+top);	
			CGContextScaleCTM(gDrawTarget->context, 1.0, -1.0);		
		}
		 */
			
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
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);

		int imgWidth = CGImageGetWidth(img->image);
		int imgHeight = CGImageGetHeight(img->image);
		CGRect newRect = CGRectMake(dstTopLeft->x-src->left, 0, imgWidth, imgHeight);
		
		ClipRect srcRect;
		srcRect.x = src->left;
		srcRect.y = src->top;
		srcRect.width = src->width;
		srcRect.height = src->height;
		gDrawTarget->mImageDrawer->drawImageRegion(dstTopLeft->x, dstTopLeft->y, &srcRect, img->mImageDrawer, transformMode);
	}

	SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);
		return EXTENT(img->width, img->height);
	}

	SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);
		int x = src->left;
		int y = src->top;
		int width = src->width;
		int height = src->height;
		gSyscall->ValidateMemRange(dst, src->height*scanlength);

		CGRect smallRect = CGRectMake(x, y, width, height);
		CGImageRef smallImage = CGImageCreateWithImageInRect(img->image, smallRect);
		
		// First get the image into your data buffer
		int imgwidth = CGImageGetWidth(img->image);
		int imgheight = CGImageGetHeight(img->image);
		Surface *dstSurface = new Surface(imgwidth, imgheight, (char*) dst, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big, scanlength*4);

		CGContextDrawImage(dstSurface->context, CGRectMake(0, 0, width, height), smallImage);
		CGImageRelease(smallImage);
			
		delete dstSurface;
		
	}

	SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
		MAHandle temp = gDrawTargetHandle;
		if(gDrawTargetHandle != HANDLE_SCREEN) {
			SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
			ROOM(SYSCALL_THIS->resources.add_RT_IMAGE(gDrawTargetHandle, gDrawTarget));
			gDrawTargetHandle = HANDLE_SCREEN;
		}
		if(handle == HANDLE_SCREEN) {
			gDrawTarget = gBackbuffer;
		} else {
			Surface* img = SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
			if(!img->data)
				BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);
			   
			gDrawTarget = img; 
			ROOM(SYSCALL_THIS->resources.add_RT_FLUX(handle, NULL));
		}
		gDrawTargetHandle = handle;
		return temp;
	}

	SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size)) {
		MYASSERT(size>0, ERR_DATA_OOB);
		Stream* stream = gSyscall->resources.get_RT_BINARY(data);
		Surface *bitmap = 0;
		
		int len;
		TEST(stream->length(len));	
		
		if(offset<0 || offset+size>len)
			BIG_PHAT_ERROR(ERR_DATA_OOB);
		
		if(!stream->ptrc()) {
			// is not a memstream, read it to a buffer and load it.
			MYASSERT(stream->seek(Seek::Start, offset), ERR_DATA_OOB);
			unsigned char *data = new unsigned char[size];
			if(data==NULL) return RES_OUT_OF_MEMORY;
			MYASSERT(stream->read(data, size), ERR_DATA_ACCESS_FAILED);
			MemStreamC memStream(data, size);
			bitmap = gSyscall->loadImage(memStream);
			delete data;
		} else {
			const char *ptr = (const char*) stream->ptrc();
			MemStreamC memStream((const void*)&ptr[offset], size);
			bitmap = gSyscall->loadImage(memStream);
		}

		
		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
		
	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha)) {
		int width = EXTENT_X(size); int height = EXTENT_Y(size);
		gSyscall->ValidateMemRange(src, width*height*4);
		//NOT_IMPLEMENTED;
		int byteSize = EXTENT_X(size)*EXTENT_Y(size)*4;
		char *data = new char[byteSize];
		memcpy(data, src, byteSize);
		Surface *bitmap = new Surface(EXTENT_X(size), EXTENT_Y(size), data, processAlpha?kCGImageAlphaLast:kCGImageAlphaNoneSkipLast);
		bitmap->mOwnData = true;
		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
		return 1;
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		gSyscall->resources.add_RT_IMAGE(placeholder, new Surface(width, height));
		return 0;
	}


	SYSCALL(int, maGetKeys()) 
	{
		if(gClosing)
			return 0;
		//MAProcessEvents();
		//return currentKeyState;
		return 0; // there's no keys on iphone :)
	}
	

	SYSCALL(int, maGetEvent(MAEvent *dst)) 
	{
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));	
		MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		//MAProcessEvents();
		//NOT_IMPLEMENTED;

		if(!gClosing)
			gEventOverflow = false;
//		if(gEventQueue.count() == 0)
//			return 0;
		
		const MAEvent* ev = gEventQueue.getAndProcess();
		if(!ev) return 0;
		else *dst = *ev; //gEventQueue.get();

		/*
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
		/*
		if(gEventQueue.count() != 0)
			return;
		*/
		 
		gEventQueue.wait(timeout);
		
		//NOT_IMPLEMENTED;
	}

	SYSCALL(int, maTime()) 
	{
		return (int)time(NULL);
	}

	SYSCALL(int, maLocalTime()) 
	{
		time_t t = time(NULL);
		tm* lt = localtime(&t);
		return t + lt->tm_gmtoff;
	}
	
	SYSCALL(int, maGetMilliSecondCount()) 
	{
		//CFTimeInterval time = CFAbsoluteTimeGetCurrent();   
		//return (int)(time*1000.0);

		return (int)((double)mach_absolute_time()*gTimeConversion);
	}
	

	SYSCALL(int, maFreeObjectMemory()) {
		return getFreeAmountOfMemory();
	}
	SYSCALL(int, maTotalObjectMemory()) {
		return getTotalAmountOfMemory();
	}

	SYSCALL(int, maVibrate(int ms)) 
	{
		AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
		return 1;
	}

	SYSCALL(void, maPanic(int result, char* message)) 
	{		
		ShowMessageBox(message, true);
		GetVMYield(gCore) = 1;
		gRunning = false;
		pthread_exit(NULL);
	}

	SYSCALL(int, maPlatformRequest(const char* url)) 
	{
		if(!platformRequest(url)) return -1;
		return 0;
	}
	
	static AVAudioPlayer* sSoundPlayer = NULL;
	
	SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size)) 
	{ 
		Stream *res = gSyscall->resources.get_RT_BINARY(sound_res);
		MYASSERT(res->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
		Stream* src = res->createLimitedCopy(size);
		MYASSERT(src, ERR_DATA_ACCESS_FAILED);
		
		byte b;
		do {
			if(!src->readByte(b))
				BIG_PHAT_ERROR(ERR_MIME_READ_FAILED);
		} while(b);
		
		int pos;
		src->tell(pos);	
		
		NSData *data = NULL;
		
		int encodedSize = size - pos + offset;
		if(!src->ptrc()) {
			byte* sound = new byte[encodedSize];
			src->read(sound, encodedSize);
			data = [NSData dataWithBytesNoCopy:sound length:encodedSize];

		} else {
			byte* sound = &(((byte*)src->ptrc())[pos]);
			data = [NSData dataWithBytesNoCopy:sound length:encodedSize freeWhenDone:NO];
		}		
		
		if(sSoundPlayer)
			[sSoundPlayer release];	
		NSError *err;
		sSoundPlayer = [[AVAudioPlayer alloc] initWithData:data error:&err];
		[data release];
		if(!sSoundPlayer) {
			// check err
			return -1;
		}
		[sSoundPlayer play];
		
		return 0;
	}

	SYSCALL(void, maSoundStop()) 
	{
		[sSoundPlayer stop];
		sSoundPlayer.currentTime = 0;
	}

	SYSCALL(int, maSoundIsPlaying()) 
	{
		return sSoundPlayer.playing==YES;
	}

	SYSCALL(void, maSoundSetVolume(int vol)) 
	{
		sSoundPlayer.volume = (float)vol/100.0f;
	}

	SYSCALL(int, maSoundGetVolume()) 
	{
		return (int)(sSoundPlayer.volume*100.0f);
	}
	
	SYSCALL(int, maGetBatteryCharge()) 
	{
		float batLeft = [[UIDevice currentDevice] batteryLevel]; 		
		return (int)(batLeft*100.0f);
	}

	SYSCALL(int, maSendTextSMS(const char* dst, const char* msg)) {
		NOT_IMPLEMENTED;
		return 1;
	}

	SYSCALL(int, maInvokeExtension(int, int, int, int)) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}
	
	SYSCALL(int, maFrameBufferGetInfo(MAFrameBufferInfo *info)) {
		int bytesPerRow = CGBitmapContextGetBytesPerRow(gBackbuffer->context);
		int bitsPerPixel = CGBitmapContextGetBitsPerPixel(gBackbuffer->context);
		int bytesPerPixel = bytesPerRow/gWidth;
		int bitsPerComponent = CGBitmapContextGetBitsPerComponent(gBackbuffer->context);
		
		info->bitsPerPixel = bitsPerPixel;
		info->bytesPerPixel = bytesPerPixel;
/*
		info->redMask = 0x00ff0000;
		info->greenMask = 0x0000ff00;
		info->blueMask = 0x000000ff;
*/
		info->redMask = 0x000000ff;
		info->greenMask = 0x0000ff00;
		info->blueMask = 0x00ff0000;
		
		info->sizeInBytes = bytesPerRow*gHeight;
		info->width = gWidth;
		info->height = gHeight;
		info->pitch = bytesPerRow;
/*
		info->redShift = 16;
		info->greenShift = 8;
		info->blueShift = 0;
*/
		info->redShift = 0;
		info->greenShift = 8;
		info->blueShift = 16;
		
		info->redBits = bitsPerComponent;
		info->greenBits = bitsPerComponent;
		info->blueBits = bitsPerComponent;
		info->supportsGfxSyscalls = 1;
		
		return 1;
	}

	static Surface *sInternalBackBuffer = NULL;	
	SYSCALL(int, maFrameBufferInit(void *data)) {
		if(sInternalBackBuffer!=NULL) return 0;
		sInternalBackBuffer = gBackbuffer;
		//backBuffer = new Image((unsigned char*)data, NULL, backBuffer->width, backBuffer->height, backBuffer->pitch, backBuffer->pixelFormat, false, false);
		//currentDrawSurface = backBuffer;
		gBackbuffer = new Surface(gWidth, gHeight, (char*)data, kCGImageAlphaNoneSkipLast);
		gDrawTarget = gBackbuffer;
		return 1;
	}
	
	
	SYSCALL(int, maFrameBufferClose()) {
		if(sInternalBackBuffer==NULL) return 0;
		delete gBackbuffer;
		gBackbuffer = sInternalBackBuffer;
		sInternalBackBuffer = NULL;
		gDrawTarget = gBackbuffer;
		return 1;
	}
	
	int maLocationStart() {
		StartUpdatingLocation();
		return MA_LPS_AVAILABLE;
	}
	
	int maLocationStop() {
		StopUpdatingLocation();
		return 0;
	}

	SYSCALL(int, maIOCtl(int function, int a, int b, int c)) 
	{
		switch(function) {

		case maIOCtl_maCheckInterfaceVersion:
			return Base::maCheckInterfaceVersion(a);

		case maIOCtl_maWriteLog:
			LOGBIN(gSyscall->GetValidatedMemRange(a, b), b);
			return 0;
		case maIOCtl_maPlatformRequest:
			return maPlatformRequest(SYSCALL_THIS->GetValidatedStr(a));				
		case maIOCtl_maGetBatteryCharge:
			return maGetBatteryCharge();		

		case maIOCtl_maLocationStart:
			return maLocationStart();
		case maIOCtl_maLocationStop:
			return maLocationStop();				
			
		case maIOCtl_maFrameBufferGetInfo:
			return maFrameBufferGetInfo(GVMRA(MAFrameBufferInfo));
		case maIOCtl_maFrameBufferInit:
			return maFrameBufferInit(GVMRA(void*));		
		case maIOCtl_maFrameBufferClose:
			return maFrameBufferClose();
				
		}
		
		return IOCTL_UNAVAILABLE;
	}
} // namespace Base

void EventQueue::handleInternalEvent(int type, void *e) {
	if(type == IEVENT_TYPE_DEFLUX_BINARY) {
		InternalEventDefluxBin *d = (InternalEventDefluxBin*)e;
		SYSCALL_THIS->resources.extract_RT_FLUX(d->handle);
		ROOM(SYSCALL_THIS->resources.dadd_RT_BINARY(d->handle, d->stream));	
		delete d;
	}
}

void MoSyncExit(int r) 
{
	LOG("MoSyncExit(%d)\n", r);

	EnterCriticalSection(&exitMutex);
	if(!exited) {
		exited = true;
		LeaveCriticalSection(&exitMutex);
		//exit(r);
		Exit();
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
	
	GetVMYield(gCore) = 1;
	gRunning = false;
	ShowMessageBox(buffer, true);	
	//MoSyncExit(errorCode);
}
	