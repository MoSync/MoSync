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

#define MA_PROF_SUPPORT_LOCATIONAPI
#define MA_PROF_SUPPORT_WIDGETAPI

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
#include "iphone_helpers.h"

#include "ThreadPoolImpl.h"

#include <base/Image.h>

#include "PimSyscall.h"
#include "helpers/cpp_ix_pim.h"

#define VSV_ARGPTR_NAME argptr
#define VSV_ARGPTR_DECL , va_list VSV_ARGPTR_NAME
#define VSV_ARGPTR_USE , VSV_ARGPTR_NAME

namespace Core {
	class VMCore;
}

extern Core::VMCore* gCore;
extern bool gRunning;

class Surface {
public:

	Surface(CGImageRef image) : image(image), context(NULL), data(NULL), mOwnData(false), orientation(1) {
		CGDataProviderRef dpr = CGImageGetDataProvider(image);
		mDataRef = CGDataProviderCopyData(dpr);
		
		this->data = (char *)CFDataGetBytePtr(mDataRef);		
		width = CGImageGetWidth(image);
		height = CGImageGetHeight(image);
		rowBytes = CGImageGetBytesPerRow(image);
		rect = CGRectMake(0, 0, width, height);
		mOwnData = false;
		orientation = 1;

		bool noAlpha = false;
		int bpp = CGImageGetBitsPerPixel(image);

		CGBitmapInfo bInfo = CGImageGetBitmapInfo(image);
		if(bpp==32 && ((bInfo&kCGBitmapAlphaInfoMask)==kCGImageAlphaNoneSkipLast || (bInfo&kCGBitmapAlphaInfoMask)==kCGImageAlphaNoneSkipFirst))
		{
			bpp = 24;
		}

		if(bpp != 32) {
			CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
			this->data = new char[width*height*4];
			mOwnData = true;
			rowBytes = width*4;
			context = CGBitmapContextCreate(this->data, width, height, 8, rowBytes, colorSpace, kCGImageAlphaNoneSkipLast);
			CGContextSetAllowsAntialiasing(context, false);
			CGContextSetBlendMode(context, kCGBlendModeCopy);
			CGContextDrawImage(context, rect, image);
			CGColorSpaceRelease(colorSpace);
			noAlpha = true;
			for(int i = 0; i < height; i++) {
				for(int j = 0; j < width; j++) {
					int* iptr = (int*)&this->data[j*4 + i*rowBytes];
					*iptr |= 0xff000000;
				}
			}
		}
		
		createImageDrawer();
		
		if((bInfo&kCGBitmapByteOrderMask)==kCGBitmapByteOrder32Host) {
			for(int i = 0; i < height; i++) {
				for(int j = 0; j < width; j++) {
					int* iptr = (int*)&this->data[j*4 + i*rowBytes];
					int c = *iptr;
					*iptr = (c&0xff00ff00) | ((c>>16)&0xff) | ((c&0xff)<<16);
				}
			}
		}
        
		if(noAlpha) {
			mImageDrawer->alphaMask = 0;
			mImageDrawer->alphaBits = 0;	
		}
	}
	
	void createImageDrawer() {
		mImageDrawer = new Image((unsigned char*)data, NULL, width, height, rowBytes, Image::PIXELFORMAT_ARGB8888, false, false);
	}
	
	void initFont() {
		if(!context) return;
		CGContextSelectFont(context, "Arial", height/40, kCGEncodingMacRoman);
		CGAffineTransform xform = CGAffineTransformMake(
														1.0,  0.0,
														0.0, -1.0,
														0.0,  0.0);
		CGContextSetTextMatrix(context, xform);	
	}

	Surface(int width, int height, char *data=NULL, CGBitmapInfo bitmapInfo=kCGImageAlphaNoneSkipLast, int rowBytes=-1) : mDataRef(NULL), orientation(1) {
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		this->width = width,
		this->height = height;
		orientation = 1;

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
		
		CGContextSetAllowsAntialiasing (context, false);
		
		rect = CGRectMake(0, 0, width, height);

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
		if(mDataRef) CFRelease(mDataRef);
	}
	
	int width, height, rowBytes, orientation;
	CGImageRef image;
	CGContextRef context;
	CGRect rect;
	bool mOwnData;
	char *data;
	CFDataRef mDataRef;
	
	Image *mImageDrawer;
};


// never use <0 for event type, just internal events :)
#define IEVENT_TYPE_DEFLUX_BINARY -1

// Space left for other events.
#define QUEUE_EVENT_FREE_SPACE 20

class EventQueue : public CircularFifo<MAEvent, EVENT_BUFFER_SIZE> {
public:
	EventQueue() : CircularFifo<MAEvent, EVENT_BUFFER_SIZE>(), mEventOverflow(false), mWaiting(false) {
		pthread_cond_init(&mCond, NULL);
		pthread_mutex_init(&mMutex, NULL);
	}
	
	~EventQueue() {
		pthread_cond_destroy(&mCond);
		pthread_mutex_destroy(&mMutex);
	}
	
	void handleInternalEvent(int type, void *e);

	bool getAndProcess(MAEvent& event) {
		if(count()==0) return false;
		MAEvent e = CircularFifo<MAEvent, EVENT_BUFFER_SIZE>::get();
		if(e.type<0) {
			handleInternalEvent(e.type, (void*)e.data);
			return getAndProcess(event);
		} else {
			event = e;
			return true;
		}
	}

	void put(const MAEvent& e) {
            CircularFifo<MAEvent, EVENT_BUFFER_SIZE>::put(e);

            pthread_mutex_lock(&mMutex);
            pthread_cond_signal(&mCond);
            pthread_mutex_unlock(&mMutex);
	}

    /**
     * Put event in queue only if there is free space.
     * The event is ignored if there isn't enough free space.
     * This method does not throw panic.
     */
    void putSafe(const MAEvent& e)
    {
        if (count() + QUEUE_EVENT_FREE_SPACE < EVENT_BUFFER_SIZE)
        {
            CircularFifo<MAEvent, EVENT_BUFFER_SIZE>::put(e);
            pthread_mutex_lock(&mMutex);
            pthread_cond_signal(&mCond);
            pthread_mutex_unlock(&mMutex);
        }
	}

	void wait(int ms) {
		pthread_mutex_lock(&mMutex);
		if(count()==0) {
			if(ms>0) {
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

	void addPointerEvent(int x, int y, int touchId, int type) {
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
			event.touchId = touchId;
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
		event.data = (int)data;
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
    extern bool gClosing;
    extern bool gEventOverflow;
}

#endif
