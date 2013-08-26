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
