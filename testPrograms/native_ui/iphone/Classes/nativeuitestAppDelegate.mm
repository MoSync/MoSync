//
//  nativeuitestAppDelegate.m
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/25/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "nativeuitestAppDelegate.h"
#import "MoSyncUISyscalls.h"

@implementation nativeuitestAppDelegate

const char* labels[] = {
		"alpha",
		"beta",
		"gamma",
		"delta",
		"epsilon",
		"zeta",
		"delta"
};

void TestApp() {
	MAHandle tabScreen = maWidgetCreate("TabScreen");	
	
	MAHandle firstScreen = maWidgetCreate("Screen");
	MAHandle tableView = maWidgetCreate("ListView");
	maWidgetAddChild(firstScreen, tableView);
	maWidgetSetProperty(firstScreen, "title", "first");		

	//maWidgetSetProperty(tableView, "width", "80");
	
	for(int i = 0; i < 7; i++) {
		MAHandle tableViewCell = maWidgetCreate("ListViewItem");
		//maWidgetSetProperty(tableViewCell, "text", labels[i]);
		if(i == 3) {
			maWidgetSetProperty(tableViewCell, "backgroundColor", "#00ff00");
			maWidgetSetProperty(tableViewCell, "accessoryType", "isChecked");
		}
		
		maWidgetAddChild(tableView, tableViewCell);		
		
		MAHandle layout = maWidgetCreate("HorizontalLayout");
		maWidgetSetProperty(layout, "width", "200");
		maWidgetSetProperty(layout, "height", "40");
		
		MAHandle button = maWidgetCreate("Button");
		maWidgetSetProperty(button, "text", labels[i]);				
		maWidgetAddChild(layout, button);
		
		MAHandle label = maWidgetCreate("Label");
		maWidgetSetProperty(label, "text", labels[i]);		
		maWidgetAddChild(layout, label);
		
		maWidgetAddChild(tableViewCell, layout);
	
	}
	
	MAHandle tabScreen2 = maWidgetCreate("NavScreen");
	maWidgetSetProperty(tabScreen2, "title", "hoho");		
	
	MAHandle secondScreen = maWidgetCreate("Screen");	
	maWidgetSetProperty(secondScreen, "title", "second");		
	MAHandle button = maWidgetCreate("Button");
	maWidgetSetProperty(button, "left", "20");			
	maWidgetSetProperty(button, "top", "0");			

	maWidgetSetProperty(button, "width", "120");			
	maWidgetSetProperty(button, "text", "Big button!");				
	maWidgetAddChild(secondScreen, button);
	
	MAHandle thirdScreen = maWidgetCreate("Screen");	
	maWidgetSetProperty(thirdScreen, "title", "third");		
	MAHandle button2 = maWidgetCreate("Button");
	maWidgetSetProperty(button2, "left", "20");			
	maWidgetSetProperty(button2, "top", "0");			
	
	maWidgetSetProperty(button2, "width", "120");			
	maWidgetSetProperty(button2, "text", "Big button2!");				
	maWidgetAddChild(thirdScreen, button2);
	
	
	//MAHandle button = maWidgetCreate("UIButton");
	//maWidgetAddChild(tableView, button);
	
	maWidgetAddChild(tabScreen, firstScreen);
	maWidgetAddChild(tabScreen2, secondScreen);	
	maWidgetAddChild(tabScreen2, thirdScreen);	
	maWidgetAddChild(tabScreen, tabScreen2);
	
	maWidgetScreenShow(tabScreen);	
}

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
	initMoSyncUISyscalls(NULL, NULL);
	TestApp();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
     [super dealloc];
}


@end
