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
 * @file Widget.cpp
 * @author Mikael Kindborg
 *
 * Widget is the base class of all widgets. This class is
 * used to wrap native widget handles and provides common
 * methods for widgets and event support.
 *
 * Note: Not all methods in this class apply to all widget types.
 * The motivation for this design is that it is convenient to
 * have the most commonly used methods in the widget base class.
 * The drawback is that some of the methods will not do anything
 * when called on a widget that does not have the property used
 * by the method.
 */

#include <mavsprintf.h>
#include <mastdlib.h>
#include "Widget.h"
#include "WidgetManager.h"
#include "WidgetEventListener.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor is protected because actual widget instances
	 * should be subclasses of this class.
	 * @widgetType The string constant that identifies the widget type
	 * (one of the MAW_ constants).
	 */
	Widget::Widget(const MAUtil::String& widgetType) :
		mWidgetManager(WidgetManager::getInstance()),
		mWidgetEventListener(NULL)
	{
		mWidgetHandle = maWidgetCreate(widgetType.c_str());
		mWidgetManager->registerWidget(mWidgetHandle, this);
	}

	/**
	 * Destructor.
	 */
	Widget::~Widget()
	{
		// If my widget handle is NOT NULL, I set it
		// to NULL in me and all my children, because
		// once the widget handle is destroyed it will
		// destroy all child widgets on the native side.
		MAWidgetHandle widgetHandle = mWidgetHandle;
		if (NULL != widgetHandle)
		{
			markWidgetHandleAsDeleted();
		}

		// Now I recursively delete all my children. They
		// will not destroy their widget handles, because
		// they are set to NULL in the above step.
		for (int i = 0; i < mChildren.size(); ++i)
		{
			delete mChildren[i];
		}

		// If I had a widget handle, it is now destroyed, which
		// also destroys all child widgets on the native side.
		if (NULL != widgetHandle)
		{
			maWidgetDestroy(widgetHandle);
		}
	}

	/**
	 * Set my widget handle and my chilren's widget
	 * handles to NULL to mark that it has been deleted.
	 */
	void Widget::markWidgetHandleAsDeleted()
	{
		// Set my widget handle to NULL.
		mWidgetHandle = NULL;

		// Recursively do the same for my children.
		for (int i = 0; i < mChildren.size(); ++i)
		{
			mChildren[i]->markWidgetHandleAsDeleted();
		}
	}

	/**
	 * Destroy the widget handle associated with this widget
	 * and all child widgets. Will also deallocate all child
	 * widget objects.
	 */
	/*void Widget::destroy()
	{
		// Destroy the widget.
		maWidgetDestroy(mWidgetHandle);
	}*/

	/**
	 * @return The handle of the widget.
	 */
	MAWidgetHandle Widget::getWidgetHandle() const
	{
		return mWidgetHandle;
	}

	/**
	 * Set a widget string property.
	 * @param property The property name.
	 * @param value The string value.
	 */
	void Widget::setProperty(
		const MAUtil::String& property,
		const MAUtil::String& value)
	{
		maWidgetSetProperty(mWidgetHandle, property.c_str(), value.c_str());
	}

	/**
	 * Set a widget integer property.
	 * @param property The property name.
	 * @param value The ingeter value.
	 */
	void Widget::setProperty(
		const MAUtil::String& property,
		int value)
	{
		char buffer[256];
		sprintf(buffer, "%d", value);
		maWidgetSetProperty(mWidgetHandle, property.c_str(), buffer);
	}

	/**
	 * Set a widget float property.
	 * @param property The property name.
	 * @param value The float value.
	 */
	void Widget::setProperty(
		const MAUtil::String& property,
		float value)
	{
		char buffer[256];
		sprintf(buffer, "%f", value);
		maWidgetSetProperty(mWidgetHandle, property.c_str(), buffer);
	}

	/**
	 * Get a widget property as an integer.
	 * @param property The property name.
	 * @return The integer value.
	 */
	int Widget::getPropertyInt(
		const MAUtil::String& property) const
	{
		char buffer[256];
		maWidgetGetProperty(mWidgetHandle, property.c_str(), buffer, 256);
		return atoi(buffer);
	}

	/**
	 * Get a widget property as a string.
	 * @param property The property name.
	 * @return The string value.
	 */
	MAUtil::String Widget::getPropertyString(
		const MAUtil::String& property) const
	{
//		char buffer[16384];
//		maWidgetGetProperty(mWidgetHandle, property.c_str(), buffer, 16384);
//		return buffer;

		int BUFFER_SIZE = 16384;
		char *buffer = new char[BUFFER_SIZE];

		maWidgetGetProperty(mWidgetHandle, property.c_str(), buffer, BUFFER_SIZE);

		MAUtil::String temp;
		temp.append(buffer, BUFFER_SIZE);
		delete []buffer;

		return temp;
	}

	/**
	 * Add a widget as a child of this widget.
	 * @param widget The widget to be added.
	 */
	void Widget::addChild(Widget* widget)
	{
		// Add the widget object to the list of children.
		mChildren.add(widget);

		// Add the widget handle as a child, this will add
		// the native widget as a child.
		maWidgetAddChild(mWidgetHandle, widget->getWidgetHandle());
	}

	/**
	 * Set the left/top position of the widget.
	 * @param left The left coordinate.
	 * @param top The top coordinate.
	 */
	void Widget::setPosition(int left, int top)
	{
		setProperty(MAW_WIDGET_LEFT, left);
		setProperty(MAW_WIDGET_TOP, top);
	}

	/**
	 * Set the size of the widget.
	 * @param width The width value.
	 * @param height The height value.
	 */
	void Widget::setSize(int width, int height)
	{
		setProperty(MAW_WIDGET_WIDTH, width);
		setProperty(MAW_WIDGET_HEIGHT, height);
	}

	/**
	 * Size the width of this widget to fill the
	 * available space in the parent widget horizontally.
	 */
	void Widget::fillSpaceHorizontally()
	{
		setProperty(MAW_WIDGET_WIDTH, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	}

	/**
	 * Size the height of this widget to fill the
	 * available space in the parent widget vertically.
	 */
	void Widget::fillSpaceVertically()
	{
		setProperty(MAW_WIDGET_HEIGHT, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	}

	/**
	 * Size the width of this widget to wrap the
	 * content horizontally.
	 */
	void Widget::wrapContentHorizontally()
	{
		setProperty(MAW_WIDGET_WIDTH, MAW_CONSTANT_WRAP_CONTENT);
	}

	/**
	 * Size the height of this widget to wrap the
	 * content vertically.
	 */
	void Widget::wrapContentVertically()
	{
		setProperty(MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);
	}

	/**
	 * Set the background color of the widget.
	 * @param color a hexadecimal color value, e.g. 0xFF0000.
	 */
	void Widget::setBackgroundColor(int color)
	{
		char buffer[256];
		sprintf(buffer, "0x%.6X", color);
		setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
	}

	/**
	 * Set the background color of the widget.
	 * @param red Red component (range 0-255).
	 * @param green Green component (range 0-255).
	 * @param blue Blue component (range 0-255).
	 */
	void Widget::setBackgroundColor(int red, int green, int blue)
	{
		char buffer[256];
		sprintf(buffer, "0x%.2X%.2X%.2X", red, green, blue);
		setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
	}

	/**
	 * Add an event listener for this widget.
	 * Note: At present there can only be one listener.
	 * The current listener will be replaced when a new
	 * one is set. Remove the listener by setting it
	 * to NULL.
	 * @param listener The listener that will receive
	 * widget events for this widget.
	 */
	void Widget::setEventListener(WidgetEventListener* listener)
	{
		mWidgetEventListener = listener;
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to the widget's listener if one is set.
	 * Note: You can either use an event listener or override this
	 * method in a sublclass to handle events.
	 * @param widgetEventData The data for the widget event.
	 */
	void Widget::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		if (NULL != mWidgetEventListener)
		{
			mWidgetEventListener->handleWidgetEvent(this, widgetEventData);
		}
	}

	bool Widget::isEmpty() const
	{
		return (mChildren.size()==0);
	}

	} // namespace UI
} // namespace MoSync
