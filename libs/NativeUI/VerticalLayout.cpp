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
 * @file VerticalLayout.cpp
 * @author Mikael Kindborg
 *
 * Class for vertical layout of widgets.
 */

#include "VerticalLayout.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	VerticalLayout::VerticalLayout() :
		Widget(MAW_VERTICAL_LAYOUT)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	VerticalLayout::~VerticalLayout()
	{
	}
	/**
	 * Set how the children in the layout should be aligned in the horizontal axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_LEFT,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_RIGHT.
	 */
	void VerticalLayout::setChildHorizontalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, alignment);
	}

	/**
	 * Set how the children in the layout should be aligned in the vertical axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_TOP,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_BOTTOM.
	 */
	void VerticalLayout::setChildVerticalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, alignment);
	}

} // namespace NativeUI
