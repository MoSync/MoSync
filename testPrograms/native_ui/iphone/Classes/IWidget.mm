//
//  IWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "IWidget.h"

@implementation IWidget

- (id)init {
	[super init];	
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
	return MA_WIDGET_ERROR;
}

- (NSString*) getPropertyWithKey: (NSString*)key {
	
	return nil;
}

- (void)dealloc {
    [super dealloc];
	[view dealloc];
}

@end
