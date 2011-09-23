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

#import "PopoverWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>
#include "MoSyncUISyscalls.h"


@implementation PopoverWidget

- (id)init {
	id res = [super init];
	top = 0;
	left = 0;
	dismissable = YES;
	direction = UIPopoverArrowDirectionAny;
	container = [[UINavigationController alloc] initWithRootViewController:controller];
	popoverController = [[UIPopoverController alloc] initWithContentViewController:container];
	popoverController.delegate = self;
	return res;
}

- (int)show {
	[super show];

	IWidget* shownScreen = [getMoSyncUI() getCurrentlyShownScreen];

	[popoverController presentPopoverFromRect:CGRectMake(left,top,0,0)
									   inView:[shownScreen getView]
					 permittedArrowDirections:direction
									 animated:YES];
	return MAW_RES_OK;
}

- (int)hide {
	[popoverController dismissPopoverAnimated:YES];
	return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	int res;
	if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		left = [value intValue];
	} else if([key isEqualToString:@MAW_WIDGET_TOP]) {
		top = [value intValue];
	} else if([key isEqualToString:@MAW_POPOVER_ARROW_POSITION]) {
		int msDirection = [value intValue];
		direction = 0;
		if (msDirection & MAW_CONSTANT_ARROW_UP) {
			direction |= UIPopoverArrowDirectionUp;
		}
		if (msDirection & MAW_CONSTANT_ARROW_DOWN) {
			direction |= UIPopoverArrowDirectionDown;
		}
		if (msDirection & MAW_CONSTANT_ARROW_LEFT) {
			direction |= UIPopoverArrowDirectionLeft;
		}
		if (msDirection & MAW_CONSTANT_ARROW_RIGHT) {
			direction |= UIPopoverArrowDirectionRight;
		}
	} else if([key isEqualToString:@MAW_POPOVER_USER_CAN_DISMISS]) {
		if ([value isEqualToString:@"true"]){
			dismissable = YES;
		}
		else {
			dismissable = NO;
		}

	} else {
		res = [super setPropertyWithKey:key toValue:value];
		if ([key isEqualToString:@MAW_WIDGET_WIDTH] || [key isEqualToString:@MAW_WIDGET_HEIGHT]) {
			[popoverController setPopoverContentSize:view.frame.size animated:YES];
		}
	}
	return res;
}

/*- (NSString*)getPropertyWithKey: (NSString*)key {

	if([key isEqualToString:@MAW_WIDGET_WIDTH]) {
		return [[[NSNumber numberWithInt: view.frame.size.width*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_HEIGHT]) {
		return [[[NSNumber numberWithInt: view.frame.size.height*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		return [[[NSNumber numberWithInt: view.frame.origin.x*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_TOP]) {
		return [[[NSNumber numberWithInt: view.frame.origin.y*getScreenScale()] stringValue] retain];
	}
	else if([key isEqualToString:@MAW_WIDGET_VISIBLE]) {
        return view.hidden ? @"false" : @"true";
    }
    else if([key isEqualToString:@MAW_WIDGET_ENABLED]) {
        UIControl* controller = (UIControl*) view; // TODO: is this correct?
        return controller.enabled ? @"true" : @"false";
    }
	return nil;
}*/

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
	[super sendEvent:MAW_EVENT_POPOVER_DISMISSED];
}

- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController {
	return dismissable;
}

- (void)dealloc {
	[popoverController release];
	[super dealloc];
}

@end
