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
 * @file Layout.h
 * @author Mikael Kindborg
 *
 * Class for layout of widgets. Create a layout instance by
 * using the WidgetManager.
 */

#ifndef MOSYNC_UI_LAYOUT_H_
#define MOSYNC_UI_LAYOUT_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for buttons.
	 */
	class Layout : public Widget
	{
	public:
		/**
		 * Constructor. Use one of the create methods in class
		 * WidgetManager to create a widget instance. Do not create
		 * an instance of this class with new unless you are implementing
		 * your custom UI library.
		 * @widgetHandle The handle of the widget.
		 * @widgetManager The widget manager for this widget.
		 */
		Layout(MAHandle widgetHandle, WidgetManager* widgetManager);

		/**
		 * Destructor.
		 */
		virtual ~Layout();
	};

	} // namespace UI
} // namespace MoSync

#endif
