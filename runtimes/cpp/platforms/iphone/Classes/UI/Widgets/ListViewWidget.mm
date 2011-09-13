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

#import "ListViewWidget.h"
#import "ListViewItemWidget.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation ListViewWidget

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [children count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSUInteger row = [indexPath row];
	IWidget* widget = [children objectAtIndex:row];
	UITableViewCell* cell = (UITableViewCell*)[widget getView];	
	[cell setFrame:CGRectMake(cell.frame.origin.x, cell.frame.origin.y, cell.frame.size.width,  tableView.rowHeight)];
	return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	//NSUInteger row = [indexPath row];
	//IWidget* widget = [children objectAtIndex:row];
	//UITableViewCell* cell = (UITableViewCell*)[widget getView];//[mDataForMyTable objectAtIndex:row];
	return tableView.rowHeight;
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath {
	NSInteger index = [indexPath row];
	
	NSLog(@"ListItem %d pressed!", index);
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_ITEM_CLICKED;
	eventData->widgetHandle = handle;
	eventData->listItemIndex = index;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);	
}

- (id)init {
	tableViewController = [[UITableViewController alloc] init];
	view = [tableViewController.tableView retain];		
	tableViewController.tableView.delegate = self;
	tableViewController.tableView.dataSource = self;
	id obj = [super init];		
	
	return obj;
}

- (void)dealloc {
    [tableViewController release];
    [super dealloc];
}

- (void)addChild: (IWidget*)child toSubview:(bool)toSubview {
	if([child class] != [ListViewItemWidget class]) {
		ListViewItemWidget* lvcw = [[ListViewItemWidget alloc] init];
		[lvcw addChild:child];
		child = lvcw;
	}
	
	[super addChild:child toSubview:NO];
	[((UITableView*)view) reloadData];
}

- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(bool)addSubview {
	if([child class] != [ListViewItemWidget class]) {
		ListViewItemWidget* lvcw = [[ListViewItemWidget alloc] init];
		[lvcw addChild:child];
		child = lvcw;
	}
	
	int ret = [super insertChild:child atIndex:index toSubview:NO];
	[((UITableView*)view) reloadData];
	return ret;
}

- (void)removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview {
	[super removeChild:child fromSuperview:removeFromSuperview];
	[((UITableView*)view) reloadData];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
    if([key isEqualToString:@"rowHeight"]) {
		float rowHeight = [value floatValue];
		UITableView* tableView = (UITableView*)view;
		tableView.rowHeight = rowHeight;
		[tableView reloadData];
	} 
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
