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
 * @file CustomPicker.cpp
 * @author Bogdan Iusco
 *
 * \brief A spinning-wheel widget used to shown one or more items(layout widgets).
 */

#include "CustomPicker.h"
#include "CustomPickerListener.h"
#include "Layout.h"

namespace NativeUI
{
	CustomPicker::CustomPicker(): Widget(MAW_CUSTOM_PICKER)
	{
	}

	CustomPicker::~CustomPicker()
	 {
		mCustomPickerListeners.clear();
	 }

	void CustomPicker::addChild(Layout* layout)
	{
		Widget::addChild(layout);
	}

	int CustomPicker::insertChild(Layout* layout, const int index)
	{
		return Widget::insertChild(layout, index);
	}

	void CustomPicker::removeChild(Layout* layout)
	{
		Widget::removeChild(layout);
	}

	/**
	 * Set the custom picker items row height.
	 * @param height Height in pixels.
	 */
	void CustomPicker::setRowHeight(const int height)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_ROW_HEIGHT, height);
	}

	/**
	 * Get the custom picker items row height.
	 * @return height in pixels.
	 */
	int CustomPicker::getRowHeight()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_ROW_HEIGHT);
	}

	/**
	 * Set the custom picker items row width.
	 * @param width Width in pixels.
	 */
	void CustomPicker::setRowWidth(const int width)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_ROW_WIDTH, width);
	}

	/**
	 * Get the custom picker items row width.
	 * @return width in pixels.
	 */
	int CustomPicker::getRowWidth()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_ROW_WIDTH);
	}

	/**
	 * Reload all its items.
	 */
	void CustomPicker::reloadData()
	{
		setProperty(MAW_CUSTOM_PICKER_RELOAD_DATA, "");
	}

	/**
	 * Show the selection indicator.
	 */
	void CustomPicker::showSelectionIndicator()
	{
		setProperty(MAW_CUSTOM_PICKER_SELECTION_INDICATOR, "true");
	}

	/**
	 * Hide the selection indicator.
	 */
	void CustomPicker::hideSelectionIndicator()
	{
		setProperty(MAW_CUSTOM_PICKER_SELECTION_INDICATOR, "false");
	}

	/**
	 * Check if the selection indicator is shown.
	 * Platform: iOS.
	 */
	bool CustomPicker::isSelectionIndicatorShown()
	{
		MAUtil::String value =
			getPropertyString(MAW_CUSTOM_PICKER_SELECTION_INDICATOR);
		return (strcmp(value.c_str(), "true") == 0) ? true : false;
	}

	/**
	 * Select an item by index.
	 */
	void CustomPicker::setSelectedItemIndex(const int index)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX, index);
	}

	/**
	 * Get the index of the selected item.
	 * If there is nothing selected, -1 will be returned.
	 */
	int CustomPicker::getSelectedItemIndex()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX);
	}

	/**
	 * Add an custom picker event listener.
	 */
	void CustomPicker::addCustomPickerListener(CustomPickerListener* listener)
	{
		addListenerToVector(mCustomPickerListeners, listener);
	}

	/**
	 * Remove the custom picker listener.
	 */
	void CustomPicker::removeCustomPickerListener(CustomPickerListener* listener)
	{
		removeListenerFromVector(mCustomPickerListeners, listener);
	}

	/**
	 * This method is called when there is an event for this widget.
	 */
	void CustomPicker::handleWidgetEvent(MAWidgetEventData* widgetEventData)
	{
		Widget::handleWidgetEvent(widgetEventData);

		if (widgetEventData->eventType == MAW_EVENT_CUSTOM_PICKER_ITEM_SELECTED)
		{
			notifyListenersItemChanged(widgetEventData->customPickerItemIndex);
		}
	}

	/**
	 * Notify listeners that user selected a new item.
	 */
	void CustomPicker::notifyListenersItemChanged(const int selectedItemIndex)
	{
		Widget* selectedItem = getChild(selectedItemIndex);
		if (!selectedItem)
		{
			return;
		}

		for (int i = 0; i < mCustomPickerListeners.size(); i++)
		{
			mCustomPickerListeners[i]->customPickerItemChanged(
				this,
				selectedItem,
				selectedItemIndex);
		}
	}

	int CustomPicker::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

	int CustomPicker::insertChild(Widget* widget, const int index)
	{
		return Widget::insertChild(widget, index);
	}

	int CustomPicker::removeChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

} // namespace NativeUI
