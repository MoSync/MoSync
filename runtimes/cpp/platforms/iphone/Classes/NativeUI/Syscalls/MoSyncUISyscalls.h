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
 * - MAW_RES_INVALID_SCREEN if the screenHandle is not a handle to a screen.
 */
int maWidgetScreenShow(MAWidgetHandle screenHandle);

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

#endif // _MOSYNC_UI_SYSCALLS_
