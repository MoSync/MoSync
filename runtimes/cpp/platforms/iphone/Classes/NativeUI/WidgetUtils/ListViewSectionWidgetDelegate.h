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
 * @file ListViewSectionWidgetDelegate.h
 * @author Bogdan Iusco
 *
 * @brief ListViewSection widget delegate.
 * Used by ListViewItemWidget objects to notify when their size has changed.
 */

#import <Foundation/Foundation.h>

@class ListViewItemWidget;

@protocol ListViewSectionWidgetDelegate <NSObject>

@required
/**
 * Notify that an list item's size has changed.
 * @param listItem List item that changed its size.
 */
-(void)sizeChangedFor:(ListViewItemWidget*) listItem;

/**
 * Ask the list section for the item's height.
 * @return The item height set in the table view.
 */
- (CGFloat)tableViewSectionRowHeight;

@end
