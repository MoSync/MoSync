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
		/**
		 * Constructor.
		 * @param customPickerScreen Interface to the custom picker.
		 */
		PropertiesScreen(ICustomPickerScreen& customPickerScreen);

		/**
		 * Destructor.
		 */
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
		/**
		 * Create screen UI.
		 */
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
		ICustomPickerScreen& mCustomPickerScreen;

		/**
		 * Screen main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Used to get a row height value from user.
		 */
		NativeUI::EditBox* mRowHeightEditBox;

		/**
		 * Used to set the row height value when clicked.
		 */
		NativeUI::Button* mRowHeightButton;

		/**
		 * Used to get a row width value from user.
		 */
		NativeUI::EditBox* mRowWidthEditBox;

		/**
		 * Used to set the row width value when clicked.
		 */
		NativeUI::Button* mRowWidthButton;

		/**
		 * Used to get a row index from user.
		 */
		NativeUI::EditBox* mSelectItemEditBox;

		/**
		 * Used to set the selected item when clicked.
		 */
		NativeUI::Button* mSelectItemButton;

		/**
		 * A NativeUI Button that will show the CustomPicker row height value
		 * when clicked.
		 */
		NativeUI::Button* mShowRowHeightButton;

		/**
		 * A NativeUI Button that will show the CustomPicker row width value
		 * when clicked.
		 */
		NativeUI::Button* mShowRowWidthButton;

		/**
		 * Used to show/hide the CustomPicker selection indicator.
		 */
		NativeUI::CheckBox* mSelectionIndicatorCheckBox;

		/**
		 * Used to check if the CustomPicker selection indicator is shown.
		 */
		NativeUI::Button* mIsSelectionIndicatorShownButton;

		/**
		 * A NativeUI Button used to show the selected item index when clicked.
		 */
		NativeUI::Button* mSelectedItemIndexButton;
	};
} // end of CustomPickerTest


#endif /* CPT_PROPERTIES_SCREEN_H_ */
