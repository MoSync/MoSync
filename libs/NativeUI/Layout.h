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
 * @file Layout.h
 * @author Emma Tresanszki
 *
 * \brief Class for layouts.
 */

#ifndef NATIVEUI_LAYOUT_H_
#define NATIVEUI_LAYOUT_H_

#include "Widget.h"

namespace NativeUI
{
	/**
	 * \brief Class for layouts.
	 */
	class Layout : public Widget
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~Layout();

		/**
		 * Sets whether the layout can be scrollable or not.
		 * Note: make sure you call this after you add all the children to it,
		 * otherwise it would have no effect.
		 * @param isScrollable If true, the layout can be scrollable.
		 */
		virtual void setScrollable(const bool isScrollable);

		/**
		 * Set the left padding.
		 *
		 * @param value The left padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingLeft( const int value);

		/**
		 * Set the top padding.
		 *
		 * @param value The top padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingTop( const int value);

		/**
		 * Set the right padding.
		 *
		 * @param value The right padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingRight( const int value);

		/**
		 * Set the bottom padding.
		 *
		 * @param value The bottom padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingBottom( const int value);

    protected:
        /**
         * Constructor is protected because actual widget instances
         * should be subclasses of this class.
         * @param layoutType The string constant that identifies the widget type
         *             (one of the MAW_ constants).
         */
        Layout(const MAUtil::String& layoutType);
	};

} // namespace NativeUI

#endif

/*! @} */
