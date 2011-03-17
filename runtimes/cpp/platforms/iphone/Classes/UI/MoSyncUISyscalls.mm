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
#import "LayoutWidgets.h"
#import "RelativeLayoutWidget.h"
#import "ListViewItemWidget.h"
#import "GLViewWidget.h"

MoSyncUI* mosyncUI;

MoSyncUI* getMoSyncUI() {
	return mosyncUI;
}

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController) {
	mosyncUI = [[MoSyncUI alloc] initWithWindow:window andController:viewController];
}

NSString* stringFromChar(const char* str) {
	NSString *sfc = [NSString stringWithUTF8String:str]; 
	return sfc;
}

MAWidgetHandle maWidgetCreate(const char *widgetType) {	
//	NSLog(@"maWidgetCreate(%s)\n", widgetType);
	
	int returnValue;
	[NSObject performSelectorOnMainThread:@selector(createWidget:)
							   withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: stringFromChar(widgetType), nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	return returnValue;
}

int maWidgetDestroy(MAWidgetHandle handle) {
	int returnValue;

	IWidget* widget = [mosyncUI getWidget:handle];
	if(widget == NULL) return MAW_RES_INVALID_HANDLE;
	
	bool isCurrentlyShownScreen = widget==[mosyncUI getCurrentlyShownScreen];
	
	[NSObject performSelectorOnMainThread:@selector(destroyWidgetInstance:)
							   withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: widget, nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];

	if(isCurrentlyShownScreen) {
		[NSObject performSelectorOnMainThread:@selector(showMoSyncCanvas)
								   withTarget:mosyncUI
								  withObjects:[NSArray arrayWithObjects: nil] 
								waitUntilDone:YES
							   andReturnValue:nil];
	}

	return returnValue;
}

int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value) {
//	NSLog(@"maWidgetSetProperty(%d, %s, %s)\n", handle, property, value);

	IWidget* widget = [mosyncUI getWidget:handle];
	if(widget == NULL) return MAW_RES_INVALID_HANDLE;	
	
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
	if(!widget) return MAW_RES_INVALID_HANDLE;		
	
// NSString* retval = [widget getPropertyWithKey:stringFromChar(property)];

	NSString* retval;
	[NSObject performSelectorOnMainThread:@selector(getPropertyWithKey:)
							   withTarget:widget
							  withObjects:[NSArray arrayWithObjects:stringFromChar(property), nil]
							waitUntilDone:YES
						   andReturnValue:&retval];
	
	
	if(retval == nil) return MAW_RES_ERROR;
	int length = maxSize;
	int realLength = [retval length];
	if(realLength > length) {
		return MAW_RES_INVALID_STRING_BUFFER_SIZE;
	}
	
	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	return realLength;
}

int maWidgetAddChild(MAWidgetHandle parentHandle, MAHandle childHandle) {
//	NSLog(@"maWidgetAddChild(%d, %d)\n", parentHandle, childHandle);

	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];
	if(!parent) return MAW_RES_INVALID_HANDLE;
	if(!child) return MAW_RES_INVALID_HANDLE;
	if(parent == child) return MAW_RES_ERROR;
	
	if([child getParent] != NULL) return MAW_RES_ERROR;
	
	// ugly.
	if(
	   !([parent class] == [HorizontalLayoutWidget class]) &&
	   !([parent class] == [VerticalLayoutWidget class]) &&
	   !([parent class] == [RelativeLayoutWidget class]) &&
	   !([parent class] == [ListViewItemWidget class]) &&	   
	   !([parent class] == [ScreenWidget class]) &&
	   !([parent superclass] == [ScreenWidget class])
	   ) {
		return MAW_RES_INVALID_LAYOUT;
	}

	[NSObject performSelectorOnMainThread:@selector(addChild:)
							 withTarget:parent
							withObjects:[NSArray arrayWithObjects: child, nil] 
						  waitUntilDone:YES
						 andReturnValue:nil];	
	return MAW_RES_OK;
}

// TODO: implement this
int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index) {

	return MAW_RES_ERROR;
}

int maWidgetRemoveChild(MAWidgetHandle childHandle) {
	int returnValue;
	IWidget* child = [mosyncUI getWidget:childHandle];
	if(!child) return MAW_RES_INVALID_HANDLE;
	[NSObject performSelectorOnMainThread:@selector(remove)
							   withTarget:child
							  withObjects:[NSArray arrayWithObjects: nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	return returnValue;
}

static bool sNativeUIEnabled = false;
bool isNativeUIEnabled() {
	return sNativeUIEnabled;
}

int maWidgetScreenShow(MAWidgetHandle screenHandle) {
	if(screenHandle == MAW_CONSTANT_MOSYNC_SCREEN_HANDLE) {
		[NSObject performSelectorOnMainThread:@selector(showMoSyncCanvas)
								   withTarget:mosyncUI
								  withObjects:[NSArray arrayWithObjects: nil] 
								waitUntilDone:YES
							   andReturnValue:nil];	
		sNativeUIEnabled = false;
		return MAW_RES_OK;
	}
	
	IWidget* screen = [mosyncUI getWidget:screenHandle];
	if(!screen) return MAW_RES_INVALID_HANDLE;

	if(!([screen class] == [ScreenWidget class]) && !([screen superclass] == [ScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}
	
	sNativeUIEnabled = true;	
	
	int returnValue;
	[NSObject performSelectorOnMainThread:@selector(show:)
								withTarget:mosyncUI
							  withObjects:[NSArray arrayWithObjects: screen, nil] 
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
	return returnValue;
}

