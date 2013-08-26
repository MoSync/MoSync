/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//
//  IWidget.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
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
