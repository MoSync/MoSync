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
	int handle;
	
	IWidget* parent;
	NSMutableArray* children;
	
	int fillWidth, fillHeight;
}

- (void)setParent:(IWidget*) parent;
- (void)setWidgetHandle:(int) handle;
- (void)dealloc;
- (id)init;
- (UIView*)getView;

- (void)addChild: (IWidget*)child;
- (void)addChild: (IWidget*)child andSubview:(bool)addSubview;
- (void)removeChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview;

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

- (void)layout;

@end
