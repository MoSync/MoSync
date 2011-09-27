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
	};

} // namespace NativeUI

#endif
