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

//Custom UIView to be used with the Popover.
//Based on the MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION macro
//that I couldn't get to work.
//No need to override any IWidget methods, works with the default ones
@interface MoSyncPopoverView : UIView {
	IWidget* mWidget;
}
- (void)setWidget:(IWidget*)widget;
- (void)layoutSubviews;
- (void)superLayoutSubviews;
@end

@implementation MoSyncPopoverView 
- (void)setWidget:(IWidget*)widget { 
	mWidget = widget; 
}
- (void)layoutSubviews {
	[mWidget layoutSubviews:self];
}

- (void)superLayoutSubviews {
	[super layoutSubviews];
}

@end

@implementation ModalDialogWidget

- (id)init {
	id ret = [super init];
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) { //The Modal View code for iPhone
		CGRect screenSize = [[UIScreen mainScreen] bounds];
        mModalViewController = [[UIViewController alloc] init];

        // Will contain the navigation bar and the view where the widgets will be added.
        UIView* bigView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, screenSize.size.width, screenSize.size.height)];

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
	else {//The Popover code for iPad
		//First, replace the default vanilla view with MoSyncPopoverView
		[view release];
		view = [[MoSyncPopoverView alloc] initWithFrame:CGRectMake(0, 0, 10, 10)];
		controller.view = view;
		[(MoSyncPopoverView*)view setWidget:self];
		//Default position is on the top left corner
		top = 0;
		left = 0;
		//width and height: more or less the iPhone dimentions (minus the navbar) in order to make
		//it analogus to the iPhone modal screen. But the user can change them anyway.
		[self setPropertyWithKey:@MAW_WIDGET_WIDTH toValue:@"320"];
		[self setPropertyWithKey:@MAW_WIDGET_HEIGHT toValue:@"450"];
		dismissable = YES;
		direction = UIPopoverArrowDirectionAny;
		autoSizeParamX = FIXED_SIZE;
		autoSizeParamY = FIXED_SIZE;
		view.backgroundColor = [UIColor whiteColor];
		//The navigation controller is needed for the title bar to appear
		container = [[UINavigationController alloc] initWithRootViewController:controller];
		popoverController = [[UIPopoverController alloc] initWithContentViewController:container];
		popoverController.delegate = self;

		//Hide the title bar by default
		container.navigationBarHidden = YES;
	}

    return ret;
}

- (int)show {
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {//Show a Modal View for iPhone
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
	else { //Show a Popover for iPad
        [self layout];

		//if the popover should wrap to the content, try to find the correct size
		int newWidth = 0;
		int newHeight = 0;
		for (IWidget *child in children)
		{
			[child show];
			if ([self getAutoSizeParamX] == WRAP_CONTENT) {
				int childWidth = [child getView].frame.origin.x + [child getView].frame.size.width;
				if(childWidth > newWidth){
					newWidth = childWidth;
				}
			}
			if ([self getAutoSizeParamY] == WRAP_CONTENT) {
				int childHeight = [child getView].frame.origin.y + [child getView].frame.size.height;
				if(childHeight > newHeight){
					newHeight = childHeight;
				}
			}
		}

		int viewHeight = view.frame.size.height;
		int viewWidth = view.frame.size.width;

		if([self getAutoSizeParamX] == WRAP_CONTENT) {
			viewWidth = newWidth;
		}
		if([self getAutoSizeParamY] == WRAP_CONTENT) {
			viewHeight = newHeight;
		}

		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];
		//At this point it should have the correct size
		controller.contentSizeForViewInPopover = view.frame.size;

		//The screen that was active when the popover was shown
		IWidget* shownScreen = [getMoSyncUI() getCurrentlyShownScreen];

		//Show the popover
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
	//The popover makes use of the "left" and "top" properties
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
			//Hide or show the title based ont he string
			if ([controller.title isEqualToString:@""]){
				container.navigationBarHidden = YES;
			}
			else {
				container.navigationBarHidden = NO;
			}

		}
	} else if([key isEqualToString:@MAW_MODAL_DIALOG_ARROW_POSITION]) {
		int msDirection = [value intValue];
		direction = 0;
		//Handles both single position cases, as well as the user
		//using | between a few
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
	} else if([key isEqualToString:@MAW_MODAL_DIALOG_USER_CAN_DISMISS]) {
		//Used by the popover only, whether the user can dismiss the popover
		//by tapping outside or not
		if ([value isEqualToString:@"true"]){
			dismissable = YES;
		}
		else {
			dismissable = NO;
		}

	}
    else {
		res = [super setPropertyWithKey:key toValue:value];
		if (UIUserInterfaceIdiomPhone != UI_USER_INTERFACE_IDIOM()) {
			if ([key isEqualToString:@MAW_WIDGET_WIDTH] || [key isEqualToString:@MAW_WIDGET_HEIGHT]) {
				//If the user changed the height, we need to change the popover again,
				//because it might already be shown on screen
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
		if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
			return [mNavigationItem.title retain];
		}
		else {
			return [controller.title retain];
		}

    }
	return [super getPropertyWithKey:key];
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
	//The user dismissed the popover by tapping outside of it
	[super sendEvent:MAW_EVENT_DIALOG_DISMISSED];
}

- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController {
	//Prihibit or allow the user to dismiss the popover by tapping outside of it
	return dismissable;
}

- (void)dealloc {
	[popoverController release];
	[container release];
    [mModalViewController release];
    [mNavigationItem release];

	[super dealloc];
}

@end
