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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file CustomPicker.h
 * @author Bogdan Iusco
 *
 * \brief A widget that provides a quick way to select one value from a set.
 * Platform: Android, iOS.
*/

#ifndef NATIVEUI_CUSTOM_PICKER_H_
#define NATIVEUI_CUSTOM_PICKER_H_

#include "Widget.h"

namespace NativeUI
{

	// Forward declarations.
	class CustomPickerListener;
	class Layout;

	/**
	 * \brief A widget that provides a quick way to select one value from a set.
	 * Platform: Android, iOS.
	 */
	class CustomPicker : public Widget
	{
	public:
		CustomPicker();

		virtual ~CustomPicker();

		/**
		 * Add a layout as a child of this widget.
		 * Platform: Android, iOS.
		 * @param layout The layout that will be added.
		 * The ownership of the widget is passed to this function.
		 * When the parent widget will be destroyed, all child widgets
		 * will be deleted.
		 * On iOS the layout must have fixed sizes.
		 */
		void addChild(Layout* layout);

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
		int insertChild(Layout* layout, const int index);

		/**
		 * Remove a child layout from its parent (but does not destroy it).
		 * Removing a currently visible top-level widget causes the MoSync view
		 * to become visible.
		 * When the parent widget will be destroyed, the child widget will not
		 * be deleted.
		 * Platform: Android, iOS.
		 * @param layout The layout to be removed.
		 */
		void removeChild(Layout* layout);

		/**
		 * Set the custom picker items row height.
		 * All its items should have the same height.
		 * Calling this method will reload all its items.
		 * Default value it's the height of the widget.
		 * Platform: iOS and Android.
		 * @param height Height in pixels.
		 */
		void setRowHeight(const int height);

		/**
		 * Get the custom picker items row height.
		 * All its items have the same height.
		 * Default value it's the height of the widget.
		 * Platform: iOS and Android.
		 * @return height in pixels.
		 */
		int getRowHeight();

		/**
		 * Set the custom picker items row width.
		 * All its items should have the same width.
		 * Calling this method will reload all its items.
		 * Default value it's the width of the widget.
		 * Platform: iOS and Android.
		 * @param width Width in pixels.
		 */
		void setRowWidth(const int width);

		/**
		 * Get the custom picker items row width.
		 * All its items have the same width.
		 * Default value it's the width of the widget.
		 * Platform: iOS and Android.
		 * @return width in pixels.
		 */
		int getRowWidth();

		/**
		 * Reload all its items.
		 * Platform: iOS.
		 */
		void reloadData();

		/**
		 * Show the selection indicator.
		 * By default, the selection indicator is hidden.
		 * Platform: iOS.
		 */
		void showSelectionIndicator();

		/**
		 * Hide the selection indicator.
		 * By default, the selection indicator is hidden.
		 * Platform: iOS.
		 */
		void hideSelectionIndicator();

		/**
		 * Check if the selection indicator is shown.
		 * By default, the selection indicator is hidden.
		 * @return true if it's shown, false otherwise.
		 * Platform: iOS.
		 */
		bool isSelectionIndicatorShown();

		/**
		 * Select an item by index.
		 * @param index Index of the item. First child has index zero.
		 */
		void setSelectedItemIndex(const int index);

		/**
		 * Get the index of the selected item.
		 * @return A zero-indexed number identifying the selected item.
		 * If the widget has no children, #MAW_RES_ERROR will be returned.
		 * If there is nothing selected, -1 will be returned.
		 */
		int getSelectedItemIndex();

		/**
		 * Add an custom picker event listener.
		 * @param listener The listener that will receive custom picker events.
		 */
		void addCustomPickerListener(CustomPickerListener* listener);

		/**
		 * Remove the custom picker listener.
		 * @param listener The listener that receives custom picker events.
		 */
		void removeCustomPickerListener(CustomPickerListener* listener);

	protected:
		/**
		 * This method is called when there is an event for this widget.
		 * It passes on the event to all widget's listeners.
		 * @param widgetEventData The data for the widget event.
		 */
		virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

	private:
		/**
		 * Notify listeners that user selected a new item.
		 * @param selectedItemIndex Index of the selected item.
		 */
		void notifyListenersItemChanged(const int selectedItemIndex);

		/**
		 * Forwards call to Widget::addChild().
		 * Made private in order to force adding only layouts as children.
		 */
		virtual int addChild(Widget* widget);

		/**
		 * Forwards call to Widget::insertChild().
		 * Made private in order to force adding only layouts as children.
		 */
		virtual int insertChild(Widget* widget, const int index);

		/**
		 * Forwards call to Widget::addChild().
		 * Made private in order to force adding only layouts as children.
		 */
		virtual int removeChild(Widget* widget);

	private:
		MAUtil::Vector<CustomPickerListener*> mCustomPickerListeners;
	};

} // namespace NativeUI

#endif /* NATIVEUI_CUSTOM_PICKER_H_ */

/*! @} */
