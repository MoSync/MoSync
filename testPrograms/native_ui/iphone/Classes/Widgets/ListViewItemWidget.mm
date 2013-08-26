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
//  ListViewCellWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
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
	else if([key isEqualToString:@"icon"]) {
		int imageHandle = [value intValue];
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
	
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
