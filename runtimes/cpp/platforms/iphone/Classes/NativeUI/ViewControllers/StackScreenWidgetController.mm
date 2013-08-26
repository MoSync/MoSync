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

/**
 * @file StackScreenWidgetController.mm
 * @author Bogdan Iusco
 *
 * @brief View controller for StackScreenWidget.
 * Used to set orientation in iOS 6 and later.
 */

#import "StackScreenWidgetController.h"
#import "ScreenOrientation.h"

@implementation StackScreenWidgetController

/**
 * Returns whether the view controller’s contents should auto rotate.
 * Available in iOS 6.0 and later.
 * @return YES.
 */
- (BOOL)shouldAutorotate
{
    return YES;
}

/**
 * Returns all of the interface orientations that the view controller supports.
 * Available in iOS 6.0 and later.
 * @return A mask with supported orientations.
 */
- (NSUInteger)supportedInterfaceOrientations
{
    UIInterfaceOrientationMask orientations = [[ScreenOrientation getInstance] supportedInterfaceOrientations];
    return orientations;
}

@end
