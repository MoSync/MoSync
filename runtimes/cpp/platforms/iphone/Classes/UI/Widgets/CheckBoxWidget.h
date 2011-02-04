//
//  CheckBoxWidget.h
//  nativeuitest
//
//  Created by Magnus Hult <magnus@magnushult.se> on 2011-01-20.
//  Copyright 2010 MoSync AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface CheckBoxWidget : IWidget {
}

- (id)init;
- (void)addChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

@end
