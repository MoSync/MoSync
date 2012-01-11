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
 * @file ApplicationBar.h
 * @author Ciprian Filipas
 *
 * \brief Class that represents a visible applicationBar. Only one applicationBar
 * is visible at a time.
 */

#ifndef NATIVEUI_APPLICATION_BAR_H_
#define NATIVEUI_APPLICATION_BAR_H_

#include "Widget.h"

namespace NativeUI
{

	/**
	 * \brief Class that represents a visible screen.
	 */
	class ApplicationBar : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		ApplicationBar();

		/**
		 * Destructor.
		 */
		~ApplicationBar();

		/**
		 * Set the IsVisible property of the application bad
		 * @param isVisible bool
		 */
		void setIsVisible(bool isVisible);

		/**
		 * Get the IsVisible property of the application bar
		 * @return isVisible bool
		 */
		bool getIsVisible();
	};

} // namespace NativeUI

#endif

/*! @} */
