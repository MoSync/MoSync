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
 * \brief A spinning-wheel widget used to shown one or more items(layout widgets).
 */

#ifndef NATIVEUI_CUSTOM_PICKER_H_
#define NATIVEUI_CUSTOM_PICKER_H_

#include "Widget.h"

namespace NativeUI
{
	// Forward declaration.
	class CustomPickerListener;

	/**
	 * \brief A spinning-wheel widget used to shown one or more items(layout widgets).
	 */
	class CustomPicker : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		CustomPicker();

		/**
		 * Destructor.
		 */
		virtual ~CustomPicker();

		/**
		 * Set the custom picker items row height.
		 * All its items should have the same height.
		 * Calling this method will reload all its items.
		 * Default value it's the height of the widget.
		 * Platform: iOS.
		 * @param height Height in pixels.
		 */
		void setRowHeight(const int height);

		/**
		 * Get the custom picker items row height.
		 * All its items have the same height.
		 * Default value it's the height of the widget.
		 * Platform: iOS.
		 * @return height in pixels.
		 */
		int getRowHeight();

		/**
		 * Set the custom picker items row width.
		 * All its items should have the same width.
		 * Calling this method will reload all its items.
		 * Default value it's the width of the widget.
		 * Platform: iOS.
		 * @param width Width in pixels.
		 */
		void setRowWidth(const int width);

		/**
		 * Get the custom picker items row width.
		 * All its items have the same width.
		 * Default value it's the width of the widget.
		 * Platform: iOS.
		 * @return width in pixels.
		 */
		int getRowWidth();

		/**
		 * Reload all its items.
		 * Platform: iOS.
		 */
		void realoadData();

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
		 */
		bool isSelectionIndicatorShown();

		/**
		 * Add an custom picker event listener.
		 * @param listener The listener that will receive custom picker events.
		 */
		void addCustomPickerListener(CustomPickerListener* listener);

		/**
		 * Remove the custom picker listener.
		 * @param listener The listener that receives custom picker events.
		 */
		void removeListViewListener(CustomPickerListener* listener);

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

	private:
		/**
		 * Array with custom picker listeners.
		 */
		MAUtil::Vector<CustomPickerListener*> mCustomPickerListeners;
	};

} // namespace NativeUI

#endif /* NATIVEUI_CUSTOM_PICKER_H_ */

/*! @} */
