//
//  ListViewWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
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
	MAEvent *event = new MAEvent;
	event->type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_POINTER_PRESSED;
	eventData->widgetHandle = handle;
	event->data = eventData;
	Base::gEventQueue.put(*event);
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
