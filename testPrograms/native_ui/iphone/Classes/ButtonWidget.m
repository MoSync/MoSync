//
//  ButtonWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ButtonWidget.h"


@implementation ButtonWidget

- (id)init {
	view = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	view.frame = CGRectMake(0, 0, 100, 40);
//	view = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 30, 30)];
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UIButton* label = (UIButton*) view;
		[label setTitle:value forState:UIControlStateNormal];
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
