//
//  MoSyncUI.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MoSyncUI.h"
#import "LabelWidget.h"
#import "TableViewWidget.h"
#import "ScreenWidget.h"
#import "ReflectionWidget.h"

@implementation MoSyncUI

NSMutableArray* widgetArray;
UIWindow* mainWindow;
UITabBarController *tabBarController;
//UINavigationController *tabBarController;

- (IWidget*)getWidget: (MAHandle) handle {
	IWidget *widget = nil;
	widget = [widgetArray objectAtIndex:(NSUInteger)handle];
	return widget;
}

- (id)init {
	[super init];
	widgetArray = [[NSMutableArray alloc] init];
	
	mainWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	mainWindow.backgroundColor = [UIColor whiteColor];  		

	
	//tabBarController = [[UITabBarController alloc] init];
    //tabBarController = [[UINavigationController alloc] init];
	
	//tabBarController.viewControllers = [NSArray array];										 	
	//tabBarController.view = mainWindow;

	//[mainWindow addSubview:tabBarController.view];	
	[mainWindow makeKeyAndVisible];
	
	
	return self;
}

- (void) close {
}

- (MAHandle) createWidget: (NSString*)name {
	IWidget *created = nil;
	NSString* realName = [name stringByAppendingString:@"Widget"];
	Class widgetClass = NSClassFromString(realName);
	if(widgetClass != nil) {
		created = [[widgetClass alloc] init];
		
		/*
		if([widgetClass class] == [ScreenWidget class]) {
			//[mainWindow addSubview: [created getView]];
			ScreenWidget* screen = (ScreenWidget*)created;
			NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
			[newItems addObject:[screen getController]];
			tabBarController.viewControllers = newItems;
		}
		*/
		
	} else {
		created = [[ReflectionWidget alloc] initWithName:name];
	}
	
	if(created == nil) return 0;
	
	[widgetArray addObject:created];
	return widgetArray.count-1;
	
}

- (void) removeWidget: (MAHandle) handle {
}

- (void)show: (MAHandle) handle {
	IWidget* widget = [self getWidget: handle];
	[mainWindow addSubview:[widget getView]];	
}

@end
