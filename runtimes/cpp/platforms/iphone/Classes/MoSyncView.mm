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

#import "MoSyncView.h"
#include "MosyncMain.h"

@implementation MoSyncView

- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:NO];
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		self.clearsContextBeforeDrawing = NO;
		UIDevice* myDevice = [UIDevice currentDevice];
		[myDevice setBatteryMonitoringEnabled:YES];

        int w, h;
        getScreenResolution(w, h);
        if (w == 640 && h == 960) // Retina display detected
        {
            // Set contentScale Factor to 2
            self.contentScaleFactor = 2.0;
        }
        
		self.frame.origin = CGRectZero.origin;
		mosyncView = nil;
        // Initialization code
		self.multipleTouchEnabled = YES;
		touchHelper = [[TouchHelper alloc] init];
		self.contentMode = UIViewContentModeRedraw;
		/*
		CGRect appFrame = [[UIScreen mainScreen] bounds];
		CGFloat screenHeight = appFrame.size.height;
		CGFloat screenWidth = appFrame.size.width;
		MoSync_Main(screenWidth, screenHeight, self);
		 */
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;
	
    CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSetInterpolationQuality(context, kCGInterpolationNone);
	CGContextSetAllowsAntialiasing(context, false);
	CGContextTranslateCTM(context, 0, self.bounds.size.height); //CGImageGetHeight(mosyncView));
	CGContextScaleCTM(context, 1.0, -1.0);
	CGContextDrawImage(context, rect, mosyncView);		
	
	MoSync_DoneUpdatingView();
}

- (void)dealloc {
    [super dealloc];
}

@end
