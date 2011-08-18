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
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, UITableViewCell)

@implementation ListViewItemWidget

- (id)init {

	static NSString *SimpleTableIdentifier = @"SimpleTableIdentifier";
	MoSyncUITableViewCell *cell = nil;
	cell = [[[MoSyncUITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault
							   reuseIdentifier:SimpleTableIdentifier] retain];
	[cell setWidget:self];
	cell.selectionStyle =  UITableViewCellSelectionStyleNone;
	view = cell;
	[self setPropertyWithKey:@"backgroundColor" toValue:@"00000000"];
	return [super init];		
}

- (void)addChild: (IWidget*)child {
//	[super addChild:child];
	UITableViewCell *cell = (UITableViewCell*)view;
	[cell.contentView addSubview: [child getView]];
	[super addChild:child toSubview:NO];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UITableViewCell* cell = (UITableViewCell*) view;
		UILabel* label = cell.textLabel;
		label.text = value;
		[self layout];
	} 
	else if([key isEqualToString:@"icon"]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_HANDLE;
		UITableViewCell* cell = (UITableViewCell*) view;
		UIImageView* imageView = cell.imageView;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);		
		imageView.image = [UIImage imageWithCGImage:imageResource->image];
	}
	else if([key isEqualToString:@"backgroundColor"]) {
		UITableViewCell *cell = (UITableViewCell*)view;
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		
		cell.contentView.backgroundColor = color;
		cell.textLabel.backgroundColor = color;
		cell.accessoryView.backgroundColor = color;		
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
		else if([value isEqualToString:@"none"]) {
			cell.accessoryType = UITableViewCellAccessoryNone;			
		} else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
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
