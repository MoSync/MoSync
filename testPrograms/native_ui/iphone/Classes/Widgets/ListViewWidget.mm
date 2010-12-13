//
//  ListViewWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ListViewWidget.h"
#import "ListViewCellWidget.h"


@implementation ListViewWidget

- (id)init {
	tableView = [[MoSyncTableView alloc] init];
	view = tableView.tableView;	
	
	return [super init];		
}

- (void)addChild: (IWidget*)child {
	if([child class] != [ListViewCellWidget class]) {
		ListViewCellWidget* lvcw = [[ListViewCellWidget alloc] init];
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
