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
 * @file Screen.h
 * @author Mikael Kindborg
 *
 * Class that represents a visible screen. Only one screen
 * is visible at a time.
 */

#include "Screen.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor. Use one of the create methods in class
	 * WidgetManager to create a widget instance. Do not create
	 * an instance of this class with new unless you are implementing
	 * your custom UI library.
	 * @widgetHandle The handle of the widget.
	 * @widgetManager The widget manager for this widget.
	 */
	Screen::Screen(MAHandle widgetHandle, WidgetManager* widgetManager) :
		Widget(widgetHandle, widgetManager)
	{
	}

	/**
	 * Destructor.
	 */
	Screen::~Screen()
	{
	}

	/**
	 * Set the title of the screen.
	 * @param title The screen title.
	 */
	void Screen::setTitle(const MAUtil::String& title)
	{
		setProperty("title", title.c_str());
	}

	/**
	 * Set the main widget of the screen.
	 * Note: A screen can only have one main widget.
	 * Use a layout as main widget and add child
	 * widgets to the layout.
	 * @param widget The widget to be set as main widget.
	 */
	void Screen::setMainWidget(Widget* widget)
	{
		addChild(widget);
	}

	/**
	 * Show a screen. Only one screen at a time is visible.
	 * The previous screen will be hidden when showing a screen.
	 * Note: This method is only applicable to screens.
	 */
	void Screen::show()
	{
		maWidgetScreenShow(getWidgetHandle());
	}

	} // namespace UI
} // namespace MoSync
