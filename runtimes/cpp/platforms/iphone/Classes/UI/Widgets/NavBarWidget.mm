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

#import "NavBarWidget.h"
#import "UIColor-Expanded.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation NavBarWidget

- (id)init {	
	UINavigationBar* navbar = [[[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, 100, 44)] retain];
	navbar.delegate = self;
	prevNavitem = [[UINavigationItem alloc] init];
	currNavitem = [[UINavigationItem alloc] init];
	[navbar pushNavigationItem:prevNavitem animated:false];
	[navbar pushNavigationItem:currNavitem animated:false];
	view = navbar;		
	
	return [super init];	
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"title"])
    {
		currNavitem.title = value;
	}
	else if([key isEqualToString:@MAW_NAV_BAR_BACK_BTN])
    {
		//navitem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:value style:UIBarButtonItemStylePlain target:nil action:nil];
		//navitem.hidesBackButton = YES;
		prevNavitem.title = value;
	}
    else if([key isEqualToString:@"backgroundColor"])
    {
        // The background color property must be handled different.
        // For the rest of the widgets the property is handled by
        // the super class(IWidget).
        UINavigationBar* navBar = (UINavigationBar*) view;
        UIColor* color = [UIColor colorWithHexString:value];
		if (!color)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
		navBar.tintColor = color;
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	return [super getPropertyWithKey:key];
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPopItem:(UINavigationItem *)item {
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPushItem:(UINavigationItem *)item {
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item {
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
	return false;
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPushItem:(UINavigationItem *)item {
	return true;
}

@end
