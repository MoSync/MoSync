//
//  ListViewCellWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ListViewItemWidget.h"
#import "UIColor-Expanded.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif

@implementation ListViewItemWidget

- (id)init {

	static NSString *SimpleTableIdentifier = @"SimpleTableIdentifier";
	
	UITableViewCell *cell = nil; //[UITableView dequeueReusableCellWithIdentifier:SimpleTableIdentifier];
	 
	if (cell == nil) {
		cell = [[[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault
								   reuseIdentifier:SimpleTableIdentifier] retain];
		cell.selectionStyle =  UITableViewCellSelectionStyleNone;
	}

	//cell.textLabel.backgroundColor = [UIColor colorWithRGBHex:0xff0000];
	//cell.backgroundView = cellContentView;
	//cell.selectedBackgroundView = cellContentView;	
	view = cell;
	
	return [super init];		
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
		//cell.text = value;
		UILabel* label = cell.textLabel;
		label.text = value;
	} 
	else if([key isEqualToString:@"image"]) {
		int imageHandle = [value intValue];
		UITableViewCell* cell = (UITableViewCell*) view;
		UIImageView* imageView = cell.imageView;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);		
		imageView.image = [UIImage imageWithCGImage:imageResource->image];
		
	}
	else if([key isEqualToString:@"backgroundColor"]) {
		//	[super addChild:child];
		UITableViewCell *cell = (UITableViewCell*)view;
		cell.contentView.backgroundColor = [UIColor colorWithHexString:value];
		cell.textLabel.backgroundColor = [UIColor colorWithHexString:value];
		cell.accessoryView.backgroundColor = [UIColor colorWithHexString:value];		
	} 
    else if([key isEqualToString:@"accessoryType"]) {
		UITableViewCell *cell = (UITableViewCell*)view;
		
		if([value isEqualToString:@"hasChildren"]) {
			cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
		} 
		else if([value isEqualToString:@"hasDetails"]) {
			cell.accessoryType = UITableViewCellAccessoryDetailDisclosureButton;
		}
		else if([value isEqualToString:@"isChecked"]) {
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
		} 
		else {
			cell.accessoryType = UITableViewCellAccessoryNone;			
		}		   
	} 
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
