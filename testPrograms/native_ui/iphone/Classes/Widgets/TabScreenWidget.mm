//
//  TabScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TabScreenWidget.h"


@implementation TabScreenWidget

- (id)init {
    //view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	UITabBarController* tabBarController = [[UITabBarController alloc] init];
	controller = tabBarController;
	tabBarController.viewControllers = [NSArray array];	
	view = controller.view;
	//controller.view = view;
	
	return self;
}

- (void)addChild: (IWidget*)child {
	UITabBarController* tabBarController = (UITabBarController*)controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems addObject:[screen getController]];
	tabBarController.viewControllers = newItems;
	
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
