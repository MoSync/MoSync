/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//
//  ListViewWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import "ListViewWidget.h"
#import "ListViewItemWidget.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#endif

@implementation ListViewWidget

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath {
	NSInteger index = [indexPath row];
	
	NSLog(@"ListItem %d pressed!", index);
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_ITEM_CLICKED;
	eventData->widgetHandle = handle;
	eventData->listItemIndex = index;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif
	
}

- (id)init {
	tableView = [[MoSyncTableView alloc] init];
	view = tableView.tableView;	
	
//	[view addTarget:self action:@selector(buttonPressed) forControlEvents:UIControlEventTouchUpInside];
	tableView.tableView.delegate = self;
	id obj = [super init];		
	
	return obj;
}

- (void)addChild: (IWidget*)child {
	if([child class] != [ListViewItemWidget class]) {
		ListViewItemWidget* lvcw = [[ListViewItemWidget alloc] init];
		[lvcw addChild:child];
		child = lvcw;
	}
	
	[tableView addView: [child getView]];
	[view reloadData];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {

	return [super setPropertyWithKey:key toValue:value];
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
