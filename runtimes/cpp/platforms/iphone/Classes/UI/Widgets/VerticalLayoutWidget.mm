//
//  VerticalLayoutWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "VerticalLayoutWidget.h"
#import "LayoutManagers.h"

@implementation VerticalLayoutWidget

- (id)init {
	//view = [[VLayoutView alloc] init];
	
	view = [[VLayoutView alloc]  initWithFrame:CGRectZero spacing:0
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
	if([key isEqualToString:@"childVerticalAlignment"]) {
		VLayoutView* vlv = (VLayoutView*)view;
		if([value isEqualToString:@"top"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentTop];
		else if([value isEqualToString:@"center"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentCenter];
		else if([value isEqualToString:@"bottom"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentBottom];		
	} else if([key isEqualToString:@"childHorizontalAlignment"]) {
		VLayoutView* vlv = (VLayoutView*)view;
		if([value isEqualToString:@"left"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
		else if([value isEqualToString:@"center"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
		else if([value isEqualToString:@"right"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentRight];		
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
