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
	 * Constructor.
	 */
	Screen::Screen() :
		Widget(MAW_SCREEN)
	{
	}

	/**
	 * Protected because only subclasses should use this constructor.
	 * @widgetType The string constant that identifies the widget type
	 * (one of the MAW_ constants).
	 */
	Screen::Screen(const MAUtil::String& widgetType) :
		Widget(widgetType)
	{
	}

	/**
	 * Destructor.
	 */
	Screen::~Screen()
	{
	}

	/**
	 * Set the title of the screen. The title is displayed
	 * on tab screens.
	 * @param title The screen title.
	 */
	void Screen::setTitle(const MAUtil::String& title)
	{
		setProperty(MAW_SCREEN_TITLE, title.c_str());
	}

	/**
	 * Set the icon of the screen. The icon is displayed
	 * on tab screens.
	 * @param imageHandle Handle to an image with the icon.
	 */
	void Screen::setIcon(MAHandle imageHandle)
	{
		setProperty(MAW_SCREEN_ICON, imageHandle);
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

	/**
	 * Handle pointer presses.
	 */
	void Screen::handlePointerPressed(MAPoint2d p)
	{

	}

	/**
	 * Handle pointer moves.
	 */
	void Screen::handlePointerMoved(MAPoint2d p)
	{

	}

	/**
	 * Handle pointer releases.
	 */
	void Screen::handlePointerReleased(MAPoint2d p)
	{

	}


	} // namespace UI
} // namespace MoSync

