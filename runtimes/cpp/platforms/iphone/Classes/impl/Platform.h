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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//#include <windows.h>
#import <CoreGraphics/CoreGraphics.h>
#include <string>
#include <mach/mach_time.h>
#include <time.h>
#include <sys/time.h>

#include <vector>
#include <string>

#include "config_platform.h"

#include <bluetooth/discovery.h>
#include <helpers/fifo.h>

#include "ThreadPoolImpl.h"

#include <base/Image.h>

namespace Core {
	class VMCore;
}
extern Core::VMCore* gCore;
extern bool gRunning;

#define FONT_HEIGHT 12

class Surface {
public:
	Surface(CGImageRef image) : image(image), context(NULL), data(NULL), mOwnData(false) {				
		CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(image));
		this->data = (char *)CFDataGetBytePtr(data);		
		width = CGImageGetWidth(image);
		height = CGImageGetHeight(image);
		rowBytes = CGImageGetBytesPerRow(image);
		rect = CGRectMake(0, 0, width, height);
		mOwnData = true;

		bool noAlpha = false;
		int bpp = CGImageGetBitsPerPixel(image);
		CGBitmapInfo bInfo = CGImageGetBitmapInfo(image);
	

		if(bpp != 32) {
			CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
			this->data = new char[width*height*4];
			rowBytes = width*4;
			context = CGBitmapContextCreate(this->data, width, height, 8, rowBytes, colorSpace, kCGImageAlphaNoneSkipLast);
			CGContextSetAllowsAntialiasing(context, false);
			CGContextSetBlendMode(context, kCGBlendModeCopy);
		
			//CGContextSaveGState(context);
			//CGContextTranslateCTM(context, 0, height);
			//CGContextScaleCTM(context, 1.0, -1.0);
			CGContextDrawImage(context, rect, image);
			//CGContextRestoreGState(context);
			
			CGColorSpaceRelease(colorSpace);
			
			//CGContextSaveGState(context);
			
			noAlpha = true;
		}
		
		createImageDrawer();
		
		if((bInfo&kCGBitmapByteOrderMask)==kCGBitmapByteOrder32Host) {
			mImageDrawer->redMask = 0x000000ff;
			mImageDrawer->redShift = 0;		
			mImageDrawer->greenMask = 0x00ff00;
			mImageDrawer->greenShift = 8;		
			mImageDrawer->blueMask = 0xff0000;
			mImageDrawer->blueShift = 16;		
			mImageDrawer->alphaMask = 0xff000000;
			mImageDrawer->alphaShift = 24;			
		}

		if(noAlpha) {
			mImageDrawer->alphaMask = 0;
			mImageDrawer->alphaBits = 0;	
		}
	}
	
	void createImageDrawer() {
		//CGBitmapInfo bInfo = CGImageGetBitmapInfo(image);
		mImageDrawer = new Image((unsigned char*)data, NULL, width, height, rowBytes, Image::PIXELFORMAT_ARGB8888, false, false);
	
		/*
		mImageDrawer->redMask = 0xff000000;
		mImageDrawer->redShift = 24;		
		mImageDrawer->greenMask = 0x00ff0000;
		mImageDrawer->greenShift = 16;		
		mImageDrawer->blueMask = 0x0000ff00;
		mImageDrawer->blueShift = 8;		
		mImageDrawer->alphaMask = 0x000000ff;
		mImageDrawer->alphaShift = 0;		
		 */
	}
	
	void initFont() {
		if(!context) return;
		CGContextSelectFont(context, "Arial", FONT_HEIGHT, kCGEncodingMacRoman);

		CGContextSetFontSize(context, FONT_HEIGHT);
		CGAffineTransform xform = CGAffineTransformMake(
														1.0,  0.0,
														0.0, -1.0,
														0.0,  0.0);
		CGContextSetTextMatrix(context, xform);	
	}

	Surface(int width, int height, char *data=NULL, CGBitmapInfo bitmapInfo=kCGImageAlphaNoneSkipLast, int rowBytes=-1) {
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		this->width = width,
		this->height = height;
		if(rowBytes==-1)
			this->rowBytes = rowBytes = width*4;
		else
			this->rowBytes = rowBytes;

		if(data==NULL) {
			this->data = data = new char[rowBytes*height];
			mOwnData = true;
		}
		else {
			this->data = data;
			mOwnData = false;
		}
		
		context = CGBitmapContextCreate(this->data, width, height, 8, rowBytes, colorSpace, bitmapInfo);
		
		CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, this->data, rowBytes * height, NULL);
		image = CGImageCreate(width, height, 8, 32, rowBytes, colorSpace, bitmapInfo, 
							  dataProvider, NULL, false, kCGRenderingIntentDefault);
		CGDataProviderRelease(dataProvider);
		CGColorSpaceRelease(colorSpace);
		
		//CGContextTranslateCTM(context, 0, height);
		//CGContextScaleCTM(context, 1.0, -1.0);	
		CGContextSetAllowsAntialiasing (context, false);
		//CGContextSetInterpolationQuality(context, kCGInterpolationNone);
		
		rect = CGRectMake(0, 0, width, height);
		
		//CGContextSetRGBFillColor(context, 0, 0, 0, 1);
		//CGContextFillRect(context, rect);	
		//CGContextClearRect(context, CGRectMake(0, 0, width, height));
		
		initFont();
		
		CGContextSaveGState(context);
		
		createImageDrawer();	
		
		if(bitmapInfo==kCGImageAlphaNoneSkipLast) {
			mImageDrawer->alphaMask = 0;
			mImageDrawer->alphaBits = 0;		
		}
	}
	
	
	~Surface() {
		if(image) CGImageRelease(image);
		if(context) CGContextRelease(context);
		if(mOwnData && data) delete data;
		if(mImageDrawer) delete mImageDrawer;
	}
	
	int width, height, rowBytes;
	CGImageRef image;
	CGContextRef context;
	CGRect rect;
	bool mOwnData;
	char *data;
	
	Image *mImageDrawer;
};


// never use <0 for event type :)
#define IEVENT_TYPE_DEFLUX_BINARY -1

class EventQueue : public CircularFifo<MAEvent, EVENT_BUFFER_SIZE> {
public:
	EventQueue() : mEventOverflow(false), mWaiting(false) {
		pthread_cond_init(&mCond, NULL);
		pthread_mutex_init(&mMutex, NULL);
	}
	
	~EventQueue() {
		pthread_cond_destroy(&mCond);
		pthread_mutex_destroy(&mMutex);
	}
	
	void handleInternalEvent(int type, void *e);

	const MAEvent* getAndProcess() {
		if(count()==0) return NULL;
		const MAEvent& e = CircularFifo<MAEvent, EVENT_BUFFER_SIZE>::get();
		if(e.type<0) {
			handleInternalEvent(e.type, e.data);
			return getAndProcess();
		} else {
			return &e;
		}
	}
	
	
	void put(const MAEvent& e) {
		CircularFifo<MAEvent, EVENT_BUFFER_SIZE>::put(e);
		//pthread_mutex_lock(&mMutex);	
		pthread_cond_signal(&mCond);
		//pthread_mutex_unlock(&mMutex);
	}
		
	void wait(int ms) {
		pthread_mutex_lock(&mMutex);
		if(count()==0) {
			if(ms!=0) {
				struct timeval now;
				struct timespec timeout;	
				gettimeofday(&now, NULL);
				timeout.tv_sec = now.tv_sec + (ms/1000);
				timeout.tv_nsec = now.tv_usec * 1000 + (ms%1000)*1000000;				
				pthread_cond_timedwait(&mCond, &mMutex, &timeout);
			} else {
				pthread_cond_wait(&mCond, &mMutex);
			}
		}
		pthread_mutex_unlock(&mMutex);	
	}
		
	void addPointerEvent(int x, int y, int type) {
		if(!mEventOverflow) {
			if(count() + 2 == EVENT_BUFFER_SIZE) {	//leave space for Close event
				mEventOverflow = true;
				clear();
				LOG("EventBuffer overflow!\n");
			}
			/* put event in event queue */
			MAEvent event;
			event.type = type;
			event.point.x = x;
			event.point.y = y;
			put(event);
		}		
	}
	
	void addScreenChangedEvent() {
		MAEvent event;
		event.type = EVENT_TYPE_SCREEN_CHANGED;
		put(event);	
	}
	
	void addCloseEvent() {
		MAEvent event;
		event.type = EVENT_TYPE_CLOSE;
		put(event);	
	}
	
	void addInternalEvent(int type, void* data) {
		MAEvent event;
		event.type = type;
		event.data = data;
		put(event);
	}
	
private:
	pthread_mutex_t mMutex;
	pthread_cond_t mCond;
							
	bool mEventOverflow;
	bool mWaiting;
	
};

namespace Base
{
	extern EventQueue gEventQueue;
}

#endif
