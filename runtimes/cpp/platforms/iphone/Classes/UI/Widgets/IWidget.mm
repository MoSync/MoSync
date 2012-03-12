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
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <helpers/cpp_defs.h>
#include "Platform.h"

#if 0
#import <QuartzCore/QuartzCore.h>
#endif

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
			viewWidth = view.bounds.size.width;
		}
		else if([child getAutoSizeParamX] == WRAP_CONTENT) {
			viewWidth = [childView sizeThatFits:CGSizeZero].width;
		}

		[childView setFrame:CGRectMake(childView.frame.origin.x, childView.frame.origin.y, viewWidth, viewHeight)];

		if([child getAutoSizeParamY] == FILL_PARENT) {
			viewHeight = view.bounds.size.height;
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
	//[self layout];
	[child layout];
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
	//[self layout];
	[child layout];

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
	[self layout];
}

- (int)remove {
	//if(!parent) return MAW_RES_REMOVED_ROOT;
	[parent removeChild: self];
	return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		[view setFrame:CGRectMake([value floatValue]/getScreenScale(), view.frame.origin.y, view.frame.size.width, view.frame.size.height)];
		[self layout];
	} else
	if([key isEqualToString:@MAW_WIDGET_TOP]) {
		[view setFrame:CGRectMake(view.frame.origin.x, [value floatValue]/getScreenScale(), view.frame.size.width, view.frame.size.height)];
		[self layout];
	} else
	if([key isEqualToString:@MAW_WIDGET_WIDTH]) {
		float width = [value floatValue];

		if(width == -2 || width == -1) {
			autoSizeParamX = width==-2?WRAP_CONTENT:FILL_PARENT;
			[self layout];
			return MAW_RES_OK;

		} else {
			autoSizeParamX = FIXED_SIZE;
		}

		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, width/getScreenScale(), view.bounds.size.height)];
		[self layout];
	} else
	if([key isEqualToString:@MAW_WIDGET_HEIGHT]) {
		float height = [value floatValue];

		if(height == -2 || height == -1) {
			autoSizeParamY = height==-2?WRAP_CONTENT:FILL_PARENT;
			[self layout];
			return MAW_RES_OK;

		} else {
			autoSizeParamY = FIXED_SIZE;
		}

		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.bounds.size.width, height/getScreenScale())];
		[self layout];
	} else
	if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR]) {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		view.backgroundColor = color;
	}
	/*else
	if([key isEqualToString:@"backgroundGradient"]) {
		NSArray *colors = [value componentsSeparatedByString: @","];
		UIColor *col1 = [UIColor colorWithHexString: [[colors objectAtIndex:0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]];
		UIColor *col2 = [UIColor colorWithHexString:[[colors objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]];

		if(!col1) return MAW_RES_INVALID_PROPERTY_VALUE;
		if(!col2) return MAW_RES_INVALID_PROPERTY_VALUE;

		CAGradientLayer *gradient = [CAGradientLayer layer];
		gradient.frame = CGRectMake(0, 0, 320, 480); //view.bounds;
		gradient.colors = [NSArray arrayWithObjects:(id)col1.CGColor, (id)col2.CGColor, nil];
		gradient.startPoint = CGPointMake(0.5f, 0.0f);
		gradient.endPoint = CGPointMake(0.5f, 1.0f);
		gradient.masksToBounds = YES;
		[view.layer insertSublayer:gradient atIndex:0];

		// todo: Fix this. Make a setFrame function in IWidget that sets the frame of both the gradient layer (if available) and the
		// view and call that wherever the frame of an IWidget is set (instead of view.frame = x).
	}
	*/
	else
	if([key isEqualToString:@MAW_WIDGET_ALPHA]) {
		float alpha = [value floatValue];
		if(alpha<0.0 || alpha>1.0) return MAW_RES_INVALID_PROPERTY_VALUE;
		view.alpha = [value floatValue];
	} else
	if([key isEqualToString:@"opaque"]){
		view.opaque = [value boolValue];
	} else
	if([key isEqualToString:@MAW_WIDGET_VISIBLE]){
		view.hidden = not [value boolValue];
	} else
	if([key isEqualToString:@MAW_WIDGET_ENABLED]){
		UIControl* controller = (UIControl*) view;
		controller.enabled = [value boolValue];
	}
	else {
		return MAW_RES_ERROR;
	}

	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {

	if([key isEqualToString:@MAW_WIDGET_WIDTH]) {
		return [[[NSNumber numberWithInt: view.bounds.size.width*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_HEIGHT]) {
		return [[[NSNumber numberWithInt: view.bounds.size.height*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		return [[[NSNumber numberWithInt: view.frame.origin.x*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_TOP]) {
		return [[[NSNumber numberWithInt: view.frame.origin.y*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_VISIBLE]) {
        return view.hidden ? @"false" : @"true";
    }
    else if([key isEqualToString:@MAW_WIDGET_ENABLED]) {
        UIControl* controller = (UIControl*) view; // TODO: is this correct?
        return controller.enabled ? @"true" : @"false";
    }
    else if ([key isEqualToString:@MAW_WIDGET_ALPHA])
    {
        return [[[NSNumber numberWithFloat: view.alpha] stringValue] retain];
    }
	return nil;
}

- (void)dealloc {
	[view release];
	[children release];
	[super dealloc];
}

// let's do this to make sure it is always released on the main thread..
- (oneway void)release
{
    if (![NSThread isMainThread]) {
        [self performSelectorOnMainThread:@selector(release) withObject:nil waitUntilDone:NO];
    } else {
        [super release];
    }
}

- (void)layout {
	// the layouts should take care of the fill parent / wrap content layouting process..
    // this should not be suboptimal, it will trigger a layout
    // that runs once before draw..
    [view setNeedsLayout];

    if(view.superview)
    {
        [view.superview setNeedsLayout];
    }

	[view layoutIfNeeded];
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
