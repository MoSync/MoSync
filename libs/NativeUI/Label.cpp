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
    int Label::getMaxNumberOfLines()
    {
        return getPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES);
    }

} // namespace NativeUI
