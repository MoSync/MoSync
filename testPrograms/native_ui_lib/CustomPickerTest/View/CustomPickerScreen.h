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
		NativeUI::VerticalLayout* mMainLayoutRef;

		/**
		 * Tested CustomPicker.
		 */
		NativeUI::CustomPicker* mCustomPickerRef;

		/**
		 * Shows the selected CustomPicker item index.
		 */
		NativeUI::Label* mSelectedItemIndexLabelRef;

		/**
		 * Add an item to the CustomPicker,
		 */
		NativeUI::Button* mAddItemButtonRef;

		/**
		 * Remove selected item from the CustomPicker,
		 */
		NativeUI::Button* mRemoveItemButtonRef;
	};
} // end of CustomPickerTest

#endif /* CPT_CUSTOM_PICKER_SCREEN_H_ */
