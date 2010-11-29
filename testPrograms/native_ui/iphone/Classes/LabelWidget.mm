//
//  LabelWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LabelWidget.h"


@implementation LabelWidget

- (id)init {
	[super init];
	
	UILabel* label = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] autorelease];
	label.opaque = NO;
	view = label;		
	
	return self;
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UILabel* label = (UILabel*) view;
		[label setText: value];
		return MA_WIDGET_OK;
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
