/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file Widget.h
 * @author Mikael Kindborg, Emma Tresanszki and Bogdan Iusco.
 *
 * Widget is the base class of all widgets. This class is
 * used to wrap native widget handles and provides common
 * methods for widgets and event support.
 */

#ifndef NATIVEUI_WIDGET_H_
#define NATIVEUI_WIDGET_H_

#include <ma.h>
#include <conprint.h>

#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Environment.h>

#include <IX_WIDGET.h>
#include "WidgetUtil.h"

namespace NativeUI
{

    // Forward declaration.
    class WidgetManager;
    class WidgetEventListener;

    /**
     * Base class for all widgets.
     */
    class Widget
    {
    public:
        /**
         * Destructor. Will delete all child widgets and
         * also handle the destruction of widget handles.
         */
        virtual ~Widget();

        /**
         * Set my widget handle and my chilren's widget
         * handles to NULL to mark that it has been deleted.
         */
        virtual void markWidgetHandleAsDeleted();

        /**
         * @return The handle of the widget.
         */
        virtual MAWidgetHandle getWidgetHandle() const;

        /**
         * Set a widget string property.
         * @param property A string representing which property to set.
         * @param value The string value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setProperty(
            const MAUtil::String& property,
            const MAUtil::String& value);

        /**
         * Set a widget integer property.
         * @param property A string representing which property to set.
         * @param value The integer value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setPropertyInt(
            const MAUtil::String& property,
            const int value);

        /**
         * Set a widget float property.
         * @param property A string representing which property to set.
         * @param value The float value which will be assigned to the property.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setPropertyFloat(
            const MAUtil::String& property,
            float value);

        /**
         * Get a widget property value as an integer, setting also the result code.
         * @param property A string representing which property to set.
         * @param resultCode Will contain the result code of the syscall.
         *                   Can be any of the following result codes:
         *                   - #MAW_RES_OK no error occurred.
         *                   - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         *                   - #MAW_RES_INVALID_PROPERTY_NAME if the property
         *                     name was invalid.
         *                   - #MAW_RES_INVALID_PROPERTY_VALUE if the property
         *                     value was invalid.
         *                   - #MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer
         *                     size was too small.
         *                   - #MAW_RES_ERROR otherwise.
         * @return The property value.
         */
        virtual int getPropertyInt(
            const MAUtil::String& property,
            int& resultCode) const;

        /**
         * Get a widget property value as an integer.
         * @param property A string representing which property to set.
         * @return The property value.
         */
        virtual int getPropertyInt(
            const MAUtil::String& property) const;

        /**
         * Get a widget property as a string, setting also the result code.
         * @param property A string representing which property to set.
         * @param resultCode Will contain the result code of the syscall.
         *                   Can be any of the following result codes:
         *                   - #MAW_RES_OK no error occurred.
         *                   - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         *                   - #MAW_RES_INVALID_PROPERTY_NAME if the property
         *                     name was invalid.
         *                   - #MAW_RES_INVALID_PROPERTY_VALUE if the property
         *                     value was invalid.
         *                   - #MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer
         *                     size was too small.
         *                   - #MAW_RES_ERROR otherwise.
         * @return The property value.
         */
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property,
            int& resultCode) const;

        /**
         * Get a widget property as a string.
         * @param property A string representing which property to set.
         * @return The property value.
         */
        virtual MAUtil::String getPropertyString(
            const MAUtil::String& property) const;

        /**
         * Add a widget as a child of this widget.
         * @param widget The widget that will be added.
         * The ownership of the widget is passed to this function.
         * When the parent widget will be destroyed, all child widgets
         * will be deleted.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the child could be added to the parent.
         * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
         * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
         * - #MAW_RES_ERROR if it could not be added for some other reason.
         */
        virtual int addChild(Widget* widget);

        /**
         * Insert a widget as a child of this widget, at a given position.
         * @param widget The widget to be added.
         * The ownership of the widget is passed to this function.
         * When the parent widget will be destroyed, all child widgets
         * will be deleted.
         * @param index The position on which to add the child.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the child could be added to the parent.
         * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
         * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
         * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
         * - #MAW_RES_ERROR if it could not be added for some other reason.
         */
        virtual int insertChild(Widget* widget,const int index);

        /**
         * Remove a child widget from its parent (but does not destroy it).
         * Removing a currently visible top-level widget causes the MoSync view
         * to become visible.
         * When the parent widget will be destroyed, the child widget will not
         * be deleted.
         * @param widget The widget to be removed.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the child could be removed from the parent.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int removeChild(Widget* widget);

        /**
         * Get a child widget of this widget.
         * @param index The index of the child.
         * @return The child widget at the given index, or null if the
         * index is invalid.
         * The ownership of the result is not passed to the caller.
         */
        virtual Widget* getChild(const int index) const;

        /**
         * Get the number of child widgets.
         * @return The number of children widgets.
         */
        virtual int countChildWidgets() const;

        /**
         * Set the left/top position of the widget.
         * @param left The left coordinate.
         * @param top The top coordinate.
         */
        virtual void setPosition(const int left, const int top);

        /**
         * Set the left position of the widget.
         * @param left The left coordinate.
         */
        virtual void setLeftPosition(const int left);

        /**
         * Set the top position of the widget.
         * @param top The top coordinate.
         */
        virtual void setTopPosition(const int top);

        /**
         * Set the size of the widget.
         * @param width The width value.
         * @param height The height value.
         */
        virtual void setSize(const int width, const int height);

        /**
         * Set the width of the widget.
         * @param width The width value.
         */
        virtual void setWidth(const int width);

        /**
         * Set the height of the widget.
         *  @param height The height value.
         */
        virtual void setHeight(const int height);

        /**
         * Size the width of this widget to fill the
         * available space in the parent widget horizontally.
         */
        virtual void fillSpaceHorizontally();

        /**
         * Size the height of this widget to fill the
         * available space in the parent widget vertically.
         */
        virtual void fillSpaceVertically();

        /**
         * Size the width of this widget to wrap the
         * content horizontally.
         */
        virtual void wrapContentHorizontally();

        /**
         * Size the height of this widget to wrap the
         * content vertically.
         */
        virtual void wrapContentVertically();

        /**
         * Set the background color of the widget.
         * @param color A hexadecimal color value, e.g. 0xFF0000.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setBackgroundColor(const int color);

        /**
         * Set the background color of the widget.
         * @param red Red component (range 0-255).
         * @param green Green component (range 0-255).
         * @param blue Blue component (range 0-255).
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setBackgroundColor(
            const int red, const int green, const int blue);

        /**
         * Set the background gradient of the widget.
         * This is available only for Android.
         * @param firstColor First color in hexadecimal, e.g. 0xFF0000.
         * @param secondColor Second color in hexadecimal, e.g. 0x0000FF.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        virtual int setBackgroundGradient(
            const int firstColor,
            const int secondColor);

        /**
         * Show or hide the widget.
         * @param visible If true shows the widget, otherwise hides it.
         */
        virtual void setVisible(const bool visible);

        /**
         * Check if the widget is visible.
         * @return True if is visible, false otherwise.
         */
        virtual bool isVisible() const;

        /**
         * Enable the widget.
         * @param enable If true enables the widget, otherwise disables it.
         */
        virtual void setEnabled(bool enable);

        /**
         * Check if the widget is enabled.
         * @return True if enabled, false otherwise.
         */
        virtual bool isEnabled();

        /**
         * Add an event listener for this widget.
         * @param listener The listener that will receive
         * widget events for this widget.
         */
        virtual void addEventListener(WidgetEventListener* listener);

        /**
         * Remove the event listener for this widget.
         * @param listener The listener that receives widget events
         * for this widget.
         */
        virtual void removeEventListener(WidgetEventListener* listener);

        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * Note: You can either use an event listener or override this
         * method in a subclass to handle events.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    protected:
        /**
         * Constructor is protected because actual widget instances
         * should be subclasses of this class.
         * @widgetType The string constant that identifies the widget type
         * (one of the MAW_ constants).
         */
        Widget(const MAUtil::String& widgetType);

    private:
        /**
         * Handle that identifies the widget.
         */
        MAWidgetHandle mWidgetHandle;

        /**
         * Pointer to a widget manager.
         */
        WidgetManager* mWidgetManager;

        /**
         * Array with widget listeners.
         */
        MAUtil::Vector<WidgetEventListener*> mWidgetEventListeners;

        /**
         * List of child widgets.
         */
        MAUtil::Vector<Widget*> mChildren;
    };

} // namespace NativeUI

#endif
