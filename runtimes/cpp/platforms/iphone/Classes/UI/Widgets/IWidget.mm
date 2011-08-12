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
#include <helpers/cpp_defs.h>
#include "Platform.h"

@interface UIView (UIViewExpanded) 
- (void)superLayoutSubviews;
- (CGSize)superSizeThatFits:(CGSize)size;
@end

@implementation UIView (UIViewExpanded)

- (void)superLayoutSubviews {
}
	
- (CGSize)superSizeThatFits:(CGSize)size {
	return CGSizeMake(0, 0);
}

@end

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
//- (void)layoutSubviews {	
	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		
		int viewWidth = childView.frame.size.width;		
		int viewHeight = childView.frame.size.height;
		
		if([child getAutoSizeParamX] == FILL_PARENT) {
			viewWidth = view.frame.size.width;
		}
		else if([child getAutoSizeParamX] == WRAP_CONTENT) {
			viewWidth = [childView sizeThatFits:CGSizeZero].width;
		}
		
		[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, viewWidth, viewHeight)];		
		
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
	CGSize ret = [_view superSizeThatFits:size];
	return ret;
}

- (id)init {
	[super init];

	parent = nil;
	children = [[NSMutableArray alloc] init];
	[self setAutoSizeParamX:FIXED_SIZE andY:FIXED_SIZE];	

	if(view) {
		[view setUserInteractionEnabled:YES];
		view.contentMode = UIViewContentModeRedraw;
		view.autoresizesSubviews = NO;
		//view.autoresizesSubviews = YES;
		view.backgroundColor = [UIColor colorWithHexString:@"00000000"];
		//view.backgroundColor = [UIColor redColor];
        
		//[[view layer] setBorderWidth:1.0];
		//[[view layer] setBorderColor:[UIColor redColor].CGColor];
    }
	
	return self;
}

- (UIView*)getView {
	return view;
}

- (void)setWidgetHandle:(int) toHandle {
	handle = toHandle;
	view.tag = handle;
}

- (int)getWidgetHandle {
	return handle;
}

- (void)setParent:(IWidget*) toParent {
	parent = toParent;
}

- (IWidget*)getParent {
	return parent;
}

- (void)addChild: (IWidget*)child toSubview:(bool)addSubview {
	UIView* childView = [child getView]; 
	[child setParent:self];
	[children addObject:child];
	if(addSubview) {
		[view addSubview:childView];
	}
//	[self layout];
	[view setNeedsLayout];
	[view setNeedsDisplay];
}

- (void)addChild: (IWidget*)child {
	[self addChild:child toSubview:YES];	
}

- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(bool)addSubview {
	int indexValue = [index intValue];
	if(indexValue<0 || indexValue>[children count]) return MAW_RES_INVALID_INDEX;
	
	UIView* childView = [child getView];
	[child setParent:self];
	
	[children insertObject:child atIndex:indexValue];
	if(addSubview) {
		[view insertSubview:childView atIndex:indexValue];
	}
	[view setNeedsLayout];
	
	return MAW_RES_OK;
}

- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index {
	return [self insertChild:child atIndex:index toSubview:YES];
}

- (void)removeChild: (IWidget*)child {
	[self removeChild:child fromSuperview:YES];
}

- (void)removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview {
	[children removeObjectIdenticalTo:child];
	[child setParent:nil];
	if(removeFromSuperview)
		[[child getView] removeFromSuperview];
	[view setNeedsLayout];
}

- (int)remove {
	//if(!parent) return MAW_RES_REMOVED_ROOT;
	[parent removeChild: self];
	return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	if([key isEqualToString:@"left"]) {
		[view setFrame:CGRectMake([value floatValue], view.frame.origin.y, view.frame.size.width, view.frame.size.height)];
		[self layout];		
	} else 
	if([key isEqualToString:@"top"]) {
		[view setFrame:CGRectMake(view.frame.origin.x, [value floatValue], view.frame.size.width, view.frame.size.height)];
		[self layout];
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
		[self layout];
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
		[self layout];		
	} else 
	if([key isEqualToString:@"backgroundColor"]) {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		view.backgroundColor = color;
	} else
	if([key isEqualToString:@"alpha"]) {
		float alpha = [value floatValue];
		if(alpha<0.0 || alpha>1.0) return MAW_RES_INVALID_PROPERTY_VALUE;
		view.alpha = [value floatValue];
	} else
	if([key isEqualToString:@"opaque"]){
		view.opaque = [value boolValue];
	} else
	if([key isEqualToString:@"visible"]){
		view.hidden = not [value boolValue];
	} else
    if([key isEqualToString:@"enabled"]){
        UIControl* controller = (UIControl*) view;
        controller.enabled = [value boolValue];
    }
    else {
			return MAW_RES_ERROR;
	}
			
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
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
	// the layouts should take care of the fill parent / wrap content layouting process..
	// so no need to do it here.
	//[view setNeedsLayout];
	[view setNeedsLayout];
//	[view layoutIfNeeded];
	//[view setNeedsDisplay];	
	//[view layoutIfNeeded];
}

/**
 * Send a widget event type.
 * @param eventDataType One of the MAW_EVENT constants.
 */
- (void) sendEvent:(const int) eventDataType
{
    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = eventDataType;
	eventData->widgetHandle = handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (void)show {
	[self layout];
	for (IWidget *child in children)
    {
		[child show];
	}	
}

@end
