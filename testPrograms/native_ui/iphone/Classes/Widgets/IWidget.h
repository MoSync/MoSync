//
//  IWidget.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define MA_WIDGET_ERROR -1
#define MA_WIDGET_OK 0

@interface IWidget : NSObject {
	UIView* view;
}

- (void)dealloc;
- (id)init;
- (UIView*)getView; 
- (void)addChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

@end
