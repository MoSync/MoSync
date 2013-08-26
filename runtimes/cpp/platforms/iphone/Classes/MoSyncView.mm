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
