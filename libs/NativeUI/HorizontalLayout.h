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
 * @file HorizontalLayout.h
 * @author Mikael Kindborg
 *
 * \brief Class for horizontal layout of widgets.
 */

#ifndef NATIVEUI_HORIZONTAL_LAYOUT_H_
#define NATIVEUI_HORIZONTAL_LAYOUT_H_

#include "Widget.h"
#include "Layout.h"

namespace NativeUI
{
	/**
	 * \brief A horizontal layout is a layout that stacks widgets in the
	 * horizontal axis.
	 */
	class HorizontalLayout : public Layout
	{
	public:
		/**
		 * Constructor.
		 * By default, when a horizontal layout is created it fills the
		 * available space in the parent widget horizontally and vertically.
		 */
		HorizontalLayout();

		/**
		 * Destructor.
		 */
		virtual ~HorizontalLayout();

		/**
		 * Sets how the children in the layout should be aligned in the
		 * vertical axis.
		 * @param alignment One of the constants #MAW_ALIGNMENT_TOP,
		 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_BOTTOM.
		 */
		virtual void setChildVerticalAlignment(const MAUtil::String alignment);

		/**
		 * Sets how the children in the layout should be aligned in the
		 * horizontal axis.
		 * @param alignment One of the constants #MAW_ALIGNMENT_LEFT,
		 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_RIGHT.
		 */
		virtual void setChildHorizontalAlignment(const MAUtil::String alignment);

	};

} // namespace NativeUI

#endif
