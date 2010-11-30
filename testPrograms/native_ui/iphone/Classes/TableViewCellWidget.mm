//
//  TableViewCellWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TableViewCellWidget.h"


@implementation TableViewCellWidget

- (id)init {
	[super init];	

	static NSString *SimpleTableIdentifier = @"SimpleTableIdentifier";
	
	UITableViewCell *cell = nil; //[UITableView dequeueReusableCellWithIdentifier:SimpleTableIdentifier];
	 
	if (cell == nil) {
	 cell = [[[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault
								   reuseIdentifier:SimpleTableIdentifier] autorelease];
	}

	//cell.textLabel.backgroundColor = [UIColor colorWithRGBHex:0xff0000];
	//cell.backgroundView = cellContentView;
	//cell.selectedBackgroundView = cellContentView;	
	view = cell;
	
	return self;
}

- (void)addChild: (IWidget*)child {
//	[super addChild:child];
	UITableViewCell *cell = (UITableViewCell*)view;
	[cell.contentView addSubview: [child getView]];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UITableViewCell* cell = (UITableViewCell*) view;
		cell.text = value;
	} else 
	if([key isEqualToString:@"backgroundColor"]) {
		//	[super addChild:child];
		UITableViewCell *cell = (UITableViewCell*)view;
		cell.contentView.backgroundColor = [UIColor colorWithHexString:value];
		cell.textLabel.backgroundColor = [UIColor colorWithHexString:value];
		
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
