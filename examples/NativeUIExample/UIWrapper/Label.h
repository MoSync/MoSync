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
 * @file Label.h
 * @author Mikael Kindborg
 *
 * Class for label of widgets.
 */

#ifndef MOSYNC_UI_LABEL_H_
#define MOSYNC_UI_LABEL_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for labels.
	 */
	class Label : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		Label();

		/**
		 * Destructor.
		 */
		virtual ~Label();

		/*
		 * Sets the text of the label.
		 * @param text The text of the label.
		 */
		void setText(const MAUtil::String& text);

		/**
		 * Center the text of the widget horizontally.
		 * Note: Not all widgets support this property.
		 */
		void centerTextHorizontally();

		/**
		 * Center the text of the widget vertically.
		 * Note: Not all widgets support this property.
		 */
		void centerTextVertically();

		/*
		 * Sets the font color of the label.
		 * @param color The font color of the label.
		 */
		void setFontColor(const int color);

		/*
		 * Sets the font size of the label.
		 * @param size The font size of the label.
		 */
		void setFontSize(const int size);
	};

	} // namespace UI
} // namespace MoSync

#endif
