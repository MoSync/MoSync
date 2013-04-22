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
#include "../Controller/Util.h"

namespace CustomPickerTest
{

	/**
	 * Constructor.
	 * @param customPickerScreen Interface to the custom picker.
	 */
	PropertiesScreen::PropertiesScreen(ICustomPickerScreen& customPickerScreen):
		mCustomPickerScreen(customPickerScreen)
	{
		mMainLayout = NULL;
		mRowHeightButton = NULL;
		mRowWidthEditBox = NULL;
		mRowWidthButton = NULL;
		mSelectItemEditBox = NULL;
		mSelectItemButton = NULL;
		mShowRowHeightButton = NULL;
		mShowRowWidthButton = NULL;
		mSelectionIndicatorCheckBox = NULL;
		mIsSelectionIndicatorShownButton = NULL;
		mSelectedItemIndexButton = NULL;

		setTitle(PROPERTIES_SCREEN_TITLE);
		createUI();

		mRowHeightButton->addButtonListener(this);
		mRowWidthButton->addButtonListener(this);
		mSelectItemButton->addButtonListener(this);
		mShowRowHeightButton->addButtonListener(this);
		mShowRowWidthButton->addButtonListener(this);
		if (isIOS())
		{
			mIsSelectionIndicatorShownButton->addButtonListener(this);
			mSelectionIndicatorCheckBox->addCheckBoxListener(this);
		}
		mSelectedItemIndexButton->addButtonListener(this);
	}

	/**
	 * Destructor.
	 */
	PropertiesScreen::~PropertiesScreen()
	{
		mRowHeightButton->removeButtonListener(this);
		mRowWidthButton->removeButtonListener(this);
		mSelectItemButton->removeButtonListener(this);
		mShowRowHeightButton->removeButtonListener(this);
		mShowRowWidthButton->removeButtonListener(this);
		mSelectedItemIndexButton->removeButtonListener(this);
		if (isIOS())
		{
			mIsSelectionIndicatorShownButton->removeButtonListener(this);
			mSelectionIndicatorCheckBox->removeCheckBoxListener(this);
		}
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void PropertiesScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mRowHeightButton)
		{
			setRowHeightButtonClicked();
		}
		else if (button == mRowWidthButton)
		{
			setRowWidthButtonClicked();
		}
		else if (button == mShowRowHeightButton)
		{
			getRowHeightButtonClicked();
		}
		else if (button == mShowRowWidthButton)
		{
			getRowWidthButtonClicked();
		}
		else if (button == mIsSelectionIndicatorShownButton)
		{
			selectionIndicatorButtonClicked();
		}
		else if (button == mSelectItemButton)
		{
			selectItemButtonCliked();
		}
		else if (button == mSelectedItemIndexButton)
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
			mCustomPickerScreen.showSelectionIndicator();
		}
		else
		{
			mCustomPickerScreen.hideSelectionIndicator();
		}
	}

	/**
	 * Create screen UI.
	 */
	void PropertiesScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		setMainWidget(mMainLayout);

		createSetRowHeight();
		createSetRowWidth();
		createSelectItem();
		createGetRowHeightButton();
		createGetRowWidthButton();
		if ( isIOS() )
		{
			createSelectionIndicatorRow();
			createIsShownSelectionIndicationButton();
		}
		createSelectedItemIndexButton();

		int rowHeight = mCustomPickerScreen.getCustomPickerRowHeight();
		int rowWidth = mCustomPickerScreen.getCustomPickerRowWidth();
		MAUtil::String rowHeightString = MAUtil::integerToString(rowHeight);
		MAUtil::String rowWidthString = MAUtil::integerToString(rowWidth);
		mRowHeightEditBox->setText(rowHeightString);
		mRowWidthEditBox->setText(rowHeightString);
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

		mRowHeightEditBox = new NativeUI::EditBox();
		mRowHeightEditBox->fillSpaceHorizontally();
		mRowHeightEditBox->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mRowHeightEditBox);

		mRowHeightButton = new NativeUI::Button();
		mRowHeightButton->fillSpaceHorizontally();
		mRowHeightButton->setText(SET_ROW_HEIGHT_BUTTON_TEXT);
		hLayout->addChild(mRowHeightButton);

		mMainLayout->addChild(hLayout);
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

		mRowWidthEditBox = new NativeUI::EditBox();
		mRowWidthEditBox->fillSpaceHorizontally();
		mRowWidthEditBox->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mRowWidthEditBox);

		mRowWidthButton = new NativeUI::Button();
		mRowWidthButton->fillSpaceHorizontally();
		mRowWidthButton->setText(SET_ROW_WIDTH_BUTTON_TEXT);
		hLayout->addChild(mRowWidthButton);

		mMainLayout->addChild(hLayout);
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

		mSelectItemEditBox = new NativeUI::EditBox();
		mSelectItemEditBox->fillSpaceHorizontally();
		mSelectItemEditBox->setInputMode(NativeUI::EDIT_BOX_INPUT_MODE_DECIMAL);
		hLayout->addChild(mSelectItemEditBox);

		mSelectItemButton = new NativeUI::Button();
		mSelectItemButton->fillSpaceHorizontally();
		mSelectItemButton->setText(SELECT_ITEM_BUTTON_TEXT);
		hLayout->addChild(mSelectItemButton);

		mMainLayout->addChild(hLayout);
	}

	/**
	 * Create and add an NativeUI Button that will show the CustomPicker
	 * items row height when clicked.
	 */
	void PropertiesScreen::createGetRowHeightButton()
	{
		mShowRowHeightButton = new NativeUI::Button();
		mShowRowHeightButton->fillSpaceHorizontally();
		mShowRowHeightButton->setText(GET_ROW_HEIGHT_BUTTON_TEXT);
		mMainLayout->addChild(mShowRowHeightButton);
	}

	/**
	 * Create and add an NativeUI Button that will show the CustomPicker
	 * items row width when clicked.
	 */
	void PropertiesScreen::createGetRowWidthButton()
	{
		mShowRowWidthButton = new NativeUI::Button();
		mShowRowWidthButton->fillSpaceHorizontally();
		mShowRowWidthButton->setText(GET_ROW_WIDTH_BUTTON_TEXT);
		mMainLayout->addChild(mShowRowWidthButton);
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

		mSelectionIndicatorCheckBox = new NativeUI::CheckBox();
		mSelectionIndicatorCheckBox->wrapContentHorizontally();
		hLayout->addChild(mSelectionIndicatorCheckBox);

		mMainLayout->addChild(hLayout);
	}

	/**
	 * Create and add to the screen's main layout a Button that will check
	 * if the selection indicator is shown when its clicked.
	 */
	void PropertiesScreen::createIsShownSelectionIndicationButton()
	{
		mIsSelectionIndicatorShownButton = new NativeUI::Button();
		mIsSelectionIndicatorShownButton->fillSpaceHorizontally();
		mIsSelectionIndicatorShownButton->setText(IS_SELECTOON_INDICATOR_SHOWN_BTN_TEXT);
		mMainLayout->addChild(mIsSelectionIndicatorShownButton);
	}

	/**
	 * Create and add to the screen's main layout a Button that will show
	 * the selected item index.
	 */
	void PropertiesScreen::createSelectedItemIndexButton()
	{
		mSelectedItemIndexButton = new NativeUI::Button();
		mSelectedItemIndexButton->fillSpaceHorizontally();
		mSelectedItemIndexButton->setText(SELECTED_ITEM_BUTTON_TEXT);
		mMainLayout->addChild(mSelectedItemIndexButton);
	}

	/**
	 * Set the CustomPicker items row height.
	 */
	void PropertiesScreen::setRowHeightButtonClicked()
	{
		MAUtil::String heightString = mRowHeightEditBox->getText();
		int height = MAUtil::stringToInteger(heightString);
		mCustomPickerScreen.setCustomPickerRowHeight(height);
		mRowHeightEditBox->hideKeyboard();
	}

	/**
	 * Set the CustomPicker items row width.
	 */
	void PropertiesScreen::setRowWidthButtonClicked()
	{
		MAUtil::String widthString = mRowWidthEditBox->getText();
		int width = MAUtil::stringToInteger(widthString);
		mCustomPickerScreen.setCustomPickerRowWidth(width);
		mRowWidthEditBox->hideKeyboard();
	}

	/**
	 * Set the selected CustomPicker item by index.
	 */
	void PropertiesScreen::selectItemButtonCliked()
	{
		MAUtil::String itemIndexString = mSelectItemEditBox->getText();
		if (itemIndexString.length() > 0)
		{
			int itemIndex = MAUtil::stringToInteger(itemIndexString);
			mCustomPickerScreen.setSelectedItemIndex(itemIndex);
			mSelectItemEditBox->hideKeyboard();
		}
	}

	/**
	 * Display an message box with the CustomPicker items row height value.
	 */
	void PropertiesScreen::getRowHeightButtonClicked()
	{
		char buffer[kBufferSize];
		int height = mCustomPickerScreen.getCustomPickerRowHeight();
		sprintf(buffer, "The CustomPicker items height is %d", height);
		maAlert("Height", buffer, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the CustomPicker items row width value.
	 */
	void PropertiesScreen::getRowWidthButtonClicked()
	{
		char buffer[kBufferSize];
		int width = mCustomPickerScreen.getCustomPickerRowWidth();
		sprintf(buffer, "The CustomPicker items width is %d", width);
		maAlert("Width", buffer, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the CustomPicker selection indicator
	 * visibility.
	 */
	void PropertiesScreen::selectionIndicatorButtonClicked()
	{
		bool isShown = mCustomPickerScreen.isSelectionIndicatorShown();
		const char* message = isShown ?
			"Selection indicator is shown" : "Selection indicator is hidden";
		maAlert("Selection indicator", message, "OK", NULL, NULL);
	}

	/**
	 * Display an message box with the selected CustomPicker item index.
	 */
	void PropertiesScreen::selectedItemIndexButtonClicked()
	{
		int index = mCustomPickerScreen.getSelectedItemIndex();
		char buffer[kBufferSize];
		sprintf(buffer, "Selected item index is %d", index);
		maAlert("Selected item index", buffer, "OK", NULL, NULL);
	}

} // end of CustomPickerTest
