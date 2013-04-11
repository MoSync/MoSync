/*
Copyright (C) 2013 MoSync AB

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
 * @file CustomPicker.h
 * @author Bogdan Iusco
 *
 * \brief A spinning-wheel widget used to shown one or more items(layout widgets).
 */

#ifndef NATIVEUI_CUSTOM_PICKER_H_
#define NATIVEUI_CUSTOM_PICKER_H_

#include "Widget.h"

namespace NativeUI
{
	/**
	 * \brief A spinning-wheel widget used to shown one or more items(layout widgets).
	 */
	class CustomPicker : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		CustomPicker();

		/**
		 * Destructor.
		 */
		virtual ~CustomPicker();

	};

} // namespace NativeUI

#endif /* NATIVEUI_CUSTOM_PICKER_H_ */

/*! @} */
