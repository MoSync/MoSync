//
//  MoSyncUISyscalls.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MoSyncUISyscalls.h"
#import "NSObjectExpanded.h"

#import "GLViewWidget.h"

MoSyncUI* mosyncUI;

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController) {
	mosyncUI = [[MoSyncUI alloc] initWithWindow:window andController:viewController];
}

NSString* stringFromChar(const char* str) {
	NSString *sfc = [NSString stringWithUTF8String:str]; 
	return sfc;
}

int currentWidgetIndex = 0;
MAHandle maWidgetCreate(const char *widgetType) {
	[NSObject performSelectorOnMainThread:@selector(createWidget:)
							   withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: stringFromChar(widgetType), nil] 
							waitUntilDone:YES];
	currentWidgetIndex++;
	return currentWidgetIndex-1;
}

void maWidgetDestroy(MAHandle handle) {
}

int maWidgetSetProperty(MAHandle handle, const char *property, const char* value) {
	IWidget* widget = [mosyncUI getWidget:handle];
	
	NSString* propertyString = stringFromChar(property);
	
	if([widget class] == [GLViewWidget class]) {
		// do this from the MoSync thread. Maybe do a generic system for this later.
		if([propertyString isEqualToString:@"bind"]) {
			[widget setPropertyWithKey:@"bind" toValue:@""];
			return 0;
		}
		
		if([propertyString isEqualToString:@"invalidate"]) {
			[widget setPropertyWithKey:@"invalidate" toValue:@""];
			return 0;
		}	
	}
	
	[NSObject performSelectorOnMainThread:@selector(setPropertyWithKey:toValue:)
							   withTarget:widget
							  withObjects:[NSArray arrayWithObjects: propertyString, stringFromChar(value), nil]
							waitUntilDone:YES];
	
	return 0;
}

// if maWidgetGetProperty returns <0 maxSize equals the size needed.
int maWidgetGetProperty(MAHandle handle, const char *property, char *value, int maxSize) {
	IWidget* widget = [mosyncUI getWidget:handle];	
	NSString* retval = [widget getPropertyWithKey:stringFromChar(property)];
	if(retval == nil) return -1;
	int length = maxSize;
	int realLength = [retval length];
	if(realLength > length) {
		return -1;
	}
	
	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	return realLength;
}

int maWidgetAddChild(MAHandle parentHandle, MAHandle childHandle) {
	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];
	[NSObject performSelectorOnMainThread:@selector(addChild:)
							 withTarget:parent
							withObjects:[NSArray arrayWithObjects: child, nil] 
						  waitUntilDone:YES];	
	return 0;
}

int maWidgetRemoveChild(MAHandle parent, MAHandle child) {
	
	return -1;
}

static bool sNativeUIEnabled = false;
bool isNativeUIEnabled() {
	return sNativeUIEnabled;
}

void maWidgetScreenShow(MAHandle screenHandle) {
	sNativeUIEnabled = true;
	
	IWidget* screen = [mosyncUI getWidget:screenHandle];
	[NSObject performSelectorOnMainThread:@selector(show:)
								withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: screen, nil] 
							waitUntilDone:YES];	

}

int maWidgetEvaluateScript(MAHandle widget, const char* script) {
	return 0;
}

int maWidgetGetMessageData(int messageId, void* buffer, int bufferSize) {
	return 0;
}

