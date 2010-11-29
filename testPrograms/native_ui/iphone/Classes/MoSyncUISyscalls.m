//
//  MoSyncUISyscalls.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MoSyncUISyscalls.h"

MoSyncUI* mosyncUI;

void initMoSyncUISyscalls() {
	mosyncUI = [[MoSyncUI alloc] init];
}

NSString* stringFromChar(const char* str) {
	NSString *sfc = [NSString stringWithCString:str length:strlen(str)];  	
	return sfc;
}

MAHandle maWidgetCreate(const char *widgetType) {
	return [mosyncUI createWidget:stringFromChar(widgetType)];
}

void maWidgetDestroy(MAHandle handle) {
}

int maWidgetSetProperty(MAHandle handle, const char *property, const char* value) {
	IWidget* widget = [mosyncUI getWidget:handle];
	return [widget setPropertyWithKey:stringFromChar(property) toValue:stringFromChar(value)];	
}

// if maWidgetGetProperty returns <0 maxSize equals the size needed.
int maWidgetGetProperty(MAHandle handle, const char *property, char *value, int* maxSize) {
	IWidget* widget = [mosyncUI getWidget:handle];	
	NSString* retval = [widget getPropertyWithKey:stringFromChar(property)];
	int length = *maxSize;
	int realLength = [retval length];
	if(realLength > length) {
		*maxSize = realLength;
		return -1;
	}
	
	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	return realLength;
}

void maWidgetAddChild(MAHandle parentHandle, MAHandle childHandle) {
	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];
	[parent addChild:child];
}

void maWidgetRemoveChild(MAHandle parent, MAHandle child) {
}

void maWidgetScreenShow(MAHandle screenHandle) {
	IWidget* screen = [mosyncUI getWidget:screenHandle];
	UIView* view = [screen getView];
	if([view class] == [UIWindow class]) {
		UIWindow* window = (UIWindow*)view;
		[window makeKeyAndVisible];
	}
}
