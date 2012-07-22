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
    INNativeUILog;
	UIViewController* c = [[ScreenWidgetController alloc] init];
	return [self initWithController:c];
}

- (id)initWithController:(UIViewController*)_controller {
    controller = _controller;
    self.view = [controller.view retain];
    self = [super init];
    if (self)
    {
        controller.title = @"";
        self.view.autoresizesSubviews = YES;
    }
	return self;
}

- (void)dealloc {
    [controller release];
    [super dealloc];
}

/**
 * Adds an widget to the end of the children list.
 * A ScreenWidget can have only one child.
 * @param child Widget to be added.
 * @return MAW_RES_OK for success, MAW_RES_ERROR if the screen already has an child.
 */
- (int)addChild:(IWidget*)child
{
    INNativeUILog;
    if ([_children count] > 0)
    {
        return MAW_RES_ERROR;
    }

	[super addChild:child toSubview:YES];
    OUTNativeUILog;
    return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {

	if([key isEqualToString:@MAW_SCREEN_TITLE]) {
		controller.title = value;
	}
	else if([key isEqualToString:@MAW_SCREEN_ICON]) {
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

/**
 * Set child's size.
 */
-(void) layout
{
    INNativeUILog;
    // Check if the screen has child.
    if ([_children count] == 0)
    {
        return;
    }

    IWidget* child = [_children objectAtIndex:0];
    child.size = self.size;
    OUTNativeUILog;
}

@end
