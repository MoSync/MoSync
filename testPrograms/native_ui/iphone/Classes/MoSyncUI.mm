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
#import "WindowWidget.h"
#import "ReflectionWidget.h"

@implementation MoSyncUI

NSMutableArray* widgetArray;
UIView* mainWindow;

- (IWidget*)getWidget: (MAHandle) handle {
	IWidget *widget = nil;
	widget = [widgetArray objectAtIndex:(NSUInteger)handle];
	return widget;
}

- (id)init {
	[super init];
	widgetArray = [[NSMutableArray alloc] init];
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
	} else {
		created = [[ReflectionWidget alloc] initWithName:name];
	}
	
	if(created == nil) return 0;
	
	[widgetArray addObject:created];
	return widgetArray.count-1;
	
}

- (void) removeWidget: (MAHandle) handle {
}

@end
