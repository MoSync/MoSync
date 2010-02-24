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
		
		CGContextTranslateCTM(context, 0, height);
		CGContextScaleCTM(context, 1.0, -1.0);	
		
		CGContextSetRGBFillColor(context, 0, 0, 0, 1);
		CGContextFillRect(context, CGRectMake(0, 0, width, height));	
		//CGContextClearRect(context, CGRectMake(0, 0, width, height));
	}
	
	
	~Surface() {
		if(image) CGImageRelease(image);
		if(context) CGContextRelease(context);
		if(data) delete data;
	}
	
	CGImageRef image;
	CGContextRef context;
	char *data;
};

#endif
