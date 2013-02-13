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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

#import "SearchBarWidget.h"
#import "UIColorExpanded.h"
#include "Platform.h"

@implementation SearchBarWidget

- (id)init
{
    self = [super init];
    if (self)
    {
        _searchBar = [[UISearchBar alloc] initWithFrame:CGRectMake(0, 10, 100, 30)];
        _searchBar.placeholder = @"Search";
        self.view = _searchBar;

        [_searchBar setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
        self.view.autoresizesSubviews = YES;
        _searchBar.showsCancelButton = YES;
        _searchBar.delegate = self;
    }
	return self;
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
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_SEARCH_BAR_TEXT])
    {
		_searchBar.text = value;
	}
	else if([key isEqualToString:@MAW_SEARCH_BAR_PLACEHOLDER])
    {
		_searchBar.placeholder = value;
	}
	else if([key isEqualToString:@MAW_SEARCH_BAR_SHOW_KEYBOARD])
    {
		if ([value isEqualToString:kWidgetTrueValue])
        {
			[_searchBar becomeFirstResponder];
		}
        else if ([value isEqualToString:kWidgetFalseValue])
        {
			[_searchBar resignFirstResponder];
		}
        else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
    else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
        UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		_searchBar.tintColor = color;
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if([key isEqualToString:@MAW_SEARCH_BAR_TEXT])
    {
		return [_searchBar.text retain];
	}

	return [super getPropertyWithKey:key];
}

- (void) searchBarSearchButtonClicked:(UISearchBar *)searchBar
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = self.handle;
	eventData->searchBarButton = 0;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}


- (void) searchBarCancelButtonClicked:(UISearchBar *)searchBar
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = self.handle;
	eventData->searchBarButton = 1;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}


/**
 * Dealloc method.
 */
-(void) dealloc
{
    [_searchBar release];
    [super dealloc];
}

@end
