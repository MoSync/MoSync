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
 * @file CustomPickerScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen that shows a CustomPicker widget.
 */

#define CUSTOM_PICKER_SCREEN_TITLE "CustomPicker"

// How many item to add to tested CustomPicker when app starts
#define DEFAULT_CUSTOM_PICKER_ITEMS 5

#define DEFAULT_ROW_HEIGHT 100

#define ADD_ITEM_BUTTON_TITLE "Add item"
#define REMOVE_ITEM_BUTTON_TITLE "Remove selected item"

#include <conprint.h>

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
		mMainLayout(NULL),
		mCustomPicker(NULL),
		mAddItemButton(NULL),
		mRemoveItemButton(NULL),
		mSelectedItemIndexLabel(NULL)
	{
		setTitle(CUSTOM_PICKER_SCREEN_TITLE);
		this->createUI();

		mCustomPicker->setRowHeight(DEFAULT_ROW_HEIGHT);
		populateCustomPicker();

		mAddItemButton->addButtonListener(this);
		mRemoveItemButton->addButtonListener(this);

		mCustomPicker->addCustomPickerListener(this);
	}

	/**
	 * Destructor.
	 */
	CustomPickerScreen::~CustomPickerScreen()
	{
		mAddItemButton->removeButtonListener(this);
		mRemoveItemButton->removeButtonListener(this);

		mCustomPicker->removeCustomPickerListener(this);
	}

	/**
	 * Add an item to the tested CustomPicker widget.
	 * From ICustomPickerScreen.
	 * @param layout Item to add.
	 */
	void CustomPickerScreen::addCustomPickerItem(NativeUI::Layout* layout)
	{
		mCustomPicker->addChild(layout);
	}

	/**
	 * Remove an item from the tested CustomPicker widget.
	 * From ICustomPickerScreen.
	 * @param layout Item to remove.
	 */
	void CustomPickerScreen::removeCustomPickerItem(NativeUI::Layout* layout)
	{
		mCustomPicker->removeChild(layout);
	}

	/**
	 * Set the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @param height Value to set.
	 */
	void CustomPickerScreen::setCustomPickerRowHeight(const int height)
	{
		mCustomPicker->setRowHeight(height);
	}

	/**
	 * Set the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @param width Value to set.
	 */
	void CustomPickerScreen::setCustomPickerRowWidth(const int width)
	{
		mCustomPicker->setRowWidth(width);
	}

	/**
	 * Get the height of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @return Items height.
	 */
	int CustomPickerScreen::getCustomPickerRowHeight()
	{
		return mCustomPicker->getRowHeight();
	}

	/**
	 * Get the width of the CustomPicker items.
	 * From ICustomPickerScreen.
	 * @return Items width.
	 */
	int CustomPickerScreen::getCustomPickerRowWidth()
	{
		return mCustomPicker->getRowWidth();
	}

	/**
	 * Show the CustomPicker selection indicator.
	 * From ICustomPickerScreen.
	 */
	void CustomPickerScreen::showSelectionIndicator()
	{
		mCustomPicker->showSelectionIndicator();
	}

	/**
	 * Hide the CustomPicker selection indicator.
	 * From ICustomPickerScreen.
	 */
	void CustomPickerScreen::hideSelectionIndicator()
	{
		mCustomPicker->hideSelectionIndicator();
	}

	/**
	 * Check if CustomPicker selection indicator is shown.
	 * @return true if shown, false otherwise.
	 */
	bool CustomPickerScreen::isSelectionIndicatorShown()
	{
		return mCustomPicker->isSelectionIndicatorShown();
	}

	/**
	 * Set the selected CustomPicker item by index.
	 * From ICustomPickerScreen.
	 * @param index Item index to select(zero indexed number).
	 */
	void CustomPickerScreen::setSelectedItemIndex(const int index)
	{
		mCustomPicker->setSelectedItemIndex(index);
	}

	/**
	 * Get the selected CustomPickerItem index.
	 * From ICustomPickerScreen.
	 * @return Selected item index(zero indexed number), or #MAW_RES_ERROR
	 * if the widget has no children.
	 */
	int CustomPickerScreen::getSelectedItemIndex()
	{
		return mCustomPicker->getSelectedItemIndex();
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
		mSelectedItemIndexLabel->setText(buffer);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void CustomPickerScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mAddItemButton)
		{
			NativeUI::Layout* item = createCustomPickerItem();
			addCustomPickerItem(item);
		}
		else if (button == mRemoveItemButton)
		{
			int index = mCustomPicker->getSelectedItemIndex();
			if (index >= 0)
			{
				NativeUI::Layout* child = (NativeUI::Layout*) mCustomPicker->getChild(index);
				mCustomPicker->removeChild(child);
				mCustomPicker->reloadData();
			}
		}
	}

	/**
	 * Create screen UI.
	 */
	void CustomPickerScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		setMainWidget(mMainLayout);

		mCustomPicker = new NativeUI::CustomPicker();
		mMainLayout->addChild(mCustomPicker);

		mSelectedItemIndexLabel = new NativeUI::Label();
		mSelectedItemIndexLabel->fillSpaceHorizontally();
		mSelectedItemIndexLabel->wrapContentVertically();
		mSelectedItemIndexLabel->setText("Selected index:");
		mMainLayout->addChild(mSelectedItemIndexLabel);

		mAddItemButton = new NativeUI::Button();
		mAddItemButton->fillSpaceHorizontally();
		mAddItemButton->setText(ADD_ITEM_BUTTON_TITLE);
		mMainLayout->addChild(mAddItemButton);

		mRemoveItemButton = new NativeUI::Button();
		mRemoveItemButton->fillSpaceHorizontally();
		mRemoveItemButton->setText(REMOVE_ITEM_BUTTON_TITLE);
		mMainLayout->addChild(mRemoveItemButton);
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
		item->setWidth(mCustomPicker->getRowWidth());
		item->setHeight(mCustomPicker->getRowHeight());
		return item;
	}

} // end of CustomPickerTest
