/*
 Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file TabScreenWidgetController.mm
 * @author Bogdan Iusco
 *
 * @brief View controller for TabScreenWidget.
 * Used to set orientation in iOS 6 and later.
 */

#import "TabScreenWidgetController.h"
#import "ScreenOrientation.h"

@implementation TabScreenWidgetController

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
