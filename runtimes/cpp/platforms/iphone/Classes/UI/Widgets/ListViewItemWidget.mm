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
	[self layout];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UITableViewCell* cell = (UITableViewCell*) view;
		//cell.text = value;
		UILabel* label = cell.textLabel;
		label.text = value;
	} 
	else if([key isEqualToString:@"icon"]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_HANDLE;
		UITableViewCell* cell = (UITableViewCell*) view;
		UIImageView* imageView = cell.imageView;
#ifndef NATIVE_TEST		
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);		
		imageView.image = [UIImage imageWithCGImage:imageResource->image];
#endif		
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
	
	return MAW_RES_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
