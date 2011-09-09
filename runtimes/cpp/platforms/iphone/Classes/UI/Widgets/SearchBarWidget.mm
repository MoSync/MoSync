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

#import "SearchBarWidget.h"

#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation SearchBarWidget

- (void) searchBarSearchButtonClicked:(UISearchBar *)searchBar {	
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->searchBarButton = 0;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}


- (void) searchBarCancelButtonClicked:(UISearchBar *)searchBar {	
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->searchBarButton = 1;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}


- (id)init {
	searchBar = [[[UISearchBar alloc] initWithFrame:CGRectMake(0, 10, 100, 30)] retain];
	searchBar.placeholder = @"Search";

	
	view = searchBar;			
	id ret = [super init];
	[searchBar setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
	view.autoresizesSubviews = YES;
	searchBar.showsCancelButton = YES;
	searchBar.delegate = self;
	
	return ret;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		searchBar.text = value;
	}
	else if([key isEqualToString:@"placeholder"]) {
		searchBar.placeholder = value;
	}
	else if([key isEqualToString:@"showKeyboard"]) {
		if([value isEqualToString:@"true"]) {
			[searchBar becomeFirstResponder];
		} else {
			[searchBar resignFirstResponder];
		}
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"text"]) {
		return [searchBar.text retain];
	}	
	
	return [super getPropertyWithKey:key];
}

@end