//
//  IWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "IWidget.h"
#import "UIColor-Expanded.h"

@implementation IWidget

- (id)init {
	[super init];
	view.contentMode = UIViewContentModeRedraw;
	parent = nil;
	children = [[NSMutableArray alloc] init];
	return self;
}

- (UIView*) getView {
	return view;
}

- (void) setWidgetHandle:(int) toHandle {
	handle = toHandle;
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
			view.autoresizingMask =	view.autoresizingMask | 
									UIViewAutoresizingFlexibleWidth | 
									UIViewAutoresizingFlexibleLeftMargin | 
									UIViewAutoresizingFlexibleRightMargin;
			//return MA_WIDGET_OK;
			width = view.superview.frame.size.width;
		}
		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, width, view.frame.size.height)];
	} else
	if([key isEqualToString:@"height"]) {
		float height = [value floatValue];
		if(height == -1) {
			view.autoresizingMask =			view.autoresizingMask | 
				UIViewAutoresizingFlexibleHeight | 
				UIViewAutoresizingFlexibleTopMargin | 
				UIViewAutoresizingFlexibleBottomMargin;
			height = view.superview.frame.size.height;
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

@end
