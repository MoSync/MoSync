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

#ifndef MOSYNC_UI_SCREEN_H_
#define MOSYNC_UI_SCREEN_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class that represents a visible screen.
	 */
	class Screen : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		Screen();

		/**
		 * Destructor.
		 */
		virtual ~Screen();

		/**
		 * Set the title of the screen.
		 * @param title The screen title.
		 */
		virtual void setTitle(const MAUtil::String& title);

		/**
		 * Set the main widget of the screen.
		 * Note: A screen can only have one main widget.
		 * Use a layout as main widget and add child
		 * widgets to the layout.
		 * @param widget The widget to be set as main widget.
		 */
		virtual void setMainWidget(Widget* widget);

		/**
		 * Show a screen. Only one screen at a time is visible.
		 * The previous screen will be hidden when showing a screen.
		 * Note: This method is only applicable to screens.
		 */
		virtual void show();

	protected:
		/**
		 * Protected because only subclasses should use this constructor.
		 * @widgetType The string constant that identifies the widget type
		 * (one of the MAW_ constants).
		 */
		Screen(const MAUtil::String& widgetType);
	};

	} // namespace UI
} // namespace MoSync

#endif
