//
//  ScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ScreenWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#ifndef NATIVE_TEST
#include "Platform.h"
#include <base/Syscall.h>
#endif


@implementation ScreenWidget

- (id)init {
    //view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	if([self class] == [ScreenWidget class])
		controller = [[ScreenWidgetController alloc] init];
	//controller.view = view;
	controller.title = @"";
	view = controller.view;
	view.bounds = [[UIScreen mainScreen] bounds];
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
//	if(!parent)
//		[child getView].frame = [[UIScreen mainScreen] bounds];
	
	[super addChild:child];	
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {

	if([key isEqualToString:@"title"]) {
		controller.title = value;
	} 
	else if([key isEqualToString:@"icon"]) {
		int imageHandle = [value intValue];
#ifndef NATIVE_TEST
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);		
		/*
		controller.tabBarItem = [[UITabBarItem alloc] initWithTitle:controller.title 
															  image:[UIImage imageWithCGImage:imageResource->image] 
																tag:0];
		*/
		
		[controller.tabBarItem setImage:[UIImage imageWithCGImage:imageResource->image]];	
#endif

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

- (void)layout {
	int viewWidth = view.frame.size.width; 
	int viewHeight = view.frame.size.height; 

	
	//[view setNeedsLayout];
	//[view setNeedsDisplay];
	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		[childView setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, viewWidth, viewHeight)];		
		
		[child layout];
		
	}	
}

@end
