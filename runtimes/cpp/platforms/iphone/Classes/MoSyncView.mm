//
//  MoSyncView.m
//  MoSync
//
//  Created by Niklas Nummelin on 2/23/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MoSyncView.h"
#include "MosyncMain.h"

@implementation MoSyncView

- (void)updateMoSyncView:(CGContextRef)ref {
	mosyncView = ref;
	//[self setNeedsDisplay];
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)decoder {
    if (self = [super initWithCoder:decoder]) {
		[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
		mosyncView = nil;
        // Initialization code
		MoSyncMain(self.frame.size.width, self.frame.size.height, self);
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;
	
	CGImageRef image = CGBitmapContextCreateImage(mosyncView);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, rect, image);	
	
	DoneUpdatingMoSyncView();
}


- (void)dealloc {
    [super dealloc];
}


@end
