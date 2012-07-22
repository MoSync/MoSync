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

#import <objc/runtime.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <helpers/cpp_defs.h>

#import "IWidget.h"
#import "UIColorExpanded.h"
#import "NSStringExpanded.h"
#include "Platform.h"

@implementation IWidget

@synthesize view = _view;
@synthesize parent = _parent;
@synthesize handle = _handle;
@synthesize autoSizeWidth = _autoSizeWidth;
@synthesize autoSizeHeight = _autoSizeHeight;


/**
 * Init function.
 */
- (id) init
{
	self = [super init];
    if (self)
    {
        _parent = nil;
        _children = [[NSMutableArray alloc] init];
        self.autoSizeHeight = WidgetAutoSizeFixed;
        self.autoSizeWidth = WidgetAutoSizeFixed;
    }
	return self;
}

/**
 * Set widget's width.
 * @param width Width to set.
 */
-(void) setWidth:(CGFloat)width
{
    CGRect rect = CGRectMake(self.originX,
                             self.originY,
                             width,
                             self.height);
    _view.frame = rect;
    [self layout];
}

/**
 * Get widget's width.
 * @return Widget's width.
 */
-(CGFloat) width
{
    return _view.bounds.size.width;
}

/**
 * Set widgets height.
 * @param height Height to set.
 */
-(void) setHeight:(CGFloat)height
{
    CGRect rect = CGRectMake(self.originX,
                             self.originY,
                             self.width,
                             height);
    _view.frame = rect;
    [self layout];
}

/**
 * Get widget's height.
 */
-(CGFloat) height
{
    return _view.bounds.size.height;
}

/**
 * Set widget's size.
 * @param size Size to set.
 */
-(void) setSize:(CGSize)size
{
    CGRect rect = CGRectMake(self.originX,
                             self.originY,
                             size.width,
                             size.height);
    _view.frame = rect;
    [self layout];
}

/**
 * Get widget's size.
 */
-(CGSize) size
{
    return _view.bounds.size;
}

/**
 * Set widget's left coordinate(or view's x coordinate).
 */
-(void) setOriginX:(CGFloat)originX
{
    CGRect rect = CGRectMake(originX,
                             _view.frame.origin.y,
                             _view.frame.size.width,
                             _view.frame.size.height);
    _view.frame = rect;
}

/**
 * Get widget's left coordinate(or view's x coordinate).
 */
-(CGFloat) originX
{
    return _view.frame.origin.x;
}

/**
 * Set widget's top coordinate(or view's y coordinate).
 */
-(void) setOriginY:(CGFloat)originY
{
    CGRect rect = CGRectMake(_view.frame.origin.x,
                             originY,
                             _view.frame.size.width,
                             _view.frame.size.height);
    _view.frame = rect;
}

/**
 * Get widget's top coordinate(or view's y coordinate).
 */
-(CGFloat) originY
{
    return _view.frame.origin.y;
}

/**
 * Set widget's auto size width and height values.
 * @param autoSizeWidth Width auto size value to set.
 * @param autoSizeHeight Height auto size value to set.
 */
-(void) setAutoSizeWidth:(WidgetAutoSizeType)autoSizeWidth
                  height:(WidgetAutoSizeType)autoSizeHeight
{
    self.autoSizeWidth = autoSizeWidth;
    self.autoSizeHeight = autoSizeHeight;
    [self layout];
}

/**
 * Adds an widget to the end of the children list.
 * Override this method to provide functionality.
 * The overrided method should verify if the widget can have children or not.
 * @param child Widget to be added.
 * @return MAW_RES_INVALID_LAYOUT.
 */
- (int)addChild:(IWidget*)child
{
    return MAW_RES_INVALID_LAYOUT;
}

/**
 * Adds an widget to the end of the children list.
 * It does not check if the widget can have children.
 * @param child Widget to be added.
 * @param toSubview If true child's view will be added as subview, otherwise
 * it only be added to a list.
 */
- (void)addChild: (IWidget*)child toSubview:(BOOL)addSubview
{
    INNativeUILog;
	UIView* childView = child.view;
	[child setParent:self];
	[_children addObject:child];
	if(addSubview)
    {
		[_view addSubview:childView];
	}

	[child layout];
    OUTNativeUILog;
}

- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(bool)addSubview {
	int indexValue = [index intValue];
	if(indexValue<0 || indexValue>[_children count]) return MAW_RES_INVALID_INDEX;

	UIView* childView = [child view];
	[child setParent:self];

	[_children insertObject:child atIndex:indexValue];
	if(addSubview) {
		[_view insertSubview:childView atIndex:indexValue];
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
	[_children removeObjectIdenticalTo:child];
	[child setParent:nil];
	if(removeFromSuperview)
		[[child view] removeFromSuperview];
	[self layout];
}

- (int)remove {
	//if(!parent) return MAW_RES_REMOVED_ROOT;
	[_parent removeChild: self];
	return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	if([key isEqualToString:@MAW_WIDGET_LEFT])
    {
        self.originX = [value floatValue]/getScreenScale();
	}
    else if([key isEqualToString:@MAW_WIDGET_TOP])
    {
		self.originY = [value floatValue]/getScreenScale();
	}
    else if([key isEqualToString:@MAW_WIDGET_WIDTH])
    {
        return [self setWidthProperty:value];
	}
    else if([key isEqualToString:@MAW_WIDGET_HEIGHT])
    {
        return [self setHeightProperty:value];
	} else
	if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR]) {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		_view.backgroundColor = color;
	}
	else
	if([key isEqualToString:@MAW_WIDGET_ALPHA]) {
		float alpha = [value floatValue];
		if(alpha<0.0 || alpha>1.0) return MAW_RES_INVALID_PROPERTY_VALUE;
		_view.alpha = [value floatValue];
	} else
	if([key isEqualToString:@"opaque"]){
		_view.opaque = [value boolValue];
	} else
	if([key isEqualToString:@MAW_WIDGET_VISIBLE]){
		_view.hidden = not [value boolValue];
	} else
	if([key isEqualToString:@MAW_WIDGET_ENABLED]){
		UIControl* controller = (UIControl*) _view;
		controller.enabled = [value boolValue];
	}
	else {
		return MAW_RES_ERROR;
	}

	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {

	if([key isEqualToString:@MAW_WIDGET_WIDTH]) {
		return [[[NSNumber numberWithInt: _view.bounds.size.width*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_HEIGHT]) {
		return [[[NSNumber numberWithInt: _view.bounds.size.height*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		return [[[NSNumber numberWithInt: _view.frame.origin.x*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_TOP]) {
		return [[[NSNumber numberWithInt: _view.frame.origin.y*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_VISIBLE]) {
        return _view.hidden ? @"false" : @"true";
    }
    else if([key isEqualToString:@MAW_WIDGET_ENABLED]) {
        UIControl* controller = (UIControl*) _view; // TODO: is this correct?
        return controller.enabled ? @"true" : @"false";
    }
    else if ([key isEqualToString:@MAW_WIDGET_ALPHA])
    {
        return [[[NSNumber numberWithFloat: _view.alpha] stringValue] retain];
    }
	return nil;
}

- (void)dealloc {
	[_view release];
    [_parent release];
	[_children release];

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

/**
 * Recalculate its and children size.
 * If needed and possible the parent will be resized too.
 */
- (void)layout
{
    INNativeUILog;
    [self layoutSubviews:_view];
    // If the widget has parent and if at least one of its auto size params is wrap content,
    // then ask the parent to layout itself.
    if (_parent &&
        (_parent.autoSizeWidth == WidgetAutoSizeWrapContent ||
         _parent.autoSizeHeight == WidgetAutoSizeWrapContent))
    {
        [_parent layout];
    }
    OUTNativeUILog;
}

/**
 * Layout its children.
 * This method should be called by the class generated by widget layouting macro.
 * @param view The view that requested the operation.
 */
-(void) layoutSubviews:(UIView*) view
{
}

/**
 * Asks the widget to calculate and return the size that best fits its subviews.
 * Override this method to provide functionality.
 * @return CGSizeZero.
 */
- (CGSize)sizeThatFitsForWidget
{
    if (!_view)
    {
        return CGSizeZero;
    }
    else
    {
        return [_view sizeThatFits:CGSizeZero];
    }
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
	eventData->widgetHandle = _handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (void)show {
//    INNativeUILog;
//	[self layout];
//	for (IWidget *child in children)
//	{
//		[child show];
//	}
//    OUTNativeUILog;
}

/**
 * Setter for MAW_WIDGET_WIDTH.
 * @param value MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_WRAP_CONTENT or
 * an int value greater or equal to zero.
 * @return MAW_RES_OK if the width was set, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setWidthProperty:(NSString*) value
{
    // Verify if the value param is valid.
    if (![value canParseNumber] )
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    int width = [value intValue];
    if (width < 0 &&
        width != MAW_CONSTANT_FILL_AVAILABLE_SPACE &&
        width != MAW_CONSTANT_WRAP_CONTENT)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    // width value is valid.
    switch (width)
    {
        case MAW_CONSTANT_FILL_AVAILABLE_SPACE:
            self.autoSizeWidth = WidgetAutoSizeFillParent;
            break;
        case MAW_CONSTANT_WRAP_CONTENT:
            self.autoSizeWidth = WidgetAutoSizeWrapContent;
            break;
        default:
            self.autoSizeWidth = WidgetAutoSizeFixed;
            self.width = (CGFloat)width / getScreenScale();
            break;
    }
    return MAW_RES_OK;
}

/**
 * Setter for MAW_WIDGET_HEIGHT.
 * @param value MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_WRAP_CONTENT or
 * an int value greater or equal to zero.
 * @return MAW_RES_OK if the height was set, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setHeightProperty:(NSString*) value
{
    // Verify if the value param is valid.
    if (![value canParseNumber] )
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    int height = [value intValue];
    if (height < 0 &&
        height != MAW_CONSTANT_FILL_AVAILABLE_SPACE &&
        height != MAW_CONSTANT_WRAP_CONTENT)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    // height value is valid.
    switch (height)
    {
        case MAW_CONSTANT_FILL_AVAILABLE_SPACE:
            self.autoSizeHeight = WidgetAutoSizeFillParent;
            break;
        case MAW_CONSTANT_WRAP_CONTENT:
            self.autoSizeHeight = WidgetAutoSizeWrapContent;
            break;
        default:
            self.autoSizeHeight = WidgetAutoSizeFixed;
            self.height = (CGFloat)height / getScreenScale();
            break;
    }
    return MAW_RES_OK;
}

@end
