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

	// Forward declaration.
	class Layout;

	/**
	 * \brief A widget that provides a quick way to select one value from a set.
	 * Platform: Android, iOS.
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

	private:
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
		virtual int addChild(Widget* widget);

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
		virtual int insertChild(Widget* widget, const int index);

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
		virtual int removeChild(Widget* widget);
	};

} // namespace NativeUI

#endif /* NATIVEUI_CUSTOM_PICKER_H_ */

/*! @} */
