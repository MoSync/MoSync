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

@implementation IWidget


- (void)setAutoSizeParamX:(AutoSizeParam)x andY:(AutoSizeParam)y {
	autoSizeParamX = x;
	autoSizeParamY = y;
}

- (AutoSizeParam)getAutoSizeParamX {
	return autoSizeParamX;
}

- (AutoSizeParam)getAutoSizeParamY {
	return autoSizeParamY;
}

- (void)layoutSubviews:(UIView*)_view {
	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		
		int viewWidth = childView.frame.size.width; 
		if([child getAutoSizeParamX] == FILL_PARENT) {
			viewWidth = view.frame.size.width;
		}
		else if([child getAutoSizeParamX] == WRAP_CONTENT) {
			viewWidth = [childView sizeThatFits:CGSizeZero].width;
		}
		
		int viewHeight = childView.frame.size.height; 
		if([child getAutoSizeParamY] == FILL_PARENT) {
			viewHeight = view.frame.size.height;
		}
		else if([child getAutoSizeParamY] == WRAP_CONTENT) {
			viewHeight = [childView sizeThatFits:CGSizeZero].height;
		}
		
		[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, viewWidth, viewHeight)];
	}
	
	[_view superLayoutSubviews];
}

- (CGSize)sizeThatFitsFor:(UIView*)_view withSize:(CGSize)size {
	return _view.frame.size;
}

- (id)init {
	[super init];
	[view setUserInteractionEnabled:YES];
	view.contentMode = UIViewContentModeRedraw;
	view.autoresizesSubviews = NO;
	parent = nil;
	children = [[NSMutableArray alloc] init];

	//[view injectMixin:[UIViewAdditions class]];
	//autoSizeParamX = FIXED_SIZE;
	//autoSizeParamY = FIXED_SIZE;
	//[self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
	[self setAutoSizeParamX:FIXED_SIZE andY:FIXED_SIZE];
	
	//[view setWidget:self];	
	//[view sizeToFit]; 
		
	view.backgroundColor = [UIColor colorWithHexString:@"00000000"];
	
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
	UIView* childView = [child getView]; 
	/*
	if([child getFillWidth] == -1)
		[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, view.frame.size.width, childView.frame.size.height)];
	if([child getFillHeight] == -1)
		[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, childView.frame.size.width, view.frame.size.height)];	
	*/
	
	[child setParent:self];
	[children addObject:child];
	if(addSubview)
		[view addSubview:[child getView]];
	
	[view setNeedsLayout];
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
	if(!parent) return MAW_RES_REMOVED_ROOT;
	[parent removeChild: self];
	return MAW_RES_OK;
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
		
		if(width == -2 || width == -1) {
			autoSizeParamX = width==-2?WRAP_CONTENT:FILL_PARENT;
			[self layout];
			return MAW_RES_OK;
			
		} else {
			autoSizeParamX = FIXED_SIZE;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, width, view.frame.size.height)];
	} else
	if([key isEqualToString:@"height"]) {
		float height = [value floatValue];
				
		if(height == -2 || height == -1) {
			autoSizeParamY = height==-2?WRAP_CONTENT:FILL_PARENT;
			[self layout];
			return MAW_RES_OK;
			
		} else {
			autoSizeParamY = FIXED_SIZE;
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
			return MAW_RES_ERROR;
	}
	
	//[view setNeedsDisplay];
	//[view setNeedsLayout];
			
	return MAW_RES_OK;
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
	[view setNeedsLayout];
//	[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, view.frame.size.height)];			
	
	int viewWidth = view.frame.size.width; 
	if(autoSizeParamX == FILL_PARENT) {
		if(view.superview)
			viewWidth = view.superview.frame.size.width;
	}
	else if(autoSizeParamX == WRAP_CONTENT) {
		viewWidth = [view sizeThatFits:CGSizeZero].width;
	}
	 
	int viewHeight = view.frame.size.height; 
	if(autoSizeParamY == FILL_PARENT) {
		if(view.superview)
			viewHeight = view.superview.frame.size.height;
	}
	else if(autoSizeParamY == WRAP_CONTENT) {
		viewHeight = [view sizeThatFits:CGSizeZero].height;
	}
	
	[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];
	
	return;
	
	//[view setNeedsLayout];
	//[view setNeedsDisplay];
	for (IWidget *child in children)
    {
		[child layout];

	}	
}

@end
