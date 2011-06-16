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
 * @file Label.cpp
 * @author Emma Tresanszki
 *
 * Class for labels. Create a label instance by
 * using the WidgetManager.
 */

#include "Label.h"

namespace MoSync
{
	namespace UI
	{

	Label::Label() :
		Widget(MAW_LABEL)
	{
		// Set some common default values to
		// simplify when creating labels.
		this->fillSpaceVertically();
		this->fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	Label::~Label()
	{
	}
	/*
	 * Sets the text of the label.
	 * @param text The text of the label.
	 */
	void Label::setText(const MAUtil::String& text)
	{
		setProperty(MAW_LABEL_TEXT, text);
	}

	/**
	 * Center the text of the widget horizontally.
	 * Note: Not all widgets support this property.
	 */
	void Label::centerTextHorizontally()
	{
		setProperty(MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	}

	/**
	 * Center the text of the widget vertically.
	 * Note: Not all widgets support this property.
	 */
	void Label::centerTextVertically()
	{
		setProperty(MAW_LABEL_TEXT_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	}

	/*
	 * Sets the font color of the label.
	 * @param color The font color of the label.
	 */
	void Label::setFontColor(const int color)
	{
		char buffer[256];
		sprintf(buffer, "0x%.6X", color);
		setProperty(MAW_LABEL_FONT_COLOR, buffer);
	}

	/*
	 * Sets the font size of the label.
	 * @param size The font size of the label.
	 */
	void Label::setFontSize(const int size)
	{
		setProperty(MAW_LABEL_FONT_SIZE, size);

	}

	} // namespace UI
} // namespace MoSync
