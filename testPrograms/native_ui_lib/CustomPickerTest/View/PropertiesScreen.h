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
 * @file PropertiesScreen.h
 * @author Bogdan Iusco
 *
 * @brief Shows widgets used to test CustomPicker properties.
 */

#ifndef CPT_PROPERTIES_SCREEN_H_
#define CPT_PROPERTIES_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/CheckBoxListener.h>
#include <NativeUI/Screen.h>

#include "ICustomPickerScreen.h"

// Forward declarations for NativeUI namespace
namespace NativeUI
{
	class Button;
	class CheckBox;
	class EditBox;
	class VeticalLayout;
}

namespace CustomPickerTest
{
	/**
	 * @brief Shows widgets used to test CustomPicker properties.
	 */
	class PropertiesScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener,
		public NativeUI::CheckBoxListener
	{
	public:
		PropertiesScreen(ICustomPickerScreen& customPickerScreen);

		virtual ~PropertiesScreen();

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

		/**
		 * This method is called when the state of the check box was changed
		 * by the user.
		 * @param checkBox The check box object that generated the event.
		 * @param state True if the check box is checked, false otherwise.
		 */
		virtual void checkBoxStateChanged(
			NativeUI::CheckBox* checkBox,
			bool state);

	private:
		void createUI();

		/**
		 * Create and add to the screen's main layout a HorizontalLayout containing:
		 * - An EditBox widget used to get a row height value for CustomPicker items.
		 * - An Button that will set the row height value when clicked.
		 */
		void createSetRowHeight();

		/**
		 * Create and add to the screen's main layout a HorizontalLayout containing:
		 * - An EditBox widget used to get a row width value for CustomPicker items.
		 * - An Button that will set the row width value when clicked.
		 */
		void createSetRowWidth();

		/**
		 * Create and add to the screen's main layout a HorizontalLayout containing:
		 * - An EditBox widget used to get a row index.
		 * - An Button that will set the selected item.
		 */
		void createSelectItem();

		/**
		 * Create and add an NativeUI Button that will show the CustomPicker
		 * items row height when clicked.
		 */
		void createGetRowHeightButton();

		/**
		 * Create and add an NativeUI Button that will show the CustomPicker
		 * items row width when clicked.
		 */
		void createGetRowWidthButton();

		/**
		 * Create and add to the screen's main layout a HorizontalLayout containing:
		 * - An Label describing the action of the CheckBox.
		 * - An CheckBox used to show/hide the CustomPicker selection indicator.
		 */
		void createSelectionIndicatorRow();

		/**
		 * Create and add to the screen's main layout a Button that will check
		 * if the selection indicator is shown when its clicked.
		 */
		void createIsShownSelectionIndicationButton();

		/**
		 * Create and add to the screen's main layout a Button that will show
		 * the selected item index.
		 */
		void createSelectedItemIndexButton();

		/**
		 * Set the CustomPicker items row height.
		 */
		void setRowHeightButtonClicked();

		/**
		 * Set the CustomPicker items row width.
		 */
		void setRowWidthButtonClicked();

		/**
		 * Set the selected CustomPicker item by index.
		 */
		void selectItemButtonCliked();

		/**
		 * Display an message box with the CustomPicker items row height value.
		 */
		void getRowHeightButtonClicked();

		/**
		 * Display an message box with the CustomPicker items row width value.
		 */
		void getRowWidthButtonClicked();

		/**
		 * Display an message box with the CustomPicker selection indicator
		 * visibility.
		 */
		void selectionIndicatorButtonClicked();

		/**
		 * Display an message box with the selected CustomPicker item index.
		 */
		void selectedItemIndexButtonClicked();

	private:
		/**
		 * Interface to the custom picker.
		 */
		ICustomPickerScreen& mCustomPickerScreenRef;

		/**
		 * Screen main layout.
		 */
		NativeUI::VerticalLayout* mMainLayoutRef;

		/**
		 * Used to get a row height value from user.
		 */
		NativeUI::EditBox* mRowHeightEditBoxRef;

		/**
		 * Used to set the row height value when clicked.
		 */
		NativeUI::Button* mRowHeightButtonRef;

		/**
		 * Used to get a row width value from user.
		 */
		NativeUI::EditBox* mRowWidthEditBoxRef;

		/**
		 * Used to set the row width value when clicked.
		 */
		NativeUI::Button* mRowWidthButtonRef;

		/**
		 * Used to get a row index from user.
		 */
		NativeUI::EditBox* mSelectItemEditBoxRef;

		/**
		 * Used to set the selected item when clicked.
		 */
		NativeUI::Button* mSelectItemButtonRef;

		/**
		 * A NativeUI Button that will show the CustomPicker row height value
		 * when clicked.
		 */
		NativeUI::Button* mShowRowHeightButtonRef;

		/**
		 * A NativeUI Button that will show the CustomPicker row width value
		 * when clicked.
		 */
		NativeUI::Button* mShowRowWidthButtonRef;

		/**
		 * Used to show/hide the CustomPicker selection indicator.
		 */
		NativeUI::CheckBox* mSelectionIndicatorCheckBoxRef;

		/**
		 * Used to check if the CustomPicker selection indicator is shown.
		 */
		NativeUI::Button* mIsSelectionIndicatorShownButtonRef;

		/**
		 * A NativeUI Button used to show the selected item index when clicked.
		 */
		NativeUI::Button* mSelectedItemIndexButtonRef;
	};
} // end of CustomPickerTest


#endif /* CPT_PROPERTIES_SCREEN_H_ */
