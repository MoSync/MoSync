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

#import <Foundation/Foundation.h>
#import "ScreenWidget.h"
#import "ScreenWidgetController.h"

@interface ModalDialogWidget : ScreenWidget <UIPopoverControllerDelegate>{
	UIPopoverController* popoverController;
    UIViewController* mModalViewController;
    UINavigationItem* mNavigationItem;
	UINavigationController* container;
	UIPopoverArrowDirection direction;
	BOOL dismissable;
	int top;
	int left;
}

- (id)init;
- (int)show;
- (int)hide;

/**
 * Sets a modal dialog/popover property.
 * @param key The property of the modal dialog/popover widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

/**
 * Returns a property value of the modal dialog/popover widget.
 * @param key The property of the modal dialog/popover widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key;

- (void)dealloc;
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;
- (BOOL)popoverControllerShouldDismissPopover:(UIPopoverController *)popoverController;

@end
