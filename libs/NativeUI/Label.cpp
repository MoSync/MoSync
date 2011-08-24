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
 * @author Bogdan Iusco
 *
 * Class for labels.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#include "Label.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    Label::Label(): TextWidget(MAW_LABEL)
    {
    }

    /**
     * Destructor.
     */
    Label::~Label()
     {
     }

    /**
     * Sets the typeface and style in which the text should be displayed.
     * @param fontHandle A font handle received from loading fonts using
	 *  #maFontGetName and #maFontLoadWithName syscalls.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 * @par Example
	 * \code
	 *   int nrFonts = maFontGetCount();
	 *    char buf[256];
	 *    // Get first font name.
	 *    maFontGetName(1, buf, 256);
	 *    // Load that font for size 10.
	 *    int fontHandle = maFontLoadWithName(buf, 10);
	 *    // Set that font to the label.
	 *	maWidgetSetProperty(labelHandle, MAW_LABEL_FONT_HANDLE, toString(fontHandle));
	 * \endcode
     */
    int Label::setFont(const MAHandle fontHandle)
    {
		return this->setPropertyInt(MAW_LABEL_FONT_HANDLE, fontHandle);
    }

    /**
     * Set the maximum number of lines used for rendering text.
     * To remove any maximum limit, and use as many lines as needed,
     * set the value of this property to 0.
     * @param maxNumberOfLines The maximum number of lines.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the maxNumberOfLines value
     *   was invalid.
     */
    int Label::setMaxNumberOfLines(const int maxNumberOfLines)
    {
        return setPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES, maxNumberOfLines);
    }

    /**
     * Get the maximum number of lines used for rendering text.
     * @return The maximum number of lines.
     */
    int Label::getMaxNumberOfLines() const
    {
        return getPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES);
    }

} // namespace NativeUI
