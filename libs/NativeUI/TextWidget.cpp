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
 * @file TextWidget.cpp
 * @author Bogdan Iusco
 *
 * Base class for widgets that have a text property.
 */

#include "TextWidget.h"
#include "WidgetUtil.h"

namespace NativeUI
{

    /**
     * Constructor is protected because actual widget instances
     * should be subclasses of this class.
     * @widgetType The string constant that identifies the widget type
     *             (one of the MAW_ constants).
     */
    TextWidget::TextWidget(const MAUtil::String& widgetType) :
        Widget(widgetType)
    {
    }

    /**
     * Destructor.
     */
     TextWidget::~TextWidget()
     {
     }

    /**
     * Set the text displayed by the widget.
     * @param text The given text.
     */
    void TextWidget::setText(const MAUtil::String& text)
    {
        this->setProperty(MAW_LABEL_TEXT, text);
    }

    /**
     * Get the text displayed by the widget.
     * @return The text displayed by the widget.
     */
     MAUtil::String TextWidget::getText() const
     {
         return this->getPropertyString(MAW_LABEL_TEXT);
     }

     /**
      * Set the vertical alignment of the text inside the widget.
      * If the widget's height is WRAP_CONTENT this property has no effect.
      * @param alignment One of the constants:
      * - #MAW_ALIGNMENT_TOP
      * - #MAW_ALIGNMENT_CENTER
      * - #MAW_ALIGNMENT_BOTTOM
      * @return Any of the following result codes:
      * - #MAW_RES_OK if the property could be set.
      * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
      */
     int TextWidget::setTextVerticalAlignment(const MAUtil::String& alignment)
     {
         return this->setProperty(MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, alignment);
     }

     /**
      * Set the horizontal alignment of the text inside the widget.
      * If the widget's width is WRAP_CONTENT this property has no effect.
      * @param alignment one of the constants:
      * - #MAW_ALIGNMENT_LEFT
      * - #MAW_ALIGNMENT_CENTER
      * - #MAW_ALIGNMENT_RIGHT
      * @return Any of the following result codes:
      * - #MAW_RES_OK if the property could be set.
      * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
      */
     int TextWidget::setTextHorizontalAlignment(const MAUtil::String& alignment)
     {
		return this->setProperty(
			MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
			alignment);
     }

    /**
     * Set the font color of the widget's text.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
     */
    int TextWidget::setFontColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return this->setProperty(MAW_BUTTON_FONT_COLOR, buffer);
    }

    /**
     * Set the font size in points of the widget's text.
     * @param size A float that specifies the number of points.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
     */
    int TextWidget::setFontSize(const float size)
    {
        return this->setPropertyFloat(MAW_BUTTON_FONT_SIZE, size);
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
	 *	maWidgetSetProperty(
	 *	    labelHandle, MAW_LABEL_FONT_HANDLE, toString(fontHandle));
	 * \endcode
     */
    int TextWidget::setFont(const MAHandle fontHandle)
    {
		return this->setPropertyInt(MAW_LABEL_FONT_HANDLE, fontHandle);
    }

} // namespace NativeUI
