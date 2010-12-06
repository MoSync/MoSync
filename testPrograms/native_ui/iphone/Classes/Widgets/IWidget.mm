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
	return self;
}

- (UIView*) getView {
	return view;
}

- (void) addChild: (IWidget*)child {
	[view addSubview:[child getView]];	
}

- (void) removeChild: (IWidget*)child {
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
//		view.frame.size.width = [value floatValue];		
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, [value floatValue], view.frame.size.height)];
	} else
	if([key isEqualToString:@"height"]) {
		//view.frame.size.height = [value floatValue];
		[view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, [value floatValue])];		
	} else 
	if([key isEqualToString:@"backgroundColor"]) {
		view.backgroundColor = [UIColor colorWithHexString:value];
	} else
	if([key isEqualToString:@"alpha"]) {
		view.alpha = [value floatValue];
	} else {
		return MA_WIDGET_ERROR;
	}
	
	return MA_WIDGET_OK;
}

- (NSString*) getPropertyWithKey: (NSString*)key {
	
	return nil;
}

- (void)dealloc {
    [super dealloc];
	[view dealloc];
}

@end
