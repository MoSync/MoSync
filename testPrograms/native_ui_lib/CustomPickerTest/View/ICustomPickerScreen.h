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
