//
//  HorizontalLayoutWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "HorizontalLayoutWidget.h"
#import "LayoutManagers.h"


@implementation HorizontalLayoutWidget

- (id)init {
	//view = [[HLayoutView alloc] init];	
	
	view = [[HLayoutView alloc]  initWithFrame:CGRectZero spacing:0
			 leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
			 hAlignment:UIControlContentHorizontalAlignmentLeft
									vAlignment:UIControlContentVerticalAlignmentTop];
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
	[view setSize];		
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
