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
 * @file CustomPickerListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for CustomPicker events.
 */

#ifndef NATIVEUI_CUSTOM_PICKER_LISTENER_H_
#define NATIVEUI_CUSTOM_PICKER_LISTENER_H_

namespace NativeUI
{

	// Forward declaration.
	class CustomPicker;
	class Widget;

	/**
	 * \brief Listener for CustomPicker events.
	 */
	class CustomPickerListener
	{
	public:
		/**
		 * This method is called when user selects a new item.
		 * @param customPicker The object that generated the event.
		 * @param selectedItem The selected object.
		 * @param selectedItemIndex The index of the selected object. First item
		 * has index zero.
		 */
		virtual void customPickerItemChanged(
			CustomPicker* customPicker,
			Widget* selectedItem,
			const int selectedItemIndex) = 0;
	};

} // namespace NativeUI

#endif /* NATIVEUI_CUSTOM_PICKER_LISTENER_H_ */

/*! @} */
