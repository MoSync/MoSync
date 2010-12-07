//
//  ScreenWidget.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"
#import "ScreenWidgetController.h"

@interface ScreenWidget : IWidget {
	ScreenWidgetController* controller;
}

- (id)init;
- (void)addChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;
- (ScreenWidgetController*) getController;

@end
