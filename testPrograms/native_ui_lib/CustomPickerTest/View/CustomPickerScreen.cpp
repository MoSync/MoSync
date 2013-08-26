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
 * @file CustomPickerScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen that shows a CustomPicker widget.
 */

#define CUSTOM_PICKER_SCREEN_TITLE "CustomPicker"

// How many item to add to tested CustomPicker when app starts
#define DEFAULT_CUSTOM_PICKER_ITEMS 5

#define DEFAULT_ROW_HEIGHT 100
#define CUSTOM_PICKER_BORDER_WIDTH 50

#define ADD_ITEM_BUTTON_TITLE "Add item"
#define REMOVE_ITEM_BUTTON_TITLE "Remove selected item"

#include <conprint.h>
#include <MAUtil/Environment.h>

#include <NativeUI/Button.h>
#include <NativeUI/CustomPicker.h>
#include <NativeUI/Label.h>
#include <NativeUI/Layout.h>
#include <NativeUI/VerticalLayout.h>

#include "CustomPickerScreen.h"
#include "ViewUtils.h"

namespace CustomPickerTest
{
	/**
	 * Constructor.
	 */
	CustomPickerScreen::CustomPickerScreen():
		mMainLayoutRef(NULL),
		mCustomPickerRef(NULL),
		mAddItemButtonRef(NULL),
		mRemoveItemButtonRef(NULL),
		mSelectedItemIndexLabelRef(NULL)
	{
		setTitle(CUSTOM_PICKER_SCREEN_TITLE);
		this->createUI();

		mCustomPickerRef->setRowHeight(DEFAULT_ROW_HEIGHT);

		// For iOS we need to remove border size from rowWidth
		MAUtil::Environment& environment(MAUtil::Environment::getEnvironment());
		if (environment.getCurrentPlatform() == MAUtil::OS_IOS)
		{
			int rowWidth =
				mCustomPickerRef->getRowWidth() - CUSTOM_PICKER_BORDER_WIDTH;
			mCustomPickerRef->setRowWidth(rowWidth);
		}

		populateCustomPicker();

		mAddItemButtonRef->addButtonListener(this);
		mRemoveItemButtonRef->addButtonListener(this);

		mCustomPickerRef->addCustomPickerListener(this);
	}

	/**
	 * Destructor.
	 */
	CustomPickerScreen::~CustomPickerScreen()
	{
		mAddItemButtonRef->removeButtonListener(this);
		mRemoveItemButtonRef->removeButtonListener(this);

		mCustomPickerRef->removeCustomPickerListener(this);
	}

	/**
	 * Add an item to the tested CustomPicker widget.
	 * From ICustomPickerScreen.
	 * @param layout Item to add.
	 */
	void CustomPickerScreen::addCustomPickerItem(NativeUI::Layout* layout)
	{
		mCustomPickerRef->addChild(layout);
	}

	/**
	 * Remove an item from the tested CustomPicker widget.
	 * From ICustomPickerScreen.
	 * @param layout Item to remove.
	 */
	void CustomPickerScreen::removeCustomPickerItem(NativeUI::Layout* layout)
	{
		mCustomPickerRef->removeChild(layout);
	}

	/**
	 * Set the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @param height Value to set.
	 */
	void CustomPickerScreen::setCustomPickerRowHeight(const int height)
	{
		int items = mCustomPickerRef->countChildWidgets();
		for (int index = 0; index < items; index++)
		{
			NativeUI::Widget* child = mCustomPickerRef->getChild(index);
			child->setHeight(height);
		}
		mCustomPickerRef->setRowHeight(height);
	}

	/**
	 * Set the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @param width Value to set.
	 */
	void CustomPickerScreen::setCustomPickerRowWidth(const int width)
	{
		int items = mCustomPickerRef->countChildWidgets();
		for (int index = 0; index < items; index++)
		{
			NativeUI::Widget* child = mCustomPickerRef->getChild(index);
			child->setWidth(width);
		}
		mCustomPickerRef->setRowWidth(width);
	}

	/**
	 * Get the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @return Items height.
	 */
	int CustomPickerScreen::getCustomPickerRowHeight()
	{
		return mCustomPickerRef->getRowHeight();
	}

	/**
	 * Get the width of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @return Items width.
	 */
	int CustomPickerScreen::getCustomPickerRowWidth()
	{
		return mCustomPickerRef->getRowWidth();
	}

	/**
	 * Show the CustomPicker selection indicator.
	 * From ICustomPickerScreen.
	 */
	void CustomPickerScreen::showSelectionIndicator()
	{
		mCustomPickerRef->showSelectionIndicator();
	}

	/**
	 * Hide the CustomPicker selection indicator.
	 * From ICustomPickerScreen.
	 */
	void CustomPickerScreen::hideSelectionIndicator()
	{
		mCustomPickerRef->hideSelectionIndicator();
	}

	/**
	 * Check if CustomPicker selection indicator is shown.
	 * @return true if shown, false otherwise.
	 */
	bool CustomPickerScreen::isSelectionIndicatorShown()
	{
		return mCustomPickerRef->isSelectionIndicatorShown();
	}

	/**
	 * Set the selected CustomPicker item by index.
	 * From ICustomPickerScreen.
	 * @param index Item index to select(zero indexed number).
	 */
	void CustomPickerScreen::setSelectedItemIndex(const int index)
	{
		mCustomPickerRef->setSelectedItemIndex(index);
	}

	/**
	 * Get the selected CustomPickerItem index.
	 * From ICustomPickerScreen.
	 * @return Selected item index(zero indexed number), or #MAW_RES_ERROR
	 * if the widget has no children.
	 */
	int CustomPickerScreen::getSelectedItemIndex()
	{
		return mCustomPickerRef->getSelectedItemIndex();
	}

	/**
	 * This method is called when user selects a new item.
	 * @param customPicker The object that generated the event.
	 * @param selectedItem The selected object.
	 * @param selectedItemIndex The index of the selected object.
	 */
	void CustomPickerScreen::customPickerItemChanged(
		NativeUI::CustomPicker* customPicker,
		NativeUI::Widget* selectedItem,
		const int selectedItemIndex)
	{
		char buffer[kBufferSize];
		sprintf(buffer, "Selected item index: %d", selectedItemIndex);
		mSelectedItemIndexLabelRef->setText(buffer);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void CustomPickerScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mAddItemButtonRef)
		{
			NativeUI::Layout* item = createCustomPickerItem();
			addCustomPickerItem(item);
		}
		else if (button == mRemoveItemButtonRef)
		{
			int index = mCustomPickerRef->getSelectedItemIndex();
			if (index >= 0)
			{
				NativeUI::Layout* child = (NativeUI::Layout*) mCustomPickerRef->getChild(index);
				removeCustomPickerItem(child);
				delete child;
				child = NULL;
			}
		}
	}

	/**
	 * Create screen UI.
	 */
	void CustomPickerScreen::createUI()
	{
		mMainLayoutRef = new NativeUI::VerticalLayout();
		setMainWidget(mMainLayoutRef);

		mCustomPickerRef = new NativeUI::CustomPicker();
		mMainLayoutRef->addChild(mCustomPickerRef);

		mSelectedItemIndexLabelRef = new NativeUI::Label();
		mSelectedItemIndexLabelRef->fillSpaceHorizontally();
		mSelectedItemIndexLabelRef->wrapContentVertically();
		mSelectedItemIndexLabelRef->setText("Selected index:");
		mMainLayoutRef->addChild(mSelectedItemIndexLabelRef);

		mAddItemButtonRef = new NativeUI::Button();
		mAddItemButtonRef->fillSpaceHorizontally();
		mAddItemButtonRef->setText(ADD_ITEM_BUTTON_TITLE);
		mMainLayoutRef->addChild(mAddItemButtonRef);

		mRemoveItemButtonRef = new NativeUI::Button();
		mRemoveItemButtonRef->fillSpaceHorizontally();
		mRemoveItemButtonRef->setText(REMOVE_ITEM_BUTTON_TITLE);
		mMainLayoutRef->addChild(mRemoveItemButtonRef);
	}

	/**
	 * Add items to tested CustomPicker.
	 */
	void CustomPickerScreen::populateCustomPicker()
	{
		for (int i = 0 ; i < DEFAULT_CUSTOM_PICKER_ITEMS; i++)
		{
			NativeUI::Layout* item = createCustomPickerItem();
			addCustomPickerItem(item);
		}
	}

	/**
	 * Create an CustomPicker item.
	 * @return The created item. Its ownership is passed to the caller.
	 */
	NativeUI::Layout* CustomPickerScreen::createCustomPickerItem()
	{
		NativeUI::VerticalLayout* item = new NativeUI::VerticalLayout();
		item->setBackgroundColor(getRandomColor());
		item->setWidth(mCustomPickerRef->getRowWidth());
		item->setHeight(mCustomPickerRef->getRowHeight());
		NativeUI::Label* x = new NativeUI::Label(" Custom Picker row ");
		item->addChild(x);
		return item;
	}

} // end of CustomPickerTest
