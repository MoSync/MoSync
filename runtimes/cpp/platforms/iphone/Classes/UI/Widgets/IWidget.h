//
//  IWidget.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AbstractLayoutView.h"

#define MA_WIDGET_ERROR -1
#define MA_WIDGET_OK 0

@interface AbstractLayoutView (AbstractLayoutViewExpanded)
- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va;
- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha;
@end


@interface IWidget : NSObject {
	UIView* view;
	int handle;
	
	IWidget* parent;
	NSMutableArray* children;
	
	int fillWidth, fillHeight;
}

- (void)setParent:(IWidget*) parent;
- (void)setWidgetHandle:(int) handle;
- (int)getWidgetHandle;
- (void)wasCreated; // do stuff after the handle has been set (temporary hack). 
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
