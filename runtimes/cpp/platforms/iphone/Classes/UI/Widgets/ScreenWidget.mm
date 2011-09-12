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

#import "ScreenWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>

@implementation ScreenWidget

- (id)init {
	UIViewController* c = [[ScreenWidgetController alloc] init];
	return [self initWithController:c];
}

- (id)initWithController:(UIViewController*)_controller {
	controller = _controller;
	controller.title = @"";
	view = controller.view;
	return [super init];
}

- (void)dealloc {
    [controller release];
    [super dealloc];
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {

	if([key isEqualToString:@"title"]) {
		controller.title = value;
	}
	else if([key isEqualToString:@"icon"]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		[controller.tabBarItem setImage:[UIImage imageWithCGImage:imageResource->image]];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {

	return [super getPropertyWithKey:key];
}

- (UIViewController*) getController {
	return controller;
}

- (void)layout {
	int viewWidth = view.frame.size.width;
	int viewHeight = view.frame.size.height;

//	if(!parent)
//		view.frame = [[UIScreen mainScreen] bounds];

	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		[childView setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];

		[child layout];
	}

	[view setNeedsLayout];
}

@end
