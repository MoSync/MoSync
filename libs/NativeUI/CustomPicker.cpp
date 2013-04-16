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

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	CustomPicker::CustomPicker(): Widget(MAW_CUSTOM_PICKER)
	{
	}

	/**
	 * Destructor.
	 */
	CustomPicker::~CustomPicker()
	{
		mCustomPickerListeners.clear();
	}

	/**
	 * Set the custom picker items row height.
	 * All its items should have the same height.
	 * Calling this method will reload all its items.
	 * Default value it's the height of the widget.
	 * Platform: iOS.
	 * @param height Height in pixels.
	 */
	void CustomPicker::setRowHeight(const int height)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_ROW_HEIGHT, height);
	}

	/**
	 * Get the custom picker items row height.
	 * All its items have the same height.
	 * Default value it's the height of the widget.
	 * Platform: iOS.
	 * @return height in pixels.
	 */
	int CustomPicker::getRowHeight()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_ROW_HEIGHT);
	}

	/**
	 * Set the custom picker items row width.
	 * All its items should have the same width.
	 * Calling this method will reload all its items.
	 * Default value it's the width of the widget.
	 * Platform: iOS.
	 * @param width Width in pixels.
	 */
	void CustomPicker::setRowWidth(const int width)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_ROW_WIDTH, width);
	}

	/**
	 * Get the custom picker items row width.
	 * All its items have the same width.
	 * Default value it's the width of the widget.
	 * Platform: iOS.
	 * @return width in pixels.
	 */
	int CustomPicker::getRowWidth()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_ROW_WIDTH);
	}

	/**
	 * Reload all its items.
	 * Platform: iOS.
	 */
	void CustomPicker::realoadData()
	{
		setProperty(MAW_CUSTOM_PICKER_RELOAD_DATA, "");
	}

	/**
	 * Show the selection indicator.
	 * By default, the selection indicator is hidden.
	 * Platform: iOS.
	 */
	void CustomPicker::showSelectionIndicator()
	{
		setProperty(MAW_CUSTOM_PICKER_SELECTION_INDICATOR, "true");
	}

	/**
	 * Hide the selection indicator.
	 * By default, the selection indicator is hidden.
	 * Platform: iOS.
	 */
	void CustomPicker::hideSelectionIndicator()
	{
		setProperty(MAW_CUSTOM_PICKER_SELECTION_INDICATOR, "false");
	}

	/**
	 * Check if the selection indicator is shown.
	 * By default, the selection indicator is hidden.
	 * @return true if it's shown, false otherwise.
	 */
	bool CustomPicker::isSelectionIndicatorShown()
	{
		MAUtil::String value =
			getPropertyString(MAW_CUSTOM_PICKER_SELECTION_INDICATOR);
		return (strcmp(value.c_str(), "true") == 0) ? true : false;
	}

	/**
	 * Add an custom picker event listener.
	 * @param listener The listener that will receive custom picker events.
	 */
	void CustomPicker::addCustomPickerListener(CustomPickerListener* listener)
	{
		addListenerToVector(mCustomPickerListeners, listener);
	}

	/**
	 * Remove the custom picker listener.
	 * @param listener The listener that receives custom picker events.
	 */
	void CustomPicker::removeListViewListener(CustomPickerListener* listener)
	{
		removeListenerFromVector(mCustomPickerListeners, listener);
	}

	/**
	 * This method is called when there is an event for this widget.
	 * It passes on the event to all widget's listeners.
	 * @param widgetEventData The data for the widget event.
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
	 * @param selectedItemIndex Index of the selected item.
	 */
	void CustomPicker::notifyListenersItemChanged(const int selectedItemIndex)
	{
		Widget* selectedItem = getChild(selectedItemIndex);
		for (int i = 0; i < mCustomPickerListeners.size(); i++)
		{
			mCustomPickerListeners[i]->customPickerItemChanged(
				this,
				selectedItem,
				selectedItemIndex);
		}
	}

} // namespace NativeUI
