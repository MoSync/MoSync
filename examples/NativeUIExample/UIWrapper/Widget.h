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
 * @author Mikael Kindborg
 *
 * Widget is the base class of all widgets. This class is
 * used to wrap native widget handles and provides common
 * methods for widgets and event support.
 */

#ifndef MOSYNC_UI_WIDGET_H_
#define MOSYNC_UI_WIDGET_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Environment.h>
#include <IX_WIDGET.h>
#include "conprint.h"

namespace MoSync
{
	namespace UI
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
		 * Destructor.
		 */
		virtual ~Widget();

		/**
		 * @return The handle of the widget.
		 */
		virtual MAWidgetHandle getWidgetHandle() const;

		/**
		 * Set a widget string property.
		 * @param property The property name.
		 * @param value The string value.
		 */
		virtual void setProperty(
			const MAUtil::String& property, const MAUtil::String& value);

		/**
		 * Set a widget integer property.
		 * @param property The property name.
		 * @param value The ingeter value.
		 */
		virtual void setProperty(
			const MAUtil::String& property, int value);

		/**
		 * Set a widget float property.
		 * @param property The property name.
		 * @param value The float value.
		 */
		virtual void setProperty(
			const MAUtil::String& property, float value);

		/**
		 * Get a widget property as an integer.
		 * @param property The property name.
		 * @return The integer value.
		 */
		virtual int getPropertyInt(
			const MAUtil::String& property) const;

		/**
		 * Get a widget property as a string.
		 * @param property The property name.
		 * @return The string value.
		 */
		virtual MAUtil::String getPropertyString(
			const MAUtil::String& property) const;

		/**
		 * Add a widget as a child of this widget.
		 * @param widget The widget to be added.
		 */
		virtual void addChild(Widget* widget);

		/**
		 * Set the left/top position of the widget.
		 * @param left The left coordinate.
		 * @param top The top coordinate.
		 */
		virtual void setPosition(int left, int top);

		/**
		 * Set the size of the widget.
		 * @param width The width value.
		 * @param height The height value.
		 */
		virtual void setSize(int width, int height);

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
		 * @param color a hexadecimal color value, e.g. 0xFF0000.
		 */
		virtual void setBackgroundColor(int color);

		/**
		 * Set the background color of the widget.
		 * @param red Red component (range 0-255).
		 * @param green Green component (range 0-255).
		 * @param blue Blue component (range 0-255).
		 */
		virtual void setBackgroundColor(int red, int green, int blue);

		/**
		 * Add an event listener for this widget.
		 * Note: At present there can only be one listener.
		 * The current listener will be replaced when a new
		 * one is set. Remove the listener by setting it
		 * to NULL.
		 * @param listener The listener that will receive
		 * widget events for this widget.
		 */
		virtual void setEventListener(WidgetEventListener* listener);

		/**
		 * This method is called when there is an event for this widget.
		 * It passes on the event to the widget's listener if one is set.
		 * Note: You can either use an event listener or override this
		 * method in a sublclass to handle events.
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
		 * Pointer to a widget listener (may be null).
		 */
		WidgetEventListener* mWidgetEventListener;
	};

	} // namespace UI
} // namespace MoSync

#endif
