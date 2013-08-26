/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//
//  NavScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import "NavScreenWidget.h"


@implementation NavScreenWidget

- (id)init {
    //view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	UINavigationController* navigationController = [[UINavigationController alloc] init];
	controller = navigationController;
	navigationController.viewControllers = [NSArray array];	
	//view = controller.view;
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
	UINavigationController* navigationController = (UINavigationController*)controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:navigationController.viewControllers];
	[newItems addObject:[screen getController]];
	navigationController.viewControllers = newItems;
	
	//[super addChild:child];
	[super addChild:child andSubview:NO];
}

- (void)removeChild: (IWidget*)child {	
	[super removeChild:child fromSuperview:NO];
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
