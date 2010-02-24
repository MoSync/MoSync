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

#include "config_platform.h"

#include <bluetooth/discovery.h>
#include <helpers/fifo.h>

namespace Core {
	class VMCore;
}
extern Core::VMCore* gCore;
extern bool gRunning;

class Surface {
public:
	Surface(CGImageRef image) : image(image), context(NULL), data(NULL) {
	}

	Surface(int width, int height) {
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		int rowBytes = width*4;
		data = new char[rowBytes*height];
		context = CGBitmapContextCreate(data, width, height, 8, rowBytes, colorSpace, kCGImageAlphaNoneSkipFirst);
		
		CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, data, rowBytes * height, NULL);
		image = CGImageCreate(width, height, 8, 32, rowBytes, colorSpace, kCGImageAlphaNoneSkipFirst, dataProvider, NULL, false, kCGRenderingIntentDefault);
		CGDataProviderRelease(dataProvider);
		CGColorSpaceRelease(colorSpace);
		
		//CGContextTranslateCTM(context, 0, height);
		//CGContextScaleCTM(context, 1.0, -1.0);	
		CGContextSetAllowsAntialiasing (context, false);
		rect = CGRectMake(0, 0, width, height);
		
		CGContextSetRGBFillColor(context, 0, 0, 0, 1);
		CGContextFillRect(context, rect);	
		//CGContextClearRect(context, CGRectMake(0, 0, width, height));
	}
	
	
	~Surface() {
		if(image) CGImageRelease(image);
		if(context) CGContextRelease(context);
		if(data) delete data;
	}
	
	CGImageRef image;
	CGContextRef context;
	CGRect rect;
	char *data;
};

class EventQueue : public CircularFifo<MAEvent, EVENT_BUFFER_SIZE> {
public:
	EventQueue() : mEventOverflow(false) {
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
	
	void addCloseEvent() {
		MAEvent event;
		event.type = EVENT_TYPE_CLOSE;
		put(event);	
	}
	
private:
	bool mEventOverflow;
	
};

namespace Base
{
	extern EventQueue gEventQueue;
}

#endif
