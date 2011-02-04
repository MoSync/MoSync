//
//  NavBarWidget.h
//  nativeuitest
//
//  Created by Magnus Hult <magnus@magnushult.se> on 01/23/11.
//  Copyright 2011 MoSync. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface NavBarWidget : IWidget {
	UINavigationItem* prevNavitem;
	UINavigationItem* currNavitem;
}

- (id)init;
- (void)addChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

- (void)navigationBar:(UINavigationBar *)navigationBar didPopItem:(UINavigationItem *)item;
- (void)navigationBar:(UINavigationBar *)navigationBar didPushItem:(UINavigationItem *)item;
- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item;
- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPushItem:(UINavigationItem *)item;
@end
