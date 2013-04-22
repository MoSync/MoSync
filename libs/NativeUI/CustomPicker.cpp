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
	 * Add a layout as a child of this widget.
	 * Platform: Android, iOS.
	 * @param layout The layout that will be added.
	 * The ownership of the widget is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted.
	 * On iOS the layout must have fixed sizes.
	 */
	void CustomPicker::addChild(Layout* layout)
	{
		Widget::addChild(layout);
	}

	/**
	 * Insert a layout as a child of this widget, at a given position.
	 * Platform: Android, iOS.
	 * @param layout The layout to be added.
	 * Its ownership is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted.
	 * @param index The position on which to add the child.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int CustomPicker::insertChild(Layout* layout, const int index)
	{
		return Widget::insertChild(layout, index);
	}

	/**
	 * Remove a child layout from its parent (but does not destroy it).
	 * Removing a currently visible top-level widget causes the MoSync view
	 * to become visible.
	 * When the parent widget will be destroyed, the child widget will not
	 * be deleted.
	 * Platform: Android, iOS.
	 * @param layout The layout to be removed.
	 */
	void CustomPicker::removeChild(Layout* layout)
	{
		Widget::removeChild(layout);
	}

	/**
	 * Set the custom picker items row height.
	 * All its items should have the same height.
	 * Calling this method will reload all its items.
	 * Default value it's the height of the widget.
	 * Platform: iOS and Android.
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
	 * Platform: iOS and Android.
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
	 * Platform: iOS and Android.
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
	 * Platform: iOS and Android.
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
	void CustomPicker::reloadData()
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
	 * @param index Index of the item. First child has index zero.
	 */
	void CustomPicker::setSelectedItemIndex(const int index)
	{
		setPropertyInt(MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX, index);
	}

	/**
	 * Get the index of the selected item.
	 * @return A zero-indexed number identifying the selected item.
	 * If the widget has no children, #MAW_RES_ERROR will be returned.
	 * If there is nothing selected, -1 will be returned.
	 */
	int CustomPicker::getSelectedItemIndex()
	{
		return getPropertyInt(MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX);
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
	void CustomPicker::removeCustomPickerListener(CustomPickerListener* listener)
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

	/**
	 * Add a widget as a child of this widget.
	 * Platform: Android, iOS.
	 * @param widget The widget that will be added.
	 * The ownership of the widget is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted.
	 * Must be a layout widget.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
	 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
	 * - #MAW_RES_CANNOT_INSERT_DIALOG if the child is a dialog.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int CustomPicker::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

	/**
	 * Insert a widget as a child of this widget, at a given position.
	 * Platform: Android, iOS.
	 * @param widget The widget to be added.
	 * The ownership of the widget is passed to this function.
	 * When the parent widget will be destroyed, all child widgets
	 * will be deleted. Must be a layout.
	 * @param index The position on which to add the child.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be added to the parent.
	 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
	 * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
	 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
	 * - #MAW_RES_CANNOT_INSERT_DIALOG if the child is a dialog.
	 * - #MAW_RES_ERROR if it could not be added for some other reason.
	 */
	int CustomPicker::insertChild(Widget* widget, const int index)
	{
		return Widget::insertChild(widget, index);
	}

	/**
	 * Remove a child widget from its parent (but does not destroy it).
	 * Removing a currently visible top-level widget causes the MoSync view
	 * to become visible.
	 * When the parent widget will be destroyed, the child widget will not
	 * be deleted.
	 * Platform: Android, iOS.
	 * @param widget The widget to be removed.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the child could be removed from the parent.
	 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int CustomPicker::removeChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

} // namespace NativeUI
