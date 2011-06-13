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
 * @file Button.h
 * @author Mikael Kindborg
 *
 * Class for buttons. Create a button instance by
 * using the WidgetManager.
 */

#ifndef MOSYNC_UI_BUTTON_H_
#define MOSYNC_UI_BUTTON_H_

#include "TextWidget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for buttons.
	 */
	class Button : public TextWidget
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
		Button(MAHandle widgetHandle, WidgetManager* widgetManager);

		/**
		 * Destructor.
		 */
		virtual ~Button();
	};

	} // namespace UI
} // namespace MoSync

#endif
