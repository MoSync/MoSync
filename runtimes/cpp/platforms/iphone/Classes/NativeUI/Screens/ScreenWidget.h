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
#import "IWidget.h"
#import "ScreenWidgetController.h"

@interface ScreenWidget : IWidget {
	UIViewController* controller;
}

- (id)init;
- (id)initWithController:(UIViewController*)controller;
- (void)dealloc;

/**
 * Adds an widget to the end of the children list.
 * A ScreenWidget can have only one child.
 * @param child Widget to be added.
 * @return MAW_RES_OK for success, MAW_RES_ERROR if the screen already has an child.
 */
- (int)addChild:(IWidget*)child;

/**
 * Set child's size.
 */
-(void) layout;

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;
- (UIViewController*) getController;

@end
