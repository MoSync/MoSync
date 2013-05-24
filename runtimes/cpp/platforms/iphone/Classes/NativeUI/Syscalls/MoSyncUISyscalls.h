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

#ifndef _MOSYNC_UI_SYSCALLS_
#define _MOSYNC_UI_SYSCALLS_

/*
 * This is the syscall implementation for the native ui api.
 */

#import <Foundation/Foundation.h>
#import "MoSyncUI.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController);
bool isNativeUIEnabled();

MoSyncUI* getMoSyncUI();

/**
 * Creates a new widget of the specified type.
 * @param widgetType A String representing the type of the widget to create.
 * @return A handle to the widget, or any of the following result codes:
 * - MAW_RES_ERROR if the widget could not be created.
 * - MAW_RES_INVALID_TYPE_NAME if the widget type was not available.
 */
MAWidgetHandle maWidgetCreate(const char *widgetType);

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
int maWidgetDestroy(MAWidgetHandle handle);

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
int maWidgetAddChild(MAWidgetHandle parentHandle, MAHandle childHandle);

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
int maWidgetInsertChild(MAWidgetHandle parentHandle, MAWidgetHandle childHandle, int index);

/**
 * Removes a child widget from its parent (but does not destroy it).
 * Removing a currently visible top-level widget causes the MoSync view to become visible.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetRemoveChild(MAWidgetHandle child);

/**
 * Shows a dialog widget.
 * @param dialogHandle The handle of the dialog that will be shown.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetModalDialogShow(MAWidgetHandle dialogHandle);

/**
 * Hides/Dismisses a currently displayed dialog.
 * @param dialogHandle The handle of the dialog that will be hidden.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the child could be removed from the parent.
 * - MAW_RES_INVALID_HANDLE if the handle was invalid.
 */
int maWidgetModalDialogHide(MAWidgetHandle dialogHandle);

/**
 * Shows a screen. If native UI hasn't been initialized, it is also initialized
 * and disables regular MoSync drawing.
 * @param screenHandle The handle to the screen.
 * @return Any of the following result codes:
 * - MAW_RES_OK if the screen could be shown.
 * - MAW_RES_INVALID_HANDLE if the screenHandle is invalid.
 * - MAW_RES_INVALID_SCREEN if the screenHandle is not a handle to a screen.
 */
int maWidgetScreenShow(MAWidgetHandle screenHandle);

/**
 * Shows a screen with a transition. If native UI hasn't been initialized, it is also initialized
 * and disables regular MoSync drawing.
 *
 * @param screenHandle The handle to the screen.
 * @param screenTransitionType The transition of the screen.
 * @param screenTransitionDuration The duration of the transition.
 *
 * @returns Any of the following result codes:
 * - MAW_RES_OK if the screen could be shown.
 * - MAW_RES_INVALID_HANDLE if the screenHandle is invalid.
 * - MAW_RES_INVALID_SCREEN if the screenHandle is not a handle to a screen.
 * - MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not available
 * on the running platform.
 * - MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the value representing the
 * duration of the screen transition is invalid.
 */
int maWidgetScreenShowWithTransition(MAWidgetHandle screenHandle,
                                     MAWScreenTransitionType screenTransitionType,
                                     int screenTransitionDuration);

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
int maWidgetStackScreenPush(MAWidgetHandle stackScreen, MAWidgetHandle screen);

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
int maWidgetStackScreenPop(MAWidgetHandle stackScreen);

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
int maWidgetSetProperty(MAWidgetHandle handle, const char *property, const char* value);

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
int maWidgetGetProperty(MAWidgetHandle handle, const char *property, char *value, int maxSize);

/**
 * Functionality not available on iOS runtime.
 * Added to make the build system work.
 * @return IOCTL_UNAVAILABLE
 */
int maWidgetScreenAddOptionsMenuItem(MAWidgetHandle widget, const char * title, const char* iconPath, int iconPredefined);

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
//int maActionBarSetEnabled(int showActionBar);

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
//int maActionBarAddMenuItem(MAHandle screenHandle, const char* title, const MA_ACTION_BAR_ITEM_ICON* item, int displayFlag);

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
//int maActionBarRemoveMenuItem(MAHandle screenHandle,int itemHandle);

/**
 * Call this syscall after you finished adding/removing items to the action bar.
 * Recommended for performance, so that the action bar does not get redrawn each time one item is changed.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarRefresh();

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
//int maActionBarSetVisibility(int showState);

/**
 * Get the current height of the action bar.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarGetHeight();

/**
 * Check if the ActionBar is currently displayed.
 *
 * \returns 1 if the Action bar is visible, 0 if it was hidden, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarIsShowing();

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
//int maActionBarSetTitle(const char* title);

/**
 * Set the icon to display in the 'home' section of the action bar.
 * @param iconHandle Drawable to show as the ActionBar icon.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_INVALID_ICON If the iconHandle is invalid.
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarSetIcon(MAHandle iconHandle);

/**
 * Set whether an activity title/subtitle should be displayed.
 * @param enableTitle True to enable the display of the title, False otherwise.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarShowTitleEnabled(int enableTitle);

/**
 * Set whether to display the activity logo rather than the activity icon.
 * A logo is often a wider, more detailed image.
 * @param enableLogo True to enable logo, False otherwise.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarShowLogoEnabled(int enableLogo);

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
//int maActionBarSetHomeButtonEnabled(int state);

/**
 * Enable or disable the icon for up navigation (which displays the "up" indicator next to the
 * ActionBar icon.
 * @param True to show the user that selecting home will return one level up rather than to the top level of the app.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarSetDisplayHomeAsUpEnabled(int showHomeAsUp);

/**
 * Set the ActionBar's background. This will be used for the primary action bar.
 * @param imageHandle The drawable to use for the ActionBar's background.
 *
 * \returns #MAW_RES_OK on success, or any of the following result codes:
 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform.
 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar was explicitly disabled.
 */
//int maActionBarSetBackgroundImage(MAHandle imageHandle);

#endif // _MOSYNC_UI_SYSCALLS_
