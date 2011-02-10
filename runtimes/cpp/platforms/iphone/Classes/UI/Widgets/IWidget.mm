/* Copyright (C) 2011 MoSync AB
 
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

#import "IWidget.h"
#import "UIColor-Expanded.h"
#import <objc/runtime.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation AbstractLayoutView (AbstractLayoutViewExpanded)

- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va {
	vAlignment = va;
}

- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha {
	hAlignment = ha;
}

@end

@implementation IWidget

- (id)init {
	[super init];
	[view setUserInteractionEnabled:YES];
	view.contentMode = UIViewContentModeRedraw;
	view.autoresizesSubviews = NO;
	parent = nil;
	children = [[NSMutableArray alloc] init];

	//[view injectMixin:[UIViewAdditions class]];

	fillWidth = 0;
	fillHeight = 0;
	[view sizeToFit]; 
		
	return self;
}

- (UIView*) getView {
	return view;
}

- (void) setWidgetHandle:(int) toHandle {
	handle = toHandle;
}

- (int)getWidgetHandle {
	return handle;
}

- (void)wasCreated {
}

- (void) setParent:(IWidget*) toParent {
	parent = toParent;
}

- (void) addChild: (IWidget*)child andSubview:(bool)addSubview {
	[child setParent:self];
	[children addObject:child];
	if(addSubview)
		[view addSubview:[child getView]];
}

- (void) addChild: (IWidget*)child {
	[self addChild:child andSubview:YES];	
}

- (void) removeChild: (IWidget*)child {
	[self removeChild:child fromSuperview:YES];
}

- (void) removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview {
	[children removeObjectIdenticalTo:child];
	[child setParent:nil];
	if(removeFromSuperview)
		[[child getView] removeFromSuperview];
}

- (int)remove {
	if(!parent) return WIDGET_ERROR;
	[parent removeChild: self];
}

- (int) setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	if([key isEqualToString:@"left"]) {
//		view.frame.origin.x = [value floatValue];
		[view setFrame:CGRectMake([value floatValue], view.frame.origin.y, view.frame.size.width, view.frame.size.height)];
	} else 
	if([key isEqualToString:@"top"]) {
//		view.frame.origin.y = [value floatValue];
		[view setFrame:CGRectMake(view.frame.origin.x, [value floatValue], view.frame.size.width, view.frame.size.height)];
	} else 
	if([key isEqualToString:@"width"]) {
		float width = [value floatValue];
		if(width == -1) {
			//view.autoresizingMask =	view.autoresizingMask | 
			//UIViewAutoresizingFlexibleWidth;
			width = view.superview.frame.size.width;
			//[view setNeedsLayout];
			fillWidth = -1;
			//return MA_WIDGET_OK;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, width, view.frame.size.height)];
	} else
	if([key isEqualToString:@"height"]) {
		float height = [value floatValue];
		if(height == -1) {
			//view.autoresizingMask =			view.autoresizingMask | 
			//UIViewAutoresizingFlexibleHeight;
			height = view.superview.frame.size.height;
			//[view setNeedsLayout];
			fillHeight = -1;
			//return MA_WIDGET_OK;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, height)];		
	} else 
	if([key isEqualToString:@"backgroundColor"]) {
		view.backgroundColor = [UIColor colorWithHexString:value];
	} else
	if([key isEqualToString:@"alpha"]) {
		view.alpha = [value floatValue];
	} else
	if([key isEqualToString:@"opaque"]){
		view.opaque = [value boolValue];
	} else
	if([key isEqualToString:@"visible"]){
		view.hidden = not [value boolValue];
	} else {
			return MA_WIDGET_ERROR;
	}
			
	return MA_WIDGET_OK;
}

- (NSString*) getPropertyWithKey: (NSString*)key {
	
	if([key isEqualToString:@"width"]) {		
		return [[NSNumber numberWithInt: view.frame.size.width ] stringValue];
		
	}
	else if([key isEqualToString:@"height"]) {
		return [[NSNumber numberWithInt: view.frame.size.height] stringValue];
	}
	else if([key isEqualToString:@"left"]) {		
		return [[NSNumber numberWithInt: view.frame.origin.x ] stringValue];
		
	}
	else if([key isEqualToString:@"top"]) {
		return [[NSNumber numberWithInt: view.frame.origin.y] stringValue];
	}
	
	return nil;
}

- (void)dealloc {
    [super dealloc];
	[view dealloc];
}

- (void)layout {	
	int viewWidth = view.frame.size.width; 
	if(fillWidth == -1) {
		viewWidth = view.superview.frame.size.width;
	}
	
	int viewHeight = view.frame.size.height; 
	if(fillHeight == -1) {
		viewHeight = view.superview.frame.size.height;
	}
	
	[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];
	//[view sizeToFit];
	
	//[view setNeedsLayout];
	//[view setNeedsDisplay];
	for (IWidget *child in children)
    {
		[child layout];

	}	
}

@end
