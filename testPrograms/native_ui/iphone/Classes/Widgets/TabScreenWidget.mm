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
	UITabBarController* tabBarController = [[[UITabBarController alloc] init] retain];
	controller = tabBarController;
	tabBarController.viewControllers = [NSArray array];	
	//view = controller.view;
	//controller.view = view;
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
	UITabBarController* tabBarController = (UITabBarController*)controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems addObject:[screen getController]];
	tabBarController.viewControllers = newItems;
	
	//[super addChild:child];
	[super addChild:child andSubview:NO];	
}

- (void)removeChild: (IWidget*)child {
	[super removeChild:child fromSuperview:NO];	
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"title"]) {
		controller.title = value;
	} 
	else if([key isEqualToString:@"currentTab"]) {
		UITabBarController* tabBarController = (UITabBarController*)controller;
		tabBarController.selectedViewController = [tabBarController.viewControllers objectAtIndex: [value intValue]]; 
		//tabBarController.selectedIndex = [value intValue];
		//[tabBarController.selectedViewController viewDidAppear:YES];
	}
	else {
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
