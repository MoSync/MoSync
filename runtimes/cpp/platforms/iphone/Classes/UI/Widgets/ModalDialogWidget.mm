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
#define NAVIGATION_BAR_HEIGHT 50

#import "ModalDialogWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>
#include "MoSyncUISyscalls.h"


@implementation ModalDialogWidget

- (id)init {
	id ret = [super init];
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
		CGRect screenSize = [[UIScreen mainScreen] bounds];
        mModalViewController = [[UIViewController alloc] init];

        // Will contain the navigation bar and the view where the widgets will be added.
        UIView* bigView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, screenSize.size.width, screenSize.size.height)];
        bigView.backgroundColor = [UIColor grayColor];
        mModalViewController.view = bigView;

        // Add navigation bar to the big view.
        UINavigationBar* navigationBar = [[UINavigationBar alloc] initWithFrame:
                                          CGRectMake(0, 0, bigView.frame.size.width, NAVIGATION_BAR_HEIGHT)];
        [bigView addSubview:navigationBar];

        // Add navigation item to navigation bar.
        mNavigationItem = [[UINavigationItem alloc] init];
        [navigationBar pushNavigationItem:mNavigationItem animated:NO];
        [navigationBar release];

        // Add a small view where the widgets will be added.
        UIView* smallView = [[UIView alloc] initWithFrame:
                             CGRectMake(0,
                                        NAVIGATION_BAR_HEIGHT,
                                        screenSize.size.width,
                                        screenSize.size.height - NAVIGATION_BAR_HEIGHT)];
        [bigView addSubview:smallView];
        view = smallView;
        [bigView release];

	}
	else {
		top = 0;
		left = 0;
		dismissable = YES;
		direction = UIPopoverArrowDirectionAny;
		container = [[UINavigationController alloc] initWithRootViewController:controller];
		popoverController = [[UIPopoverController alloc] initWithContentViewController:container];
		popoverController.delegate = self;
	}

    return ret;
}

- (int)show {
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
		int viewWidth = view.frame.size.width;
        int viewHeight = view.frame.size.height;

        for (IWidget *child in children)
        {
            UIView* childView = [child getView];
            [childView setFrame:CGRectMake(0, 0, viewWidth, viewHeight)];

            [child layout];
        }

        [view setNeedsLayout];

        MoSyncUI* mosyncUI = getMoSyncUI();
        [mosyncUI showModal:(UINavigationController*)mModalViewController];
	}
	else {
        [super show];
		IWidget* shownScreen = [getMoSyncUI() getCurrentlyShownScreen];

		[popoverController presentPopoverFromRect:CGRectMake(left,top,0,0)
										   inView:[shownScreen getView]
						 permittedArrowDirections:direction
										 animated:YES];
	}
	return MAW_RES_OK;
}

- (int)hide {
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
        [mModalViewController dismissModalViewControllerAnimated:YES];
	}
	else {
		[popoverController dismissPopoverAnimated:YES];
	}
	return MAW_RES_OK;
}

- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	int res;
	if([key isEqualToString:@MAW_WIDGET_LEFT]) {
		left = [value intValue];
	} else if([key isEqualToString:@MAW_WIDGET_TOP]) {
		top = [value intValue];
	} else if([key isEqualToString:@MAW_MODAL_DIALOG_TITLE]) {
		if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
            mNavigationItem.title = value;
		}
		else {
			[controller.title release];
			controller.title = value;
		}
	}
//        else if([key isEqualToString:@MAW_DIALOG_ARROW_POSITION]) {
//		int msDirection = [value intValue];
//		direction = 0;
//		if (msDirection & MAW_CONSTANT_ARROW_UP) {
//			direction |= UIPopoverArrowDirectionUp;
//		}
//		if (msDirection & MAW_CONSTANT_ARROW_DOWN) {
//			direction |= UIPopoverArrowDirectionDown;
//		}
//		if (msDirection & MAW_CONSTANT_ARROW_LEFT) {
//			direction |= UIPopoverArrowDirectionLeft;
//		}
//		if (msDirection & MAW_CONSTANT_ARROW_RIGHT) {
//			direction |= UIPopoverArrowDirectionRight;
//		}
//	} else if([key isEqualToString:@MAW_DIALOG_USER_CAN_DISMISS]) {
//		if ([value isEqualToString:@"true"]){
//			dismissable = YES;
//		}
//		else {
//			dismissable = NO;
//		}
//
//	}
    else {
		res = [super setPropertyWithKey:key toValue:value];
		if (UIUserInterfaceIdiomPhone != UI_USER_INTERFACE_IDIOM()) {
			if ([key isEqualToString:@MAW_WIDGET_WIDTH] || [key isEqualToString:@MAW_WIDGET_HEIGHT]) {
				[popoverController setPopoverContentSize:view.frame.size animated:YES];
			}
		}
	}
	return res;
}

/**
 * Returns a property value of the modal dialog/popover widget.
 * @param key The property of the modal dialog/popover widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    if ([key isEqualToString:@MAW_MODAL_DIALOG_TITLE])
    {
        return [mNavigationItem.title retain];
    }
	return [super getPropertyWithKey:key];
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
//	[super sendEvent:MAW_EVENT_DIALOG_DISMISSED];
}

- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController {
	return dismissable;
}

- (void)dealloc {
	[popoverController release];
    [mModalViewController release];
    [mNavigationItem release];

	[super dealloc];
}

@end
