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
