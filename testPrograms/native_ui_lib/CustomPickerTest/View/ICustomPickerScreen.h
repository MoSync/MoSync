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

/**
 * @file ICustomPickerScreen.h
 * @author Bogdan Iusco
 *
 * @brief Interface for a NativeUI Screen that shows a CustomPicker widget.
 */

#ifndef CPT_I_CUSTOM_PICKER_SCREEN_H_
#define CPT_I_CUSTOM_PICKER_SCREEN_H_

namespace NativeUI
{
	class Layout;
	class Widget;
}

namespace CustomPickerTest
{

	class ICustomPickerScreen
	{
	public:
		/**
		 * Add an item to the tested CustomPicker widget.
		 * @param layout Item to add.
		 */
		virtual void addCustomPickerItem(NativeUI::Layout* layout) = 0;

		/**
		 * Remove an item from the tested CustomPicker widget.
		 * @param layout Item to remove.
		 */
		virtual void removeCustomPickerItem(NativeUI::Layout* layout) = 0;

		/**
		 * Set the height of the CustomPicker items.
		 * @param height Value to set.
		 */
		virtual void setCustomPickerRowHeight(const int height) = 0;

		/**
		 * Set the height of the CustomPicker items.
		 * @param width Value to set.
		 */
		virtual void setCustomPickerRowWidth(const int width) = 0;

		/**
		 * Get the height of the CustomPicker items.
		 * @return Items height.
		 */
		virtual int getCustomPickerRowHeight() = 0;

		/**
		 * Get the width of the CustomPicker items.
		 * @return Items width.
		 */
		virtual int getCustomPickerRowWidth() = 0;

		/**
		 * Show the CustomPicker selection indicator.
		 */
		virtual void showSelectionIndicator() = 0;

		/**
		 * Hide the CustomPicker selection indicator.
		 */
		virtual void hideSelectionIndicator() = 0;

		/**
		 * Check if CustomPicker selection indicator is shown.
		 * @return true if shown, false otherwise.
		 */
		virtual bool isSelectionIndicatorShown() = 0;

		/**
		 * Set the selected CustomPicker item by index.
		 * @param index Item index to select(zero indexed number).
		 */
		virtual void setSelectedItemIndex(const int index) = 0;

		/**
		 * Get the selected CustomPickerItem index.
		 * @return Selected item index(zero indexed number), or #MAW_RES_ERROR
		 * if the widget has no children.
		 */
		virtual int getSelectedItemIndex() = 0;

	}; // end of ICustomPickerScreen

} // end of CustomPickerTest

#endif /* CPT_I_CUSTOM_PICKER_SCREEN_H_ */
