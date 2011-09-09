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
#import "StackScreenWidget.h"
#import "LayoutWidgets.h"
#import "RelativeLayoutWidget.h"
#import "ListViewItemWidget.h"
#import "ListViewWidget.h"
#import "GLViewWidget.h"

MoSyncUI* mosyncUI;

MoSyncUI* getMoSyncUI() {
	return mosyncUI;
}

static bool sNativeUIEnabled = false;
bool isNativeUIEnabled() {
	return sNativeUIEnabled;
}

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController) {
	mosyncUI = [[MoSyncUI alloc] initWithWindow:window andController:viewController];
}

NSString* stringFromChar(const char* str) {
	return [[NSString alloc] initWithUTF8String:str];    
}

MAWidgetHandle maWidgetCreate(const char *widgetType) {	
//	NSLog(@"maWidgetCreate(%s)\n", widgetType);
	
	int returnValue;
	NSString* widgetTypeString = stringFromChar(widgetType);
    NSArray *arguments = [[NSArray alloc] initWithObjects: widgetTypeString, nil];
	[NSObject performSelectorOnMainThread:@selector(createWidget:)
							   withTarget:mosyncUI
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [arguments release];
    [widgetTypeString release];

	return returnValue;
}

int maWidgetDestroy(MAWidgetHandle handle) {
	int returnValue;

    IWidget* widget = [mosyncUI getWidget:handle];
	NSLog(@"retainCount after getWidget: %d", [widget retainCount]);
    if(widget == NULL) return MAW_RES_INVALID_HANDLE;
	
	bool isCurrentlyShownScreen = widget==[mosyncUI getCurrentlyShownScreen];
	
    NSArray* arguments = [[NSArray alloc] initWithObjects: widget, nil];
	[NSObject performSelectorOnMainThread:@selector(destroyWidgetInstance:)
							   withTarget:mosyncUI
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];

    [arguments release];

	if(isCurrentlyShownScreen) {
		maWidgetScreenShow(MAW_CONSTANT_MOSYNC_SCREEN_HANDLE);
	}

	return returnValue;
}

int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value) {
//	NSLog(@"maWidgetSetProperty(%d, %s, %s)\n", handle, property, value);

	IWidget* widget = [mosyncUI getWidget:handle];
	if(widget == NULL) return MAW_RES_INVALID_HANDLE;	
	
	NSString* propertyString = stringFromChar(property);
	
	if([widget class] == [GLViewWidget class] || [widget class] == [GL2ViewWidget class] ) {
		// do this from the MoSync thread. Maybe do a generic system for this later.
		if([propertyString isEqualToString:@"bind"]) {
			return [widget setPropertyWithKey:@"bind" toValue:@""];
		}
		
		if([propertyString isEqualToString:@"invalidate"]) {
			return [widget setPropertyWithKey:@"invalidate" toValue:@""];
		}	
	}
	
	int returnValue;
	
	NSString *valueString = stringFromChar(value);
    NSArray* arguments = [[NSArray alloc] initWithObjects: propertyString, valueString, nil];
	[NSObject performSelectorOnMainThread:@selector(setPropertyWithKey:toValue:)
							   withTarget:widget
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	
    [arguments release];
    [valueString release];
	[propertyString release];
    
	return returnValue;
}

// if maWidgetGetProperty returns <0 maxSize equals the size needed.
int maWidgetGetProperty(MAWidgetHandle handle, const char *property, char *value, int maxSize) {
	IWidget* widget = [mosyncUI getWidget:handle];
	if(!widget) return MAW_RES_INVALID_HANDLE;		
	
	NSString* propertyString = stringFromChar(property);
	NSString* retval;
    NSArray* arguments = [[NSArray alloc] initWithObjects: propertyString, nil];    
	[NSObject performSelectorOnMainThread:@selector(getPropertyWithKey:)
							   withTarget:widget
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&retval];
	
	//[arguments release];
    
	if(retval == nil) return MAW_RES_ERROR;
	int length = maxSize;
	int realLength = [retval length];
	if(realLength > length) {
		return MAW_RES_INVALID_STRING_BUFFER_SIZE;
	}
	
	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	[retval release];
    [propertyString release];
	
	return realLength;
}

int maWidgetPerformAction(MAWidgetHandle widget, const char *action, const char *param){
	int returnValue=1;
	
	return returnValue; 
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
	   !([parent class] == [ListViewWidget class]) &&
	   !([parent class] == [ListViewItemWidget class]) &&		   
	   !([parent class] == [ScreenWidget class]) &&
	   !([parent superclass] == [ScreenWidget class])
	   ) {
		return MAW_RES_INVALID_LAYOUT;
	}

    NSArray* arguments = [[NSArray alloc] initWithObjects: child, nil];
	[NSObject performSelectorOnMainThread:@selector(addChild:)
							 withTarget:parent
							withObjects:arguments
						  waitUntilDone:YES
						 andReturnValue:nil];
    [arguments release];
    
	return MAW_RES_OK;
}

int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index) {
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
	   !([parent class] == [ListViewWidget class]) &&	   
	   !([parent class] == [ListViewItemWidget class]) &&	   
	   !([parent class] == [ScreenWidget class]) &&
	   !([parent superclass] == [ScreenWidget class])
	   ) {
		return MAW_RES_INVALID_LAYOUT;
	}

	
	int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects: child, [[NSNumber alloc] initWithInt:index], nil];
	[NSObject performSelectorOnMainThread:@selector(insertChild:atIndex:)
							   withTarget:parent
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];	
	[arguments release];
	return returnValue;
}

int maWidgetRemoveChild(MAWidgetHandle childHandle) {
	int returnValue;
	IWidget* child = [mosyncUI getWidget:childHandle];
	if(!child) return MAW_RES_INVALID_HANDLE;
    NSArray* arguments = [[NSArray alloc] initWithObjects:  nil];
	[NSObject performSelectorOnMainThread:@selector(remove)
							   withTarget:child
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
	[arguments release];
	return returnValue;
}

int maWidgetStackScreenPush(MAWidgetHandle stackScreenHandle, MAWidgetHandle screenHandle) {
	IWidget* stackScreen = [mosyncUI getWidget:stackScreenHandle];
	if(!stackScreen) return MAW_RES_INVALID_HANDLE;

	IWidget* screen = [mosyncUI getWidget:screenHandle];
	if(!screen) return MAW_RES_INVALID_HANDLE;	
	
	if(!([screen class] == [ScreenWidget class]) && !([screen superclass] == [ScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}
	
	if(!([stackScreen class] == [StackScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}
	
    NSArray* arguments = [[NSArray alloc] initWithObjects:screen, nil];
	[NSObject performSelectorOnMainThread:@selector(push:)
							   withTarget:stackScreen
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:nil];
    [arguments release];
	return MAW_RES_OK;
}

int maWidgetStackScreenPop(MAWidgetHandle stackScreenHandle) {
	IWidget* stackScreen = [mosyncUI getWidget:stackScreenHandle];
	if(!stackScreen) return MAW_RES_INVALID_HANDLE;
	
	if(!([stackScreen class] == [StackScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}
	
    NSArray* arguments = [[NSArray alloc] initWithObjects:nil]; 
	[NSObject performSelectorOnMainThread:@selector(pop)
							   withTarget:stackScreen
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:nil];
    [arguments release];    
	return MAW_RES_OK;
}

int maWidgetScreenShow(MAWidgetHandle screenHandle) {
	IWidget* screen = [mosyncUI getWidget:screenHandle];
	if(!screen) return MAW_RES_INVALID_HANDLE;

	if(!([screen class] == [ScreenWidget class]) && !([screen superclass] == [ScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}
	
	sNativeUIEnabled = screenHandle==MAW_CONSTANT_MOSYNC_SCREEN_HANDLE?false:true;	
	
	int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects:screen, nil];    
	[NSObject performSelectorOnMainThread:@selector(show:)
								withTarget:mosyncUI
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [arguments release];
	return returnValue;
}

