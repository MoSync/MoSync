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
 * @file PropertiesScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Shows widgets used to test CustomPicker properties.
 */

#define PROPERTIES_SCREEN_TITLE "Properties"
#define SET_ROW_HEIGHT_BUTTON_TEXT "Set row height"
#define SET_ROW_WIDTH_BUTTON_TEXT "Set row width"
#define GET_ROW_HEIGHT_BUTTON_TEXT "Get row height"
#define GET_ROW_WIDTH_BUTTON_TEXT "Get row width"

#define SELECTION_INDICATOR_LABEL_TEXT "Show/hide selection indicator"
#define IS_SELECTOON_INDICATOR_SHOWN_BTN_TEXT "Is selection indicator shown"

#define SELECT_ITEM_BUTTON_TEXT "Select item index"
#define SELECTED_ITEM_BUTTON_TEXT "Get selected item index"

#include <NativeUI/Button.h>
#include <NativeUI/CheckBox.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/Label.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>

#include <MAUtil/String.h>
#include <MAUtil/util.h>

#include "PropertiesScreen.h"
#include "ViewUtils.h"

namespace CustomPickerTest
{

	PropertiesScreen::PropertiesScreen(ICustomPickerScreen& customPickerScreen):
		mCustomPickerScreenRef(customPickerScreen)
	{
		mMainLayoutRef = NULL;
		mRowHeightButtonRef = NULL;
		mRowWidthEditBoxRef = NULL;
		mRowWidthButtonRef = NULL;
		mSelectItemEditBoxRef = NULL;
		mSelectItemButtonRef = NULL;
		mShowRowHeightButtonRef = NULL;
		mShowRowWidthButtonRef = NULL;
		mSelectionIndicatorCheckBoxRef = NULL;
		mIsSelectionIndicatorShownButtonRef = NULL;
		mSelectedItemIndexButtonRef = NULL;

		setTitle(PROPERTIES_SCREEN_TITLE);
		createUI();

		mRowHeightButtonRef->addButtonListener(this);
		mRowWidthButtonRef->addButtonListener(this);
		mSelectItemButtonRef->addButtonListener(this);
		mShowRowHeightButtonRef->addButtonListener(this);
		mShowRowWidthButtonRef->addButtonListener(this);
		mIsSelectionIndicatorShownButtonRef->addButtonListener(this);
		mSelectedItemIndexButtonRef->addButtonListener(this);

		mSelectionIndicatorCheckBoxRef->addCheckBoxListener(this);
	}

	PropertiesScreen::~PropertiesScreen()
	{
		mRowHeightButtonRef->removeButtonListener(this);
		mRowWidthButtonRef->removeButtonListener(this);
		mSelectItemButtonRef->removeButtonListener(this);
		mShowRowHeightButtonRef->removeButtonListener(this);
		mShowRowWidthButtonRef->removeButtonListener(this);
		mIsSelectionIndicatorShownButtonRef->removeButtonListener(this);
		mSelectedItemIndexButtonRef->removeButtonListener(this);

		mSelectionIndicatorCheckBoxRef->removeCheckBoxListener(this);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void PropertiesScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mRowHeightButtonRef)
		{
			setRowHeightButtonClicked();
		}
		else if (button == mRowWidthButtonRef)
		{
			setRowWidthButtonClicked();
		}
		else if (button == mShowRowHeightButtonRef)
		{
			getRowHeightButtonClicked();
		}
		else if (button == mShowRowWidthButtonRef)
		{
			getRowWidthButtonClicked();
		}
		else if (button == mIsSelectionIndicatorShownButtonRef)
		{
			selectionIndicatorButtonClicked();
		}
		else if (button == mSelectItemButtonRef)
		{
			selectItemButtonCliked();
		}
		else if (button == mSelectedItemIndexButtonRef)
		{
			selectedItemIndexButtonClicked();
		}
	}

	/**
	 * This method is called when the state of the check box was changed
	 * by the user.
	 * @param checkBox The check box object that generated the event.
	 * @param state True if the check box is checked, false otherwise.
	 */
	void PropertiesScreen::checkBoxStateChanged(
		NativeUI::CheckBox* checkBox,
		bool state)
	{
		if (state)
		{
			mCustomPickerScreenRef.showSelectionIndicator();
		}
		else
		{
			mCustomPickerScreenRef.hideSelectionIndicator();
		}
	}

	void PropertiesScreen::createUI()
	{
		mMainLayoutRef = new NativeUI::VerticalLayout();
		setMainWidget(mMainLayoutRef);

		createSetRowHeight();
		createSetRowWidth();
		createSelectItem();
		createGetRowHeightButton();
		createGetRowWidthButton();
		createSelectionIndicatorRow();
		createIsShownSelectionIndicationButton();
		createSelectedItemIndexButton();

		int rowHeight = mCustomPickerScreenRef.getCustomPickerRowHeight();
		int rowWidth = mCustomPickerScreenRef.getCustomPickerRowWidth();
		MAUtil::String rowHeightString = MAUtil::integerToString(rowHeight);
		MAUtil::String rowWidthString = MAUtil::integerToString(rowWidth);
		mRowHeightEditBoxRef->setText(rowHeightString);
		mRowWidthEditBoxRef->setText(rowHeightString);
	}

	/**
	 * Create and add to the screen's main layout a HorizontalLayout containing:
	 * - An EditBox widget used to get a row height value for CustomPicker items.
	 * - An Button that will set the row height value when clicked.
	 */
	void PropertiesScreen::createSetRowHeight()
	{
		NativeUI::HorizontalLayout* hLayout = new NativeUI::HorizontalLayout();
		hLayout->fillSpaceHorizontally();
		hLayout->wrapContentVertically();

		mRowHeightEditBoxRef = new NativeUI::EditBox();
		mRowHeightEditBoxRef->fillSpaceHorizontally();
		mRowHeightEditBoxRef->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mRowHeightEditBoxRef);

		mRowHeightButtonRef = new NativeUI::Button();
		mRowHeightButtonRef->fillSpaceHorizontally();
		mRowHeightButtonRef->setText(SET_ROW_HEIGHT_BUTTON_TEXT);
		hLayout->addChild(mRowHeightButtonRef);

		mMainLayoutRef->addChild(hLayout);
	}

	/**
	 * Create and add to the screen's main layout a HorizontalLayout containing:
	 * - An EditBox widget used to get a row width value for CustomPicker items.
	 * - An Button that will set the row width value when clicked.
	 */
	void PropertiesScreen::createSetRowWidth()
	{
		NativeUI::HorizontalLayout* hLayout = new NativeUI::HorizontalLayout();
		hLayout->fillSpaceHorizontally();
		hLayout->wrapContentVertically();

		mRowWidthEditBoxRef = new NativeUI::EditBox();
		mRowWidthEditBoxRef->fillSpaceHorizontally();
		mRowWidthEditBoxRef->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mRowWidthEditBoxRef);

		mRowWidthButtonRef = new NativeUI::Button();
		mRowWidthButtonRef->fillSpaceHorizontally();
		mRowWidthButtonRef->setText(SET_ROW_WIDTH_BUTTON_TEXT);
		hLayout->addChild(mRowWidthButtonRef);

		mMainLayoutRef->addChild(hLayout);
	}

	/**
	 * Create and add to the screen's main layout a HorizontalLayout containing:
	 * - An EditBox widget used to get a row index.
	 * - An Button that will set the selected item.
	 */
	void PropertiesScreen::createSelectItem()
	{
		NativeUI::HorizontalLayout* hLayout = new NativeUI::HorizontalLayout();
		hLayout->fillSpaceHorizontally();
		hLayout->wrapContentVertically();

		mSelectItemEditBoxRef = new NativeUI::EditBox();
		mSelectItemEditBoxRef->fillSpaceHorizontally();
		mSelectItemEditBoxRef->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mSelectItemEditBoxRef);

		mSelectItemButtonRef = new NativeUI::Button();
		mSelectItemButtonRef->fillSpaceHorizontally();
		mSelectItemButtonRef->setText(SELECT_ITEM_BUTTON_TEXT);
		hLayout->addChild(mSelectItemButtonRef);

		mMainLayoutRef->addChild(hLayout);
	}

	/**
	 * Create and add an NativeUI Button that will show the CustomPicker
	 * items row height when clicked.
	 */
	void PropertiesScreen::createGetRowHeightButton()
	{
		mShowRowHeightButtonRef = new NativeUI::Button();
		mShowRowHeightButtonRef->fillSpaceHorizontally();
		mShowRowHeightButtonRef->setText(GET_ROW_HEIGHT_BUTTON_TEXT);
		mMainLayoutRef->addChild(mShowRowHeightButtonRef);
	}

	/**
	 * Create and add an NativeUI Button that will show the CustomPicker
	 * items row width when clicked.
	 */
	void PropertiesScreen::createGetRowWidthButton()
	{
		mShowRowWidthButtonRef = new NativeUI::Button();
		mShowRowWidthButtonRef->fillSpaceHorizontally();
		mShowRowWidthButtonRef->setText(GET_ROW_WIDTH_BUTTON_TEXT);
		mMainLayoutRef->addChild(mShowRowWidthButtonRef);
	}

	/**
	 * Create and add to the screen's main layout a HorizontalLayout containing:
	 * - An Label describing the action of the CheckBox.
	 * - An CheckBox used to show/hide the CustomPicker selection indicator.
	 */
	void PropertiesScreen::createSelectionIndicatorRow()
	{
		NativeUI::HorizontalLayout* hLayout = new NativeUI::HorizontalLayout();
		hLayout->fillSpaceHorizontally();
		hLayout->wrapContentVertically();

		NativeUI::Label* label = new NativeUI::Label();
		label->fillSpaceHorizontally();
		label->setText(SELECTION_INDICATOR_LABEL_TEXT);
		hLayout->addChild(label);

		mSelectionIndicatorCheckBoxRef = new NativeUI::CheckBox();
		mSelectionIndicatorCheckBoxRef->wrapContentHorizontally();
		hLayout->addChild(mSelectionIndicatorCheckBoxRef);

		mMainLayoutRef->addChild(hLayout);
	}

	/**
	 * Create and add to the screen's main layout a Button that will check
	 * if the selection indicator is shown when its clicked.
	 */
	void PropertiesScreen::createIsShownSelectionIndicationButton()
	{
		mIsSelectionIndicatorShownButtonRef = new NativeUI::Button();
		mIsSelectionIndicatorShownButtonRef->fillSpaceHorizontally();
		mIsSelectionIndicatorShownButtonRef->setText(IS_SELECTOON_INDICATOR_SHOWN_BTN_TEXT);
		mMainLayoutRef->addChild(mIsSelectionIndicatorShownButtonRef);
	}

	/**
	 * Create and add to the screen's main layout a Button that will show
	 * the selected item index.
	 */
	void PropertiesScreen::createSelectedItemIndexButton()
	{
		mSelectedItemIndexButtonRef = new NativeUI::Button();
		mSelectedItemIndexButtonRef->fillSpaceHorizontally();
		mSelectedItemIndexButtonRef->setText(SELECTED_ITEM_BUTTON_TEXT);
		mMainLayoutRef->addChild(mSelectedItemIndexButtonRef);
	}

	/**
	 * Set the CustomPicker items row height.
	 */
	void PropertiesScreen::setRowHeightButtonClicked()
	{
		MAUtil::String heightString = mRowHeightEditBoxRef->getText();
		int height = MAUtil::stringToInteger(heightString);
		mCustomPickerScreenRef.setCustomPickerRowHeight(height);
		mRowHeightEditBoxRef->hideKeyboard();
	}

	/**
	 * Set the CustomPicker items row width.
	 */
	void PropertiesScreen::setRowWidthButtonClicked()
	{
		MAUtil::String widthString = mRowWidthEditBoxRef->getText();
		int width = MAUtil::stringToInteger(widthString);
		mCustomPickerScreenRef.setCustomPickerRowWidth(width);
		mRowWidthEditBoxRef->hideKeyboard();
	}

	/**
	 * Set the selected CustomPicker item by index.
	 */
	void PropertiesScreen::selectItemButtonCliked()
	{
		MAUtil::String itemIndexString = mSelectItemEditBoxRef->getText();
		if (itemIndexString.length() > 0)
		{
			int itemIndex = MAUtil::stringToInteger(itemIndexString);
			mCustomPickerScreenRef.setSelectedItemIndex(itemIndex);
			mSelectItemEditBoxRef->hideKeyboard();
		}
	}

	/**
	 * Display an message box with the CustomPicker items row height value.
	 */
	void PropertiesScreen::getRowHeightButtonClicked()
	{
		char buffer[kBufferSize];
		int height = mCustomPickerScreenRef.getCustomPickerRowHeight();
		sprintf(buffer, "The CustomPicker items height is %d", height);
		maAlert("Height", buffer, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the CustomPicker items row width value.
	 */
	void PropertiesScreen::getRowWidthButtonClicked()
	{
		char buffer[kBufferSize];
		int width = mCustomPickerScreenRef.getCustomPickerRowWidth();
		sprintf(buffer, "The CustomPicker items width is %d", width);
		maAlert("Width", buffer, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the CustomPicker selection indicator
	 * visibility.
	 */
	void PropertiesScreen::selectionIndicatorButtonClicked()
	{
		bool isShown = mCustomPickerScreenRef.isSelectionIndicatorShown();
		const char* message = isShown ?
			"Selection indicator is shown" : "Selection indicator is hidden";
		maAlert("Selection indicator", message, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the selected CustomPicker item index.
	 */
	void PropertiesScreen::selectedItemIndexButtonClicked()
	{
		int index = mCustomPickerScreenRef.getSelectedItemIndex();
		char buffer[kBufferSize];
		sprintf(buffer, "Selected item index is %d", index);
		maAlert("Selected item index", buffer, "OK", NULL, NULL);
	}

} // end of CustomPickerTest
