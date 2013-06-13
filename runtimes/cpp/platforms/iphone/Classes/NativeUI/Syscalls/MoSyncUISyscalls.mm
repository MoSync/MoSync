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
#import "NavBarButtonWidget.h"

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

MAWidgetHandle maWidgetCreateWithInt(const char *widgetType, int arg) {
    if (widgetType == NULL)
        return MAW_RES_INVALID_TYPE_NAME;

    NSString* widgetTypeString = stringFromChar(widgetType);
    NSNumber* number = [[NSNumber alloc] initWithInt:arg];

	__block int returnValue;
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    if (mainQueue == dispatch_get_current_queue())
    {
        returnValue = [mosyncUI createWidget:widgetTypeString initParam:number];
    }
    else
    {
        dispatch_sync(mainQueue, ^
            {
                returnValue = [mosyncUI createWidget:widgetTypeString initParam:number];
            });
    }
    [number release];
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

/**
 * Removes a child widget from its parent (but does not destroy it).
 * Removing a currently visible top-level widget causes the MoSync view to become visible.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetRemoveChild(MAWidgetHandle childHandle)
{
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

/**
 * Shows a dialog widget.
 * @param dialogHandle The handle of the dialog that will be shown.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetModalDialogShow(MAWidgetHandle dialogHandle)
{
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

/**
 * Hides/Dismisses a currently displayed dialog.
 * @param dialogHandle The handle of the dialog that will be hidden.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
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

/**
 * Shows a screen. If native UI hasn't been initialized, it is also initialized
 * and disables regular MoSync drawing.
 * @param screenHandle The handle to the screen.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the screen could be shown.
 * - MAW_RES_INVALID_HANDLE if the screenHandle is invalid.
 * - MAW_RES_INVALID_SCREEN if the screenHandle is not a handle to a screen.
 */
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

int maWidgetScreenShowWithTransition(MAWidgetHandle screenHandle, MAWScreenTransitionType screenTransitionType, int screenTransitionDuration) {

	IWidget* screen = [mosyncUI getWidget:screenHandle];
	if(!screen) return MAW_RES_INVALID_HANDLE;

	if(!([screen class] == [ScreenWidget class]) && !([screen superclass] == [ScreenWidget class])) {
		return MAW_RES_INVALID_SCREEN;
	}

	sNativeUIEnabled = screenHandle==MAW_CONSTANT_MOSYNC_SCREEN_HANDLE?false:true;

    NSNumber* scrTransType = [[NSNumber alloc] initWithInt:screenTransitionType];
    NSNumber* scrTransDur = [[NSNumber alloc] initWithInt:screenTransitionDuration];

	int returnValue;
    NSArray* arguments = [[NSArray alloc] initWithObjects:screen, scrTransType, scrTransDur, nil];
	[NSObject performSelectorOnMainThread:@selector(show:withTransitionType:andTransitionDuration:)
                               withTarget:mosyncUI
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&returnValue];
    [arguments release];
    [scrTransDur release];
    [scrTransType release];

	return returnValue;
}

/**
 * Pushes a screen to the given screen stack, hides the current screen and
 * shows the pushed screen it. Pushing it to the stack will make it
 * automatically go back to the previous screen when popped.
 * @param stackScreen A handle to a MAW_STACK_SCREEN.
 * @param newScreen A handle to either a MAW_SCREEN or a MAW_TAB_SCREEN.
 * The handle cannot exist in the stack already.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the given screen could be pushed.
 * - MAW_RES_INVALID_HANDLE if either the screenStack or newScreen handle was invalid.
 * - MAW_RES_INVALID_SCREEN if the screen handle is not a handle to a screen.
 * - MAW_RES_ERROR if something else than a screen stack was passed, or if
 * the handle already exists in the stack.
 */
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

/**
 * Pops a screen from a screen stack, hides the current screen and shows
 * the popped screen before the If there is no previous screen in the screen
 * stack, an empty screen will be shown.
 * @param stackScreen A handle to a MAW_STACK_SCREEN to pop from.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the given screen could be popped.
 * - MAW_RES_INVALID_HANDLE if the screen stack does not exist.
 * - MAW_RES_ERROR if something else than a screen stack was passed.
 */
int maWidgetStackScreenPop(MAWidgetHandle stackScreenHandle) {
	IWidget* stackScreen = [mosyncUI getWidget:stackScreenHandle];
	if(!stackScreen) return MAW_RES_INVALID_HANDLE;

	if(!([stackScreen class] == [StackScreenWidget class])) {
		return MAW_RES_ERROR;
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

/**
 * Sets a specified property on the given widget.
 * @param widget Handle to the widget.
 * @param property A string representing which property to set.
 * @param value The value which will be assigned to the property.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the property could be set.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - MAW_RES_ERROR otherwise.
 */
int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value) {
	IWidget* widget = [mosyncUI getWidget:handle];
	if(widget == NULL) return MAW_RES_INVALID_HANDLE;

	if(property == NULL)
		return MAW_RES_INVALID_PROPERTY_NAME;

	if(value == NULL)
		return MAW_RES_INVALID_PROPERTY_VALUE;

	NSString* propertyString = stringFromChar(property);

	if([widget class] == [GLViewWidget class] || [widget class] == [GL2ViewWidget class] ) {
		// do this from the MoSync thread. Maybe do a generic system for this later.
		if([propertyString isEqualToString:@"bind"])
		{
			[propertyString release];
			return [widget setPropertyWithKey:@"bind" toValue:@""];
		}

		if([propertyString isEqualToString:@"invalidate"])
		{
			[propertyString release];
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

/**
 * Retrieves a specified property from the given widget.
 * @param widget Handle to the widget.
 * @param property A string representing which property to set.
 * @param value A buffer that will hold the value of the property, represented as a string.
 * @param bufSize Size of the buffer.
 * @return The number of bytes copied on success, or any of the following result codes:
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
 * - MAW_RES_ERROR otherwise.
 */
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

	[retval getCString:value maxLength:length encoding:NSUTF8StringEncoding];
	[retval release];
    [propertyString release];

	return realLength;
}

/**
 * Functionality not available on iOS runtime.
 * Added to make the build system work.
 * @return IOCTL_UNAVAILABLE
 */
int maWidgetScreenAddOptionsMenuItem(MAWidgetHandle widget, const char * title,
                                     const char* iconHandle, int iconPredefined)
{
    return IOCTL_UNAVAILABLE;
}

int maWidgetScreenInsertNavBarButton(int side, MAWidgetHandle screenWidgetHandle, MAWidgetHandle navBarButtonWidgetHandle, int index)
{
    IWidget* widget = [mosyncUI getWidget:screenWidgetHandle];
    if(!widget) return MAW_RES_INVALID_HANDLE;
    if ([widget class] != [ScreenWidget class] && [widget class] != [StackScreenWidget class]) return MAW_RES_ERROR;
    ScreenWidget* screen = (ScreenWidget*)widget;

    widget = [mosyncUI getWidget:navBarButtonWidgetHandle];
    if(!widget) return MAW_RES_INVALID_HANDLE;
    if ([widget class] != [NavBarButtonWidget class]) return MAW_RES_ERROR;
    NavBarButtonWidget* button = (NavBarButtonWidget*)widget;

    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    if (mainQueue == dispatch_get_current_queue())
    {
        [screen attachNavBarButton:side navBarButtonWidget:button atIndex:index];
    }
    else
    {
        dispatch_async(mainQueue, ^
            {
                [screen attachNavBarButton:side navBarButtonWidget:button atIndex:index];
            });
    }
    return MAW_RES_OK;
}

/**
 * Enable or disable the Action bar.
 * By default, the Action bar is disabled.
 * Note that the Action bar is only available starting from Android API level 11.
 * @param showActionBar True to enable it, False to disable it.
 *
 * \returns
 * - #MAW_RES_OK on success, or
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 */
/*int maActionBarSetEnabled(int showActionBar)
{
    return IOCTL_UNAVAILABLE;
}*/


/**
 * Add a menu item to the Action bar.
 * @param screenHandle Handle to the screen widget.
 * @param MA_ACTION_BAR_ITEM_ICON Arguments for the new menu item.
 * @param title The title associated for the new item. Can be left null.
 * @param iconPredefinedId Optional icon for the action bar item. Set it to -1 if the item has no icon.
 * @param displayFlag One of the \link #MAW_ACTION_BAR_ITEM_SHOW_  \endlink constants.
 *
 * \returns The the item handle on success, or any of the following result codes:
 * - #MAW_RES_ERROR
 * - #MAW_RES_ACTION_BAR_INVALID_ICON
 * - #MAW_RES_ACTION_BAR_INVALID_FLAG
 * - #MAW_RES_INVALID_HANDLE If the screenHandle points to an invalid screen.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 */
/*int maActionBarAddMenuItem(MAHandle screenHandle, const char* title, const MA_ACTION_BAR_ITEM_ICON* item, int displayFlag)
{
    return IOCTL_UNAVAILABLE;
}*/

/**
 * Remove a menu item from the Action bar.
 * @param screenHandle Handle to the screen widget.
 * @param itemHandle Handle to the menu item.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform,
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled, or
 * - #MAW_RES_INVALID_HANDLE If the indicated screen has no item on specified handle.
 */
// int maActionBarRemoveMenuItem(MAHandle screenHandle,int itemHandle)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Call this syscall after you finished adding/removing items to the action bar.
 * Recommended for performance, so that the action bar does not get redrawn each time one item is changed.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarRefresh()
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Enable or disable the display of the action bar.
 * Use this when you need the application to get into fullscreen, like for instance in a help screen.
 * Default value is True if the action bar is enabled on the application.
 * @param showState True to show the action bar, False to hide it.
 * If you are hiding the ActionBar through #maScreenSetFullscreen(), you should not call this function directly.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetVisibility(int showState)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Get the current height of the action bar.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarGetHeight()
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Check if the ActionBar is currently displayed.
 *
 * \returns 1 if the Action bar is visible, 0 if it was hidden, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarIsShowing()
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Set the action bar's title.
 * This will only be displayed if #maActionBarShowTitleEnabled(true) is set.
 * By default, the title is the activity title, in this case application's name.
 * @param title The ActionBar title to set.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetTitle(const char* title)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Set the icon to display in the 'home' section of the action bar.
 * @param iconHandle Drawable to show as the ActionBar icon.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_INVALID_ICON If the iconHandle is invalid.
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetIcon(MAHandle iconHandle)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Set whether an activity title/subtitle should be displayed.
 * @param enableTitle True to enable the display of the title, False otherwise.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarShowTitleEnabled(int enableTitle)
// {
//     return IOCTL_UNAVAILABLE;
// }

/*
 * Set whether to display the activity logo rather than the activity icon.
 * A logo is often a wider, more detailed image.
 * @param enableLogo True to enable logo, False otherwise.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
*/
// int maActionBarShowLogoEnabled(int enableLogo)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Enable or disable the "home" button in the corner of the action bar.
 * This defaults to true for packages targeting < API 14. For packages targeting API 14 or greater,
 * the application should call this method to enable interaction with the home/up affordance.
 * If you're using the ActionBar icon to navigate to the home activity, beware that beginning with Android API level 14,
 * you must explicitly enable the icon as an action item by calling #maActionBarSetHomeButtonEnabled(true)
 * (in previous versions, the ActionBar icon was enabled as an action item by default).
 * @param state True to enable the home button, False to disable the home button.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetHomeButtonEnabled(int state)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Enable or disable the icon for up navigation (which displays the "up" indicator next to the
 * ActionBar icon.
 * @param True to show the user that selecting home will return one level up rather than to the top level of the app.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetDisplayHomeAsUpEnabled(int showHomeAsUp)
// {
//     return IOCTL_UNAVAILABLE;
// }

/**
 * Set the ActionBar's background. This will be used for the primary action bar.
 * @param imageHandle The drawable to use for the ActionBar's background.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
// int maActionBarSetBackgroundImage(MAHandle imageHandle)
// {
//     return IOCTL_UNAVAILABLE;
// }
