//
//  IWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "IWidget.h"
#import "UIColor-Expanded.h"
#import <objc/runtime.h>

/*
@interface NSObject (NSObjectMixin) 
- (void) injectMixin: (Class) mixin;
@end

@implementation NSObject (NSObjectMixin) 

- (void) injectMixinOn: (Class)object with:(Class) mixin
{
	NSString *prefix = @"__";
	unsigned count;
	Method *methods = class_copyMethodList(mixin, &count);
	for(int i = 0; i < count; i++) {
		Method method = methods[i];
		SEL name = method_getName(method);
		IMP imp = method_getImplementation(method);
		const char* types = method_getTypeEncoding(method);
		BOOL added = class_addMethod(object, name, imp, types);
		if(!added) {
			IMP originalImp = class_getMethodImplementation(object, name);
			class_replaceMethod(object, name, imp, types);
			SEL originalName = NSSelectorFromString([NSString stringWithFormat:@"%@%s", prefix, name]);
			class_addMethod(object, originalName, originalImp, types);
		}
	}
}
@end
*/

/*
@interface UIView (UIViewExpanded)
- (void) mosynclayoutSubviews;
@end

@implementation UIView (UIViewExpanded)
- (void) mosynclayoutSubviews {
	int newWidth = self.frame.size.width;
	int newHeight = self.frame.size.height;
	
	if(self.autoresizingMask&UIViewAutoresizingFlexibleWidth) {
		newWidth = self.superview.frame.size.width;
	}
	
	if(self.autoresizingMask&UIViewAutoresizingFlexibleHeight) {
		newHeight = self.superview.frame.size.height;
	}
	
	[self setFrame: CGRectMake(self.frame.origin.x, self.frame.origin.y, newWidth, newHeight)];
	//[self setBounds: CGRectMake(0, 0, newWidth, newHeight)];
	
	for (UIView *child in self.subviews)
    {
		[child setNeedsLayout];
	}
	
}
@end

@implementation UISearchBar (UISearchBarExpanded)

- (void) layoutSubviews {
	[super layoutSubviews];			
	[self mosynclayoutSubviews];	
}

@end
*/

@implementation AbstractLayoutView (AbstractLayoutViewExpanded)

- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va {
	vAlignment = va;
}

- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha {
	hAlignment = ha;
}

@end

@implementation IWidget

+ (void)load {
	//[UIView override_layoutSubviews];
	//[UISearchBar override_layoutSubviews];
	//[AbstractLayoutView override_layoutSubviews];
	
	
}

- (id)init {
	[super init];
	view.contentMode = UIViewContentModeRedraw;
	view.autoresizesSubviews = NO;
	parent = nil;
	children = [[NSMutableArray alloc] init];

	//[view injectMixin:[UIViewAdditions class]];

	fillWidth = 0;
	fillHeight = 0;
	[view sizeToFit]; 
	
	return self;
}

- (UIView*) getView {
	return view;
}

- (void) setWidgetHandle:(int) toHandle {
	handle = toHandle;
}

- (int)getWidgetHandle {
	return handle;
}

- (void) setParent:(IWidget*) toParent {
	parent = toParent;
}

- (void) addChild: (IWidget*)child andSubview:(bool)addSubview {
	[child setParent:self];
	[children addObject:child];
	if(addSubview)
		[view addSubview:[child getView]];
}

- (void) addChild: (IWidget*)child {
	[self addChild:child andSubview:YES];	
}

- (void) removeChild: (IWidget*)child {
	[self removeChild:child fromSuperview:YES];
}

- (void) removeChild: (IWidget*)child fromSuperview:(bool)removeFromSuperview {
	[children removeObjectIdenticalTo:child];
	[child setParent:nil];
	if(removeFromSuperview)
		[[child getView] removeFromSuperview];
}


- (int) setPropertyWithKey: (NSString*)key toValue:(NSString*)value {
	if([key isEqualToString:@"left"]) {
//		view.frame.origin.x = [value floatValue];
		[view setFrame:CGRectMake([value floatValue], view.frame.origin.y, view.frame.size.width, view.frame.size.height)];
	} else 
	if([key isEqualToString:@"top"]) {
//		view.frame.origin.y = [value floatValue];
		[view setFrame:CGRectMake(view.frame.origin.x, [value floatValue], view.frame.size.width, view.frame.size.height)];
	} else 
	if([key isEqualToString:@"width"]) {
		float width = [value floatValue];
		if(width == -1) {
			//view.autoresizingMask =	view.autoresizingMask | 
			//UIViewAutoresizingFlexibleWidth;
			width = view.superview.frame.size.width;
			//[view setNeedsLayout];
			fillWidth = -1;
			//return MA_WIDGET_OK;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, width, view.frame.size.height)];
	} else
	if([key isEqualToString:@"height"]) {
		float height = [value floatValue];
		if(height == -1) {
			//view.autoresizingMask =			view.autoresizingMask | 
			//UIViewAutoresizingFlexibleHeight;
			height = view.superview.frame.size.height;
			//[view setNeedsLayout];
			fillHeight = -1;
			//return MA_WIDGET_OK;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, height)];		
	} else 
	if([key isEqualToString:@"backgroundColor"]) {
		view.backgroundColor = [UIColor colorWithHexString:value];
	} else
	if([key isEqualToString:@"alpha"]) {
		view.alpha = [value floatValue];
	} else
	if([key isEqualToString:@"opaque"]){
		view.opaque = [value boolValue];
	} else
	if([key isEqualToString:@"visible"]){
		view.hidden = not [value boolValue];
	} else {
			return MA_WIDGET_ERROR;
	}
			
	return MA_WIDGET_OK;
}

- (NSString*) getPropertyWithKey: (NSString*)key {
	
	if([key isEqualToString:@"width"]) {		
		return [[NSNumber numberWithInt: view.frame.size.width ] stringValue];
		
	}
	else if([key isEqualToString:@"height"]) {
		return [[NSNumber numberWithInt: view.frame.size.height] stringValue];
	}
	else if([key isEqualToString:@"left"]) {		
		return [[NSNumber numberWithInt: view.frame.origin.x ] stringValue];
		
	}
	else if([key isEqualToString:@"top"]) {
		return [[NSNumber numberWithInt: view.frame.origin.y] stringValue];
	}
	
	return nil;
}

- (void)dealloc {
    [super dealloc];
	[view dealloc];
}

- (void)layout {	
	int viewWidth = view.frame.size.width; 
	if(fillWidth == -1) {
		viewWidth = view.superview.frame.size.width;
	}
	
	int viewHeight = view.frame.size.height; 
	if(fillHeight == -1) {
		viewHeight = view.superview.frame.size.height;
	}
	
	[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];
	//[view sizeToFit];
	
	//[view setNeedsLayout];
	[view setNeedsDisplay];
	for (IWidget *child in children)
    {
		[child layout];

	}	
}

@end
