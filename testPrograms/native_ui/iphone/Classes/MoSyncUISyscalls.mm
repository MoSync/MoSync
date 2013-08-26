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
//  MoSyncUISyscalls.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
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
	
	[retval getCString:value maxLength:length encoding:NSUTF8StringEncoding];
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

