/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
