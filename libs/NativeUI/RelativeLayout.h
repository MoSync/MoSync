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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file RelativeLayout.h
 * @author Emma Tresanszki
 *
 * \brief Class for relative layout of widgets.
 */

#ifndef NATIVEUI_RELATIVE_LAYOUT_H_
#define NATIVEUI_RELATIVE_LAYOUT_H_

#include "Widget.h"
#include "Layout.h"

namespace NativeUI
{
	/**
	 * \brief A relative layout is a layout that layouts widgets relative
	 * to its coordinate system.
	 */
	class RelativeLayout : public Layout
	{
	public:
		/**
		 * Constructor.
		 */
		RelativeLayout();

		/**
		 * Destructor.
		 */
		virtual ~RelativeLayout();

		/**
		 * Sets the content offset.
		 * The layout will be scrolled to the given coordinate so its
		 * left-top visible corner will match the content offset point.
		 *
		 * Platform: iOS
		 * @param xCoord The left coordinate of the content offset point.
		 * @param yCoord The top coordinate of the content offset point.
		 */
		void setContentOffset(const int xCoord, const int yCoord);

		/**
		 * Get the x coordinate of the content offset point.
		 * The content offset point repesents the visible top-left point
		 * of a layout.
		 * Platform: iOS.
		 * @return The left coordinate.
		 */
		int getContentOffsetCoordX();

		/**
		 * Get the y coordinate of the content offset point.
		 * The content offset point repesents the visible top-left point
		 * of a layout.
		 * Platform: iOS.
		 * @return The top coordinate.
		 */
		int getContentOffsetCoordY();
	};

} // namespace NativeUI

#endif

/*! @} */
