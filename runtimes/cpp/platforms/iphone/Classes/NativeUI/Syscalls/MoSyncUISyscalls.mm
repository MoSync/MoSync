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
#import "ModalDialogWidget.h"
#import "RelativeLayoutWidget.h"
#import "ListViewItemWidget.h"
#import "ListViewWidget.h"
#import "GLViewWidget.h"
#import "OptionsDialogView.h"
#import "VerticalLayoutWidget.h"
#import "HorizontalLayoutWidget.h"

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

/**
 * Create an NSString object using a C array of UTF8-encoded bytes.
 * The caller must release the returned object.
 * @param str The C array. Must not be NULL.
 */
NSString* stringFromChar(const char* str) {
	return [[NSString alloc] initWithUTF8String:str];
}

/**
 * Creates a new widget of the specified type.
 * @param widgetType A String representing the type of the widget to create.
 * @return A handle to the widget, or any of the following result codes:
 * - MAW_RES_ERROR if the widget could not be created.
 * - MAW_RES_INVALID_TYPE_NAME if the widget type was not available.
 */
MAWidgetHandle maWidgetCreate(const char *widgetType) {
	int returnValue;
    if (widgetType == NULL)
        return MAW_RES_INVALID_TYPE_NAME;
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

/**
 * Frees the memory and resources held by the given widget. Destryoing a widget
 * with children will also cause its children to be destroyed. Once a handle has
 * been destroyed it cannot be referenced by the maWidget* functions.
 * If the given widget has a parent, the widget will be removed from its parent.
 * @param widget A handle to the widget to be destroyed.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the widget was destroyed.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetDestroy(MAWidgetHandle handle) {
	int returnValue;

    IWidget* widget = [mosyncUI getWidget:handle];
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

/**
 * Adds a widget to the given parent as a child. Letting the
 * parent widget layout the child.
 * @param parent The widget layout to which the child will be added.
 * @param child The widget that will be added to the parent.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be added to the parent.
 * - MAW_RES_INVALID_HANDLE if any of the handles were invalid.
 * - MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
 * - MAW_RES_CANNOT_INSERT_DIALOG if the child is a modal dialog.
 * - MAW_RES_ERROR if it could not be added for some other reason.
 */
int maWidgetAddChild(MAWidgetHandle parentHandle, MAHandle childHandle)
{
	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];

    // Check if the handles are valid.
	if (!parent || !child)
    {
        return MAW_RES_INVALID_HANDLE;
    }

    // Check if the widgets are the same or if the child already has a parent.
	if (parent == child || child.parent)
    {
        return MAW_RES_ERROR;
    }

    int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects: child, nil];
	[NSObject performSelectorOnMainThread:@selector(addChild:)
                               withTarget:parent
                              withObjects:arguments
                            waitUntilDone:YES
                           andReturnValue:&returnValue];
    [arguments release];
	return returnValue;
}

/**
 * Inserts a widget to the given parent as a child at an index. Letting the
 * parent widget layout the child.
 * @param parent The widget layout in which the child will be inserted.
 * @param child The widget that will be added to the parent.
 * @param index The index where the widget should be inserted.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be added to the parent.
 * - MAW_RES_INVALID_HANDLE if any of the handles were invalid.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 * - MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
 * - MAW_RES_CANNOT_INSERT_DIALOG if the child is a modal dialog.
 * - MAW_RES_ERROR if it could not be added for some other reason.
 */
int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index)
{
	IWidget* parent = [mosyncUI getWidget:parentHandle];
	IWidget* child = [mosyncUI getWidget:childHandle];

    // Check if the handles are valid.
	if (!parent || !child)
    {
        return MAW_RES_INVALID_HANDLE;
    }

    // Check if the widgets are the same or if the child already has a parent.
	if (parent == child || child.parent)
    {
        return MAW_RES_ERROR;
    }

	int returnValue;
    NSNumber* indexNumber = [[NSNumber alloc] initWithInt:index];
    NSArray* arguments = [[NSArray alloc] initWithObjects: child, indexNumber, nil];
	[NSObject performSelectorOnMainThread:@selector(insertChild:atIndex:)
							   withTarget:parent
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [indexNumber release];
	[arguments release];
	return returnValue;
}

/* dummy function for the build to work*/
int maWidgetScreenAddOptionsMenuItem(MAWidgetHandle widget, const char * title,
                                     const char* iconHandle, int iconPredefined)
{
    return IOCTL_UNAVAILABLE;
}

int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value) {
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

    if(property == NULL)
        return MAW_RES_INVALID_PROPERTY_NAME;

    if(value == NULL)
        return MAW_RES_INVALID_PROPERTY_VALUE;

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

    if(property == NULL)
        return MAW_RES_INVALID_PROPERTY_NAME;

    if(value == NULL)
        return MAW_RES_INVALID_PROPERTY_VALUE;


	NSString* propertyString = stringFromChar(property);
	NSString* retval;
    NSArray* arguments = [[NSArray alloc] initWithObjects: propertyString, nil];

	[NSObject performSelectorOnMainThread:@selector(getPropertyWithKey:)
							   withTarget:widget
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&retval];

	[arguments release];

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

int maWidgetModalDialogShow(MAWidgetHandle dialogHandle) {
	IWidget* dialog = [mosyncUI getWidget:dialogHandle];
	if(!dialog) return MAW_RES_INVALID_HANDLE;

	if(!([dialog class] == [ModalDialogWidget class])) {
		return MAW_RES_INVALID_HANDLE;
	}

	int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects:nil];
	[NSObject performSelectorOnMainThread:@selector(show)
							   withTarget:dialog
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [arguments release];
	return returnValue;
}

int maWidgetModalDialogHide(MAWidgetHandle dialogHandle) {
	IWidget* dialog = [mosyncUI getWidget:dialogHandle];
	if(!dialog) return MAW_RES_INVALID_HANDLE;

	if(!([dialog class] == [ModalDialogWidget class])) {
		return MAW_RES_INVALID_HANDLE;
	}

	int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects:nil];
	[NSObject performSelectorOnMainThread:@selector(hide)
							   withTarget:dialog
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [arguments release];
	return returnValue;
}
