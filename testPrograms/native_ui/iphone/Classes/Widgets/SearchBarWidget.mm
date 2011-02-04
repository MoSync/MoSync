//
//  SearchScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SearchBarWidget.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif


@implementation SearchBarWidget

- (void) searchBarSearchButtonClicked:(UISearchBar *)searchBar {
	
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->searchBarButton = 0;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif	
}


- (void) searchBarCancelButtonClicked:(UISearchBar *)searchBar {
	
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->searchBarButton = 1;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif	
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

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
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
	return WIDGET_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"text"]) {
		return searchBar.text;
	}	
	
	return [super getPropertyWithKey:key];
}

@end