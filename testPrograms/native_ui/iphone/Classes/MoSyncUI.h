//
//  MoSyncUI.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface MoSyncUI : NSObject {

}

- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller;
- (void)close;
- (void)createWidget: (NSString*)name; // increasing handles beginning at 0
- (IWidget*)getWidget: (int) handle;
- (void)removeWidget: (IWidget*) handle;
- (void)addChild: (IWidget*)child toParent:(IWidget*)parent;
- (void)show: (IWidget*) handle;

@end
