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



- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)decoder {
    if (self = [super initWithCoder:decoder]) {
        // Initialization code
		MoSyncMain(self.frame.size.width, self.frame.size.height, self);
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
    // Drawing code
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	// drawing with a white stroke color
	CGContextSetRGBStrokeColor(ctx, 1.0, 1.0, 1.0, 1.0);
	// drawing with a white fill color
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 1.0, 1.0);
	// Add Filled Rectangle, 
	CGContextFillRect(ctx, rect);
}


- (void)dealloc {
    [super dealloc];
}


@end
