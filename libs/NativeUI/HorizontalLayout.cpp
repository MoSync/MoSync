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
 * @file HorizontalLayout.cpp
 * @author Mikael Kindborg
 *
 * Class for horizontal layout of widgets.
 */

#include "HorizontalLayout.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 * By default, when a horizontal layout is created it fills the
	 * available space in the parent widget horizontally and vertically.
	 */
	HorizontalLayout::HorizontalLayout() :
		Widget(MAW_HORIZONTAL_LAYOUT)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	HorizontalLayout::~HorizontalLayout()
	{
	}

	/**
	 * Sets how the children in the layout should be aligned in the
	 * vertical axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_TOP,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_BOTTOM.
	 */
	void HorizontalLayout::setChildVerticalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, alignment);
	}

	/**
	 * Sets how the children in the layout should be aligned in the
	 * horizontal axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_LEFT,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_RIGHT.
	 */
	void HorizontalLayout::setChildHorizontalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, alignment);
	}

} // namespace NativeUI
