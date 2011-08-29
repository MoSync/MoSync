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
 * @author Bogdan Iusco
 *
 * Class for labels.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#ifndef NATIVEUI_LABEL_H_
#define NATIVEUI_LABEL_H_

#include "TextWidget.h"

namespace NativeUI
{
    /**
     * Class for labels.
     * Methods for setting the text, alignment, font size and color are inherited
     * from TextWidget.
     */
    class Label : public TextWidget
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
        virtual int setMaxNumberOfLines(const int maxNumberOfLines);

        /**
         * Get the maximum number of lines used for rendering text.
         * @return The maximum number of lines.
         */
        virtual int getMaxNumberOfLines() const;
    };

} // namespace NativeUI

#endif /* NATIVEUI_LABEL_H_ */
