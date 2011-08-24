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
 * @file TextWidget.h
 * @author Mikael Kindborg
 *
 * Base class for widgets that have a text property.
 */

#ifndef MOSYNC_UI_TEXTWIDGET_H_
#define MOSYNC_UI_TEXTWIDGET_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Base class for widgets that have a text property.
	 */
	class TextWidget : public Widget
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~TextWidget();

		/**
		 * Set the text of the widget.
		 * Note: Not all widgets support this property.
		 * @param text The text to be displayed by the widget.
		 */
		virtual void setText(const MAUtil::String& text);

		/**
		 * Center the text of the widget horizontally.
		 * Note: Not all widgets support this property.
		 */
		virtual void centerTextHorizontally();

		/**
		 * Center the text of the widget vertically.
		 * Note: Not all widgets support this property.
		 */
		virtual void centerTextVertically();

		/**
		 * Sets the font color of the widget.
		 * @param color The font color of the widget.
		 */
		void setFontColor(const int color);

		/**
		 * Set the font color of the widget.
		 * Note: Not all widgets support this property.
		 * @param red Red component (range 0-255).
		 * @param green Green component (range 0-255).
		 * @param blue Blue component (range 0-255).
		 */
		virtual void setFontColor(int red, int green, int blue);

		/**
		 * Set the font size of the widget.
		 * Note: Not all widgets support this property.
		 * @param size The font size.
		 */
		virtual void setFontSize(int size);

	protected:
		/**
		 * Constructor is protected because actual widget instances
		 * should be subclasses of this class.
		 * @widgetType The string constant that identifies the widget type
		 * (one of the MAW_ constants).
		 */
		TextWidget(const MAUtil::String& widgetType);
	};

	} // namespace UI
} // namespace MoSync

#endif
