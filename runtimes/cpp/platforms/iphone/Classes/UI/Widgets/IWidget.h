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

// If the value is negative it returns MAW_RES_INVALID_PROPERTY_VALUE constant.
#define TEST_FOR_NEGATIVE_VALUE(value) if (0 > value)\
                                       {\
                                            return  MAW_RES_INVALID_PROPERTY_VALUE; \
                                       }

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AbstractLayoutView.h"

typedef enum {
	FIXED_SIZE,
	FILL_PARENT,
	WRAP_CONTENT
} AutoSizeParam;

@interface IWidget : NSObject {
	UIView* view;
	int handle;
	
	IWidget* parent;
	NSMutableArray* children;
	
	AutoSizeParam autoSizeParamX;
	AutoSizeParam autoSizeParamY;	
	
}

// this generates a wrapper that routes the layoutSubviews and sizeThatFits commands to an IWidget
// make sure not to call the IWidget::view.layoutSubviews or IWidget::view.sizeThatFits from the function  but the super* functions..
#define MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSyncName, UIViewName) \
@interface MoSyncName ## UIViewName : UIViewName {\
IWidget* mWidget;\
}\
- (void)setWidget:(IWidget*)widget;\
- (void)layoutSubviews;\
- (void)superLayoutSubviews;\
- (CGSize)sizeThatFits:(CGSize)size;\
@end\
@implementation MoSync##UIViewName \
- (void)setWidget:(IWidget*)widget { \
	mWidget = widget; \
}\
- (void)layoutSubviews {\
	[mWidget layoutSubviews:self];\
}\
- (CGSize)sizeThatFits:(CGSize)size {\
	return [mWidget sizeThatFitsFor:(UIView*)self withSize:size];\
}\
- (void)superLayoutSubviews {\
	[super layoutSubviews];\
}\
- (CGSize)superSizeThatFits:(CGSize)size {\
	return [super sizeThatFits:size];\
}\
@end\

- (void)setAutoSizeParamX:(AutoSizeParam)x andY:(AutoSizeParam)y;
- (AutoSizeParam)getAutoSizeParamX;
- (AutoSizeParam)getAutoSizeParamY;

// override this if you want a special behaviour...
- (void)layoutSubviews:(UIView*)view;
- (CGSize)sizeThatFitsFor:(UIView*)view withSize:(CGSize)size;

- (void)setParent:(IWidget*) parent;
- (IWidget*)getParent;
- (void)setWidgetHandle:(int) handle;
- (int)getWidgetHandle;
- (void)dealloc;
- (id)init;
- (UIView*)getView;

- (void)addChild: (IWidget*)child;
- (void)addChild: (IWidget*)child toSubview:(bool)toSubview;
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index toSubview:(bool)addSubview;
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index;

- (int)remove;
- (void)removeChild: (IWidget*)child;
- (void)removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview;

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;

- (void)layout;

// when a root screen is shown, this will be called recursively for all widgets.
- (void)show;

@end
