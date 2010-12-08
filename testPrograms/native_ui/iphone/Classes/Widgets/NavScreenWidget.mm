//
//  NavScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NavScreenWidget.h"


@implementation NavScreenWidget

- (id)init {
    //view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	UINavigationController* navigationController = [[UINavigationController alloc] init];
	controller = navigationController;
	navigationController.viewControllers = [NSArray array];	
	view = controller.view;
	//controller.view = view;
	
	return self;
}

- (void)addChild: (IWidget*)child {
	UINavigationController* navigationController = controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:navigationController.viewControllers];
	[newItems addObject:[screen getController]];
	navigationController.viewControllers = newItems;
	
	//[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"title"]) {
		controller.title = value;
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

- (UIViewController*) getController {
	return controller;
}

@end
