/*
Copyright (C) 2010 MoSync AB

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

//
//  NavScreenWidget.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import <Foundation/Foundation.h>
#import "ScreenWidget.h"

@interface NavScreenWidget : ScreenWidget {
}

- (id)init;
- (void)addChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;
- (UIViewController*) getController;

@end
