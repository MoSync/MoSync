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

#import "MoSyncView.h"
#include "MosyncMain.h"

#include "iphone_helpers.h"

#include "Platform.h"

@implementation MoSyncView

- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
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
		self.frame.origin.y = 0;
		mosyncView = nil;
        // Initialization code
		MoSyncMain(self.frame.size.width, self.frame.size.height, self);
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;

    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, rect, mosyncView);	
	DoneUpdatingMoSyncView();
	 
}


- (void)dealloc {
    [super dealloc];
}

bool down = false;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];

	if(!down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_PRESSED);
		down = true;
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_DRAGGED);
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_RELEASED);	
		down = false;
	}	
}

@end
