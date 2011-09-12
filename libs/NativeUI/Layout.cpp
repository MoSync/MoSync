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
 * @file Layout.cpp
 * @author Emma Tresanszki
 *
 * Class for layouts.
 */

#include "Layout.h"
#include "WidgetUtil.h"

namespace NativeUI
{

    /**
     * Constructor is protected because actual widget instances
     * should be subclasses of this class.
     * @widgetType The string constant that identifies the widget type
     *             (one of the MAW_ constants).
     */
    Layout::Layout(const MAUtil::String& widgetType) :
        Widget(widgetType)
    {
    }

    /**
     * Destructor.
     */
    Layout::~Layout()
     {
     }

	/**
	 * Sets whether the layout can be scrollable or not.
	 * @param If true, the layout can be scrollable.
	 */
    void Layout::setScrollable(const bool isScrollable)
    {
		setProperty("isScrollable", (isScrollable ? "true" : "false") );
    }

} // namespace NativeUI
