/* Copyright (C) 2011 MoSync AB
 
 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import "MoSyncUISyscalls.h"
#import "NSObjectExpanded.h"

#import "ScreenWidget.h"
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
MAWidgetHandle maWidgetCreate(const char *widgetType) {
	int returnValue;
	[NSObject performSelectorOnMainThread:@selector(createWidget:)
							   withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: stringFromChar(widgetType), nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	if(returnValue != WIDGET_RES_OK) return returnValue;
	
	currentWidgetIndex++;
	return currentWidgetIndex-1;
}

int maWidgetDestroy(MAWidgetHandle handle) {
	return WIDGET_RES_OK;
}

int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value) {
	IWidget* widget = [mosyncUI getWidget:handle];
	
	NSString* propertyString = stringFromChar(property);
	
	if([widget class] == [GLViewWidget class]) {
		// do this from the MoSync thread. Maybe do a generic system for this later.
		if([propertyString isEqualToString:@"bind"]) {
			return [widget setPropertyWithKey:@"bind" toValue:@""];
		}
		
		if([propertyString isEqualToString:@"invalidate"]) {
			return [widget setPropertyWithKey:@"invalidate" toValue:@""];
		}	
	}
	
	int returnValue;
	
	[NSObject performSelectorOnMainThread:@selector(setPropertyWithKey:toValue:)
							   withTarget:widget
							  withObjects:[NSArray arrayWithObjects: propertyString, stringFromChar(value), nil]
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	return returnValue;
}

// if maWidgetGetProperty returns <0 maxSize equals the size needed.
int maWidgetGetProperty(MAWidgetHandle handle, const char *property, char *value, int maxSize) {
	IWidget* widget = [mosyncUI getWidget:handle];	
	NSString* retval = [widget getPropertyWithKey:stringFromChar(property)];
	if(retval == nil) return WIDGET_RES_ERROR;
	int length = maxSize;
	int realLength = [retval length];
	if(realLength > length) {
		return WIDGET_RES_INVALID_STRING_BUFFER_SIZE;
	}
	
	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	return realLength;
}

int maWidgetAddChild(MAWidgetHandle parentHandle, MAHandle childHandle) {
	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];
	if(!parent) return WIDGET_RES_INVALID_HANDLE;
	if(!child) return WIDGET_RES_INVALID_HANDLE;

	[NSObject performSelectorOnMainThread:@selector(addChild:)
							 withTarget:parent
							withObjects:[NSArray arrayWithObjects: child, nil] 
						  waitUntilDone:YES
						 andReturnValue:nil];	
	return WIDGET_RES_OK;
}

int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index) {

	return WIDGET_RES_ERROR;
}

int maWidgetRemoveChild(MAWidgetHandle childHandle) {
	int returnValue;
	IWidget* child = [mosyncUI getWidget:childHandle];	
	[NSObject performSelectorOnMainThread:@selector(remove:)
							   withTarget:child
							  withObjects:[NSArray arrayWithObjects: nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	if(returnValue == WIDGET_RES_ERROR) {
		// should flip back to MoSync view.
		return WIDGET_RES_REMOVED_ROOT;
	}
	
	return WIDGET_RES_ERROR;
}

static bool sNativeUIEnabled = false;
bool isNativeUIEnabled() {
	return sNativeUIEnabled;
}

int maWidgetScreenShow(MAWidgetHandle screenHandle) {
	sNativeUIEnabled = true;
	
	IWidget* screen = [mosyncUI getWidget:screenHandle];
	
	if(!([screen class] == [ScreenWidget class]) && !([screen superclass] == [ScreenWidget class])) {
		return WIDGET_RES_INVALID_SCREEN;
	}
	
	int returnValue;
	[NSObject performSelectorOnMainThread:@selector(show:)
								withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: screen, nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	return returnValue;
}

