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
 * @file CustomPickerScreen.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen that shows a CustomPicker widget.
 */

#ifndef CPT_CUSTOM_PICKER_SCREEN_H_
#define CPT_CUSTOM_PICKER_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/CustomPickerListener.h>
#include <NativeUI/Screen.h>

#include "ICustomPickerScreen.h"

// Forward declarations for NativeUI
namespace NativeUI
{
	class Button;
	class CustomPicker;
	class Label;
	class Layout;
	class VerticalLayout;
}

namespace CustomPickerTest
{

	/**
	 * @brief NativeUI Screen that shows a CustomPicker widget.
	 */
	class CustomPickerScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener,
		public NativeUI::CustomPickerListener,
		public ICustomPickerScreen
	{
	public:
		/**
		 * Constructor.
		 */
		CustomPickerScreen();

		/**
		 * Destructor.
		 */
		virtual ~CustomPickerScreen();

		/**
		 * Add an item to the tested CustomPicker widget.
		 * From ICustomPickerScreen.
		 * @param layout Item to add.
		 */
		virtual void addCustomPickerItem(NativeUI::Layout* layout);

		/**
		 * Remove an item from the tested CustomPicker widget.
		 * From ICustomPickerScreen.
		 * @param layout Item to remove.
		 */
		virtual void removeCustomPickerItem(NativeUI::Layout* layout);

		/**
		 * Set the height of the CustomPicker items.
		 * From ICustomPickerScreen.
		 * @param height Value to set.
		 */
		virtual void setCustomPickerRowHeight(const int height);

		/**
		 * Set the height of the CustomPicker items.
		 * From ICustomPickerScreen.
		 * @param width Value to set.
		 */
		virtual void setCustomPickerRowWidth(const int width);

		/**
		 * Get the height of the CustomPicker items.
		 * From ICustomPickerScreen.
		 * @return Items height.
		 */
		virtual int getCustomPickerRowHeight();

		/**
		 * Get the width of the CustomPicker items.
		 * From ICustomPickerScreen.
		 * @return Items width.
		 */
		virtual int getCustomPickerRowWidth();

		/**
		 * Show the CustomPicker selection indicator.
		 * From ICustomPickerScreen.
		 */
		virtual void showSelectionIndicator();

		/**
		 * Hide the CustomPicker selection indicator.
		 * From ICustomPickerScreen.
		 */
		virtual void hideSelectionIndicator();

		/**
		 * Check if CustomPicker selection indicator is shown.
		 * From ICustomPickerScreen.
		 * @return true if shown, false otherwise.
		 */
		virtual bool isSelectionIndicatorShown();

		/**
		 * Set the selected CustomPicker item by index.
		 * From ICustomPickerScreen.
		 * @param index Item index to select(zero indexed number).
		 */
		virtual void setSelectedItemIndex(const int index);

		/**
		 * Get the selected CustomPickerItem index.
		 * From ICustomPickerScreen.
		 * @return Selected item index(zero indexed number), or #MAW_RES_ERROR
		 * if the widget has no children.
		 */
		virtual int getSelectedItemIndex();

		/**
		 * This method is called when user selects a new item.
		 * @param customPicker The object that generated the event.
		 * @param selectedItem The selected object.
		 * @param selectedItemIndex The index of the selected object.
		 */
		virtual void customPickerItemChanged(
			NativeUI::CustomPicker* customPicker,
			NativeUI::Widget* selectedItem,
			const int selectedItemIndex);

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

	private:
		/**
		 * Create screen UI.
		 */
		void createUI();

		/**
		 * Add items to tested CustomPicker.
		 */
		void populateCustomPicker();

		/**
		 * Create an CustomPicker item.
		 * @return The created item. Its ownership is passed to the caller.
		 */
		NativeUI::Layout* createCustomPickerItem();

	private:
		/**
		 * Screen main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Tested CustomPicker.
		 */
		NativeUI::CustomPicker* mCustomPicker;

		/**
		 * Shows the selected CustomPicker item index.
		 */
		NativeUI::Label* mSelectedItemIndexLabel;

		/**
		 * Add an item to the CustomPicker,
		 */
		NativeUI::Button* mAddItemButton;

		/**
		 * Remove selected item from the CustomPicker,
		 */
		NativeUI::Button* mRemoveItemButton;
	};
} // end of CustomPickerTest

#endif /* CPT_CUSTOM_PICKER_SCREEN_H_ */
