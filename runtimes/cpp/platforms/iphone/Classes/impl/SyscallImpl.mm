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

#include "config_platform.h"
#import <UIKit/UIKit.h>
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
//#include <helpers/CPP_IX_ACCELEROMETER.h>

#include <helpers/CPP_IX_WIDGET.h>
#include "MoSyncUISyscalls.h"

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

#ifdef SUPPORT_OPENGL_ES
#include <helpers/CPP_IX_OPENGL_ES.h>
#include <OpenGLES/ES1/gl.h>
#include "../../../../generated/gl.h.cpp"
#endif

extern ThreadPool gThreadPool;

#define NOT_IMPLEMENTED BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED)

int Surface::fontSize;

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

	Surface* gBackbuffer;
	Surface* gDrawTarget;
	MAHandle gDrawTargetHandle = HANDLE_SCREEN;
	
	unsigned char *gBackBufferData;
	
	mach_timebase_info_data_t gTimeBase;
	uint64_t gTimeStart;
	
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

		CFDataRef png_data = CFDataCreate (NULL,data,len);
		CGDataProviderRef dpr  = CGDataProviderCreateWithCFData (png_data);
		
		CGImageRef imageRef;
		switch(format) {
			case JPEG: imageRef = CGImageCreateWithJPEGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
			case PNG: imageRef = CGImageCreateWithPNGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
		}
		   
		CGDataProviderRelease(dpr);
				
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
		
		Surface::fontSize = gHeight/40;
		gBackbuffer = new Surface(gWidth, gHeight);
		CGContextRestoreGState(gBackbuffer->context);		
		CGContextTranslateCTM(gBackbuffer->context, 0, gHeight);
		CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
		CGContextSaveGState(gBackbuffer->context);
		
		// init font
		CFStringRef str = CFStringCreateWithCStringNoCopy(NULL, "Helvetica", kCFStringEncodingUTF8, NULL);
		sUnicodeFont = CGFontCreateWithFontName(str);
		//CFRelease(str);
		
		gDrawTarget = gBackbuffer;
		
		mach_timebase_info( &gTimeBase );
		//gTimeConversion = 1e-6 * (double)machInfo.numer/(double)machInfo.denom;
		gTimeStart = mach_absolute_time();
		
		
		MANetworkInit();		
		
		// init some image.h optimizations.
		initMulTable();
		initRecipLut();
		
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
		init();
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
		rect->left = gDrawTarget->mImageDrawer->clipRect.x;
		rect->top = gDrawTarget->mImageDrawer->clipRect.y; 
		rect->width = gDrawTarget->mImageDrawer->clipRect.width;
		rect->height = gDrawTarget->mImageDrawer->clipRect.height;

		/*
		CGRect cr = CGContextGetClipBoundingBox(gDrawTarget->context);
		rect->left = cr.origin.x;
		rect->top = cr.origin.y;
		rect->width = cr.size.width;
		rect->height = cr.size.height;
		*/
	}

	SYSCALL(int, maSetColor(int argb)) {
		oldColor = currentColor;
		currentColor = argb;
		float red =   (float)((argb&0x00ff0000)>>16)/255.0f;
		float green = (float)((argb&0x0000ff00)>>8)/255.0f;
		float blue =  (float)((argb&0x000000ff))/255.0f;
		currentRed = red;
		currentGreen = green;
		currentBlue = blue;
		
		// hmmmm I don't know why I have to do this :)
		realColor = (argb&0xff00ff00)|((argb&0x00ff0000)>>16)|((argb&0x000000ff)<<16);
		
		return oldColor;
	}

	SYSCALL(void, maPlot(int posX, int posY)) {
		if(!gDrawTarget->data) DEBIG_PHAT_ERROR;
		gDrawTarget->mImageDrawer->drawPoint(posX, posY, realColor);
		   
	}

	SYSCALL(void, maLine(int x0, int y0, int x1, int y1)) {
		gDrawTarget->mImageDrawer->drawLine(x0, y0, x1, y1, realColor);
	}

	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		//CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);			
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

	int stringLength(const wchar_t* str) {
		int len = 0;
		while(*str++ != 0) {
			if(*str == '\n') return len;
			len++;
		}
		return len;
	}
	
	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);		
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		CGContextShowTextAtPoint(gDrawTarget->context, 0, 0, str, strlen(str));
		CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
		int width = after.x;
		int height = FONT_HEIGHT;
		return EXTENT(width, height);
	}

	SYSCALL(MAExtent, maGetTextSizeW(const wchar_t* str)) {
		int numGlyphs = wcslen(str)*2;
		if(numGlyphs==0) return EXTENT(0, 0);		
		CGGlyph* glyphs = new CGGlyph[numGlyphs];
		CMFontGetGlyphsForUnichars(sUnicodeFont, (const UniChar*)str, glyphs, numGlyphs);
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		CGContextShowGlyphsAtPoint(gDrawTarget->context, 0, 0, glyphs, numGlyphs); 
		CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
		int width = after.x;
		delete glyphs;	
		return EXTENT(width, FONT_HEIGHT);
	}

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);	
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);		
		CGContextShowTextAtPoint(gDrawTarget->context, left, top+FONT_HEIGHT, str, strlen(str));
	}

	SYSCALL(void, maDrawTextW(int left, int top, const wchar_t* str)) {
		int numGlyphs = wcslen(str)*2;		
		if(numGlyphs==0) return;
		CGGlyph* glyphs = new CGGlyph[numGlyphs];
		CMFontGetGlyphsForUnichars(sUnicodeFont, (const UniChar*)str, glyphs, numGlyphs);
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);	
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);		
		CGContextShowGlyphsAtPoint(gDrawTarget->context, left, top+FONT_HEIGHT, glyphs, numGlyphs);		
		delete glyphs;
	}

	SYSCALL(void, maUpdateScreen()) {
		if(gClosing)
			return;
		MoSync_UpdateView(gBackbuffer->image);
	}

	SYSCALL(void, maResetBacklight()) {
		// do nothing, it can't be reset as far as I can tell.. still a private api.
	}

	SYSCALL(MAExtent, maGetScrSize()) {
		return EXTENT(gWidth, gHeight);
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);	
		gDrawTarget->mImageDrawer->drawImage(left, top, img->mImageDrawer);				
	}
	
	void flipColorsFromAxGyToAyGx(int *buf, int width, int height, int scanlength) {
		int *ptr = (int*)buf;
		for(int j = 0; j < height; j++) {
			for(int i = 0; i < width; i++) {
				int abgr = *ptr;
				*ptr = (abgr&0xff00ff00)|((abgr&0x00ff0000)>>16)|((abgr&0x000000ff)<<16);
				ptr++;
			}
			ptr+=-width+scanlength;
		}		
	}	

	SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect,
		int scanlength)) {
		gSyscall->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(srcRect, sizeof(MARect));
		gSyscall->ValidateMemRange(src, scanlength*srcRect->height*4);
		
		Surface *srcSurface = new 
		Surface(srcRect->width, srcRect->height, (char*)src, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big, scanlength*4);
		
		ClipRect srcRectCR;
		srcRectCR.x = srcRect->left;
		srcRectCR.y = srcRect->top;
		srcRectCR.width = srcRect->width;
		srcRectCR.height = srcRect->height;
		gDrawTarget->mImageDrawer->drawImageRegion(dstPoint->x, dstPoint->y, &srcRectCR, srcSurface->mImageDrawer, 0);
		delete srcSurface;
		
		flipColorsFromAxGyToAyGx(&((int*)gDrawTarget->data)[srcRect->left+srcRect->top*(gDrawTarget->rowBytes>>2)], srcRect->width, srcRect->height, gDrawTarget->rowBytes>>2);
	}

	SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft,
		int transformMode))
	{
		gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(src, sizeof(MARect));	
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);

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
		//int imgwidth = CGImageGetWidth(img->image);
		//int imgheight = CGImageGetHeight(img->image);
		//memset(dst, 0, scanlength*height*4);
		
		Surface *srcSurface = new Surface(smallImage);
		
		//Surface *dstSurface = new Surface(imgwidth, imgheight, (char*) dst, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big, scanlength*4);

		int* dstptr = (int*)dst;
		int* srcptr = (int*)srcSurface->data;
		for(int i = 0; i < height; i++) {
			memcpy(dstptr, srcptr, width*4); 
			srcptr+=width;
			dstptr+=scanlength;
	   }
		
		//CGContextDrawImage(dstSurface->context, CGRectMake(0, 0, width, height), smallImage);
		//CGImageRelease(smallImage);
		
		flipColorsFromAxGyToAyGx((int*)dst, width, height, scanlength);
		delete srcSurface;
		//delete dstSurface;
		
		
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
		
		if(!bitmap) return RES_BAD_INPUT;
		if(!bitmap->image) {
			delete bitmap;
			// most likely bad input.
			return RES_BAD_INPUT;
		}
		
		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
		
	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha)) {
		int width = EXTENT_X(size); int height = EXTENT_Y(size);
		gSyscall->ValidateMemRange(src, width*height*4);
		int byteSize = EXTENT_X(size)*EXTENT_Y(size)*4;
		char *data = new char[byteSize];
		if(!data) return RES_OUT_OF_MEMORY;
		memcpy(data, src, byteSize);
		Surface *bitmap = new Surface(EXTENT_X(size), EXTENT_Y(size), data, processAlpha?kCGImageAlphaPremultipliedLast:kCGImageAlphaNoneSkipLast);
		if(!bitmap) {
			delete data;
			return RES_OUT_OF_MEMORY;
		}
		
		if(!bitmap->context || !bitmap->image || !bitmap->data) {
			delete bitmap;
			delete data;
			return RES_OUT_OF_MEMORY;
		}
		
		
		bitmap->mOwnData = true;
		flipColorsFromAxGyToAyGx((int*)data, EXTENT_X(size), EXTENT_Y(size), EXTENT_X(size));
		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		Surface *surf = new Surface(width, height);
		if(!surf) return RES_OUT_OF_MEMORY;
		if(!surf->context || !surf->image || !surf->data) {
			delete surf;
			return RES_OUT_OF_MEMORY;			
		}
			
		return gSyscall->resources.add_RT_IMAGE(placeholder, surf);
	}


	SYSCALL(int, maGetKeys()) 
	{
		if(gClosing)
			return 0;
		return 0; // there's no keys on iphone :)
	}
	

	SYSCALL(int, maGetEvent(MAEvent *dst)) 
	{
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));	
		MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		if(!gClosing)
			gEventOverflow = false;
		
		const MAEvent* ev = gEventQueue.getAndProcess();
		if(!ev) return 0;
		else *dst = *ev; //gEventQueue.get();
		
#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(ev->type == eventType) {\
		memcpy(MoSync_GetCustomEventData(), ev->data, sizeof(dataType));\
		delete (dataType*)ev->data;\
		dst->data = MoSync_GetCustomEventDataMoSyncPointer(); }
		
		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);
		
		return 1;
	}

	SYSCALL(void, maWait(int timeout)) 
	{
		if(gClosing)
			return;
		gEventQueue.wait(timeout);
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
		//int time = (int)(CACurrentMediaTime()*1000.0);
		//int time = (int)(CFAbsoluteTimeGetCurrent()*1000.0f);
		//int time = (int)((double)mach_absolute_time()*gTimeConversion);
		int time = (((mach_absolute_time() - gTimeStart) * gTimeBase.numer) / gTimeBase.denom) / 1000000;
		
		
		return time;
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
		MoSync_ShowMessageBox(message, true);
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
		
		if(sSoundPlayer) {
			if(sSoundPlayer.playing==YES)
				[sSoundPlayer stop];
			[sSoundPlayer release];	
		}
		
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

		//CGBitmapInfo bInfo = CGImageGetBitmapInfo(gBackbuffer->image);
		
		info->redMask = 0x00ff0000;
		info->greenMask = 0x0000ff00;
		info->blueMask = 0x000000ff;
		info->redShift = 16;
		info->greenShift = 8;
		info->blueShift = 0;				
		
		info->sizeInBytes = bytesPerRow*gHeight;
		info->width = gWidth;
		info->height = gHeight;
		info->pitch = bytesPerRow;

		info->redBits = bitsPerComponent;
		info->greenBits = bitsPerComponent;
		info->blueBits = bitsPerComponent;
		info->supportsGfxSyscalls = 1;
		
		return 1;
	}

	static Surface *sInternalBackBuffer = NULL;	
	SYSCALL(int, maFrameBufferInit(const void *data)) {
		if(sInternalBackBuffer!=NULL) return 0;
		sInternalBackBuffer = gBackbuffer;

		gBackbuffer = new Surface(gWidth, gHeight, (char*)data, kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little);
		CGContextRestoreGState(gBackbuffer->context);		
		CGContextTranslateCTM(gBackbuffer->context, 0, gHeight);
		CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
		CGContextSaveGState(gBackbuffer->context);
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
		MoSync_StartUpdatingLocation();
		return MA_LPS_AVAILABLE;
	}
	
	int maLocationStop() {
		MoSync_StopUpdatingLocation();
		return 0;
	}
	
	int maAccelerometerStart() {
		MoSync_StartUpdatingAccelerometer();
		return 0;
	}
	
	int maAccelerometerStop() {
		MoSync_StopUpdatingAccelerometer();
		return 0;
	}	
	
	int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints) {
		MoSync_ShowTextBox(title, inText, outText, maxSize, constraints);
		return 0;
	}
	
	int maGetSystemProperty(const char *key, char *buf, int size) {
		int res = -1;
		if(strcmp(key, "mosync.iso-639-1")==0) {
			// I don't know if this works perfectly (in the documentation it 
			// says that it will return iso-639-x, but it looks like iso-639-1)
			CFLocaleRef userLocaleRef = CFLocaleCopyCurrent(); 
			CFStringRef str = (CFStringRef)CFLocaleGetValue(userLocaleRef, kCFLocaleLanguageCode);			
			res = CFStringGetLength(str);
			CFStringGetCString(str, buf, size, kCFStringEncodingUTF8);
			CFRelease(str);
			CFRelease(userLocaleRef);
		}
		
		return res;
	}	

#ifdef SUPPORT_OPENGL_ES
	int maOpenGLInitFullscreen() {
		return 0;
	}
	
	int maOpenGLCloseFullscreen() {
		return 0;
	}
	
	int maOpenGLTexImage2D(MAHandle image) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);	
	
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, img->data);
		
		return MA_GL_TEX_IMAGE_2D_OK;
	}
#endif	//SUPPORT_OPENGL_ES	
	
	SYSCALL(int, maIOCtl(int function, int a, int b, int c)) 
	{
		switch(function) {

		case maIOCtl_maWriteLog:
		{
			const char *str = (const char*) gSyscall->GetValidatedMemRange(a, b);
			LOGBIN(str, b);
			logWithNSLog(str, b);
			return 0;
		}
		maIOCtl_case(maPlatformRequest);
		maIOCtl_case(maGetBatteryCharge);
		maIOCtl_case(maLocationStart);
		maIOCtl_case(maLocationStop);
		maIOCtl_case(maFrameBufferGetInfo);
		maIOCtl_case(maFrameBufferInit);
		maIOCtl_case(maFrameBufferClose);				
		maIOCtl_syscall_case(maFileOpen);
		maIOCtl_syscall_case(maFileWriteFromData);
		maIOCtl_syscall_case(maFileReadToData);
		maIOCtl_syscall_case(maFileTell);
		maIOCtl_syscall_case(maFileSeek);
		maIOCtl_syscall_case(maFileRead);
		maIOCtl_syscall_case(maFileWrite);
		maIOCtl_syscall_case(maFileExists);
		maIOCtl_syscall_case(maFileClose);
		maIOCtl_syscall_case(maFileCreate);
		maIOCtl_syscall_case(maFileDelete);
		maIOCtl_syscall_case(maFileSize);
		maIOCtl_case(maTextBox);		
		maIOCtl_case(maGetSystemProperty);
		maIOCtl_IX_WIDGET_caselist
#ifdef SUPPORT_OPENGL_ES
				maIOCtl_IX_OPENGL_ES_caselist;
#endif	//SUPPORT_OPENGL_ES

//		maIOCtl_case(maAccelerometerStart);
//		maIOCtl_case(maAccelerometerStop);
		}
		
		return IOCTL_UNAVAILABLE;
	}
	
	SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
		NOT_IMPLEMENTED;
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
		MoSync_Exit();
		EnterCriticalSection(&exitMutex);
	}
	LeaveCriticalSection(&exitMutex);
	pthread_exit(NULL);
}

void MoSyncErrorExit(int errorCode) 
{
	LOG("ErrorExit %i\n", errorCode);
	char buffer[256];
	//char* ptr = buffer + 
	sprintf(buffer, "MoSync Panic\np%i.", errorCode);
#if 0
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
#endif
	
	gRunning = false;
	logWithNSLog(buffer, strlen(buffer));
	MoSync_ShowMessageBox(buffer, true);	
	pthread_exit(NULL);
}
	