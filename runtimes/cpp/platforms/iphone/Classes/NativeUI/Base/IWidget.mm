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
#import "MoSyncUISyscalls.h"
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
//    [self layout];
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
//    [self layout];
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
 * Will trigger a layout.
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
 * The child will be layouted.
 * @param child Widget to be added.
 * @param toSubview If true child's view will be added as subview, otherwise
 * it will only be added to a list.
 */
- (void)addChild: (IWidget*)child toSubview:(BOOL)toSubview
{
	UIView* childView = child.view;
	[child setParent:self];
	[_children addObject:child];
	if (toSubview)
    {
		[_view addSubview:childView];
	}

	[self layout];
}

/**
 * Insert a widget to a given index.
 * The child will be layouted.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @param toSubview If true child's view will be added as subview, otherwise
 * it will only be added to a list.
 * @return MAW_RES_OK if the child was added, MAW_RES_INVALID_INDEX if the index param
 * was invalid.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(BOOL)toSubview
{
	int indexValue = [index intValue];
	if (indexValue < 0 || indexValue > [_children count])
    {
        return MAW_RES_INVALID_INDEX;
    }

	[child setParent:self];
	[_children insertObject:child atIndex:indexValue];

	if (toSubview)
    {
		[_view insertSubview:child.view atIndex:indexValue];
	}

	[child layout];
	return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * Override this method to provide functionality.
 * The overrided method should verify if the widget can have children or not.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return MAW_RES_INVALID_LAYOUT.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index
{
    return MAW_RES_INVALID_LAYOUT;
}

/**
 * Remove a widget from its parent(remove _view from its superview).
 * @return MAW_RES_OK.
 */
- (int)remove
{
	[_parent removeChild: self];
	return MAW_RES_OK;
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child
{
	[self removeChild:child fromSuperview:YES];
}

/**
 * Remove a child from the widget.
 * @param child Widget to remove.
 * @param fromSuperview If true the child's view will be removed its superview,
 * otherwise it will only be removed from the children list.
 */
- (void)removeChild: (IWidget*)child fromSuperview:(bool)fromSuperview
{
	[_children removeObjectIdenticalTo:child];
	[child setParent:nil];
	if (fromSuperview)
    {
		[[child view] removeFromSuperview];
    }
	[self layout];
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;
{
    int result = MAW_RES_OK;
	if([key isEqualToString:@MAW_WIDGET_LEFT])
    {
        self.originX = [value floatValue] / getScreenScale();
	}
    else if([key isEqualToString:@MAW_WIDGET_TOP])
    {
		self.originY = [value floatValue] / getScreenScale();
	}
    else if([key isEqualToString:@MAW_WIDGET_WIDTH])
    {
        result = [self setWidthProperty:value];
	}
    else if([key isEqualToString:@MAW_WIDGET_HEIGHT])
    {
        result = [self setHeightProperty:value];
	}
    else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		_view.backgroundColor = color;
	}
	else if([key isEqualToString:@MAW_WIDGET_ALPHA])
    {
		float alpha = [value floatValue];
		if (alpha < 0.0 || alpha > 1.0) return MAW_RES_INVALID_PROPERTY_VALUE;
		_view.alpha = [value floatValue];
	}
    else if([key isEqualToString:@"opaque"])
    {
		_view.opaque = [value boolValue];
	}
    else if([key isEqualToString:@MAW_WIDGET_VISIBLE])
    {
		_view.hidden = not [value boolValue];
	}
    else if([key isEqualToString:@MAW_WIDGET_ENABLED])
    {
		UIControl* controller = (UIControl*) _view;
		controller.enabled = [value boolValue];
	}
	else
    {
		return MAW_RES_ERROR;
	}

	return result;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if([key isEqualToString:@MAW_WIDGET_WIDTH])
    {
		return [[[NSNumber numberWithInt: _view.bounds.size.width*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_HEIGHT])
    {
		return [[[NSNumber numberWithInt: _view.bounds.size.height*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_LEFT])
    {
		return [[[NSNumber numberWithInt: _view.frame.origin.x*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_TOP])
    {
		return [[[NSNumber numberWithInt: _view.frame.origin.y*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_VISIBLE])
    {
        return _view.hidden ? kWidgetFalseValue : kWidgetTrueValue;
    }
    else if([key isEqualToString:@MAW_WIDGET_ENABLED])
    {
        UIControl* controller = (UIControl*) _view;
        return controller.enabled ? kWidgetTrueValue : kWidgetFalseValue;
    }
    else if ([key isEqualToString:@MAW_WIDGET_ALPHA])
    {
        return [[[NSNumber numberWithFloat: _view.alpha] stringValue] retain];
    }
	return nil;
}

/**
 * Recalculate its size and other widget sizes, if needed.
 */
- (void)layout
{
    CGSize sizeThatFits = [self sizeThatFitsForWidget];
    float width = self.width;
    float height = self.height;
    if (self.autoSizeWidth != WidgetAutoSizeFixed)
    {
        width = sizeThatFits.width;
    }
    if (self.autoSizeHeight != WidgetAutoSizeFixed)
    {
        height = sizeThatFits.height;
    }

    self.size = CGSizeMake(width, height);

    // The widget's size will be set by the parent(if it has one).
    if (_parent)
    {
        [_parent layout];
    }
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
- (void)sendEvent:(const int)eventDataType
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = eventDataType;
	eventData->widgetHandle = _handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

/**
 * When a root screen is shown, this will be called recursively for all widgets.
 */
- (void)show
{
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

    [self layout];
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

    [self layout];
    return MAW_RES_OK;
}

/**
 * Let's do this to make sure it is always released on the main thread.
 */
- (oneway void)release
{
    if (![NSThread isMainThread])
    {
        [self performSelectorOnMainThread:@selector(release) withObject:nil waitUntilDone:NO];
    }
    else
    {
        [super release];
    }
}

/**
 * Realease all contained objects.
 */
- (void)dealloc
{
    // Destroy its children.
    for (IWidget* child in [[_children copy] autorelease])
    {
        maWidgetDestroy(child.handle);
    }

	[_view release];
	[_children release];

	[super dealloc];
}

/**
 * Get the index of a child from the children list.
 * @param child The child object whose index should be returned.
 * @return The index of the child, or NSNotFound if the child param is not
 * an child of the current widget.
 */
- (NSUInteger)indexOfChild:(IWidget*)child
{
    return [_children indexOfObject:child];
}

@end
