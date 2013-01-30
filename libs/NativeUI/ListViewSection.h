/*
Copyright (C) 2012 MoSync AB

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
 * @file ListViewSection.h
 * @author Bogdan Iusco
 *
 * @brief A ListViewSection widget is used to group one or more
 * ListViewItem objects.
 */

#ifndef NATIVEUI_LIST_VIEW_SECTION_H_
#define NATIVEUI_LIST_VIEW_SECTION_H_

#include <NativeUI/Widget.h>
#include <MAUtil/String.h>

namespace NativeUI
{

	/**
	 * @brief List view section types.
	 * Platform: iOS, Android and Windows Phone.
	 */
	enum ListViewSectionType
	{
		/**
		 * @brief A section that presents a group of rows, that correspond to the same title.
		 */
		LIST_VIEW_SECTION_TYPE_ALPHABETICAL = 0,
		/**
		 * @brief AA section that presents a group of rows, specifically list view items.
		 * Header and footer information can be set to segmented sections.
		 */
		LIST_VIEW_SECTION_TYPE_SEGMENTED
	};

	// Forward declaration.
	class ListViewItem;

	/**
	 * @brief A ListViewSection widget is used to group one or more
	 * ListViewItem objects.
	 */
	class ListViewSection : public Widget
	{
	public:
		/**
		 * Constructor.
		 * @param type List view section's type.
		 */
		ListViewSection(ListViewSectionType type);

		/**
		 * Get the list view section type.
		 * @return List view section type.
		 */
		ListViewSectionType getType();

		/**
		 * Add a list item to this section.
		 * @param item The item that will be added.
		 * The ownership of the item object is passed to this method.
		 */
		void addItem(ListViewItem* item);

		/**
		 * Insert a list item to this section at a certain index.
		 * @param item The item that will be added.
		 * @param index The index at which the item will be added
		 * The ownership of the item object is passed to this method.
		 */
		void insertItem(ListViewItem* item, int index);

		/**
		 * Removes a list item from this section.
		 * @param item The item that will be removed.
		 */
		void removeItem(ListViewItem* item);

		/**
		 * Removes a list item from this section.
		 * @param index The index of the item that will be removed.
		 */
		void removeItem(int index);

		/**
		 * Gets a list item from this section.
		 * @param index The index of the item.
		 */
		ListViewItem* getItem(int index);

		/**
		 * Set section title.
		 * On iOS section title will appear on the right side of the list.
		 * On Android section title will appear on the fast scroll thumb.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @param title The given title.
		 */
		void setTitle(const MAUtil::String& title);

		/**
		 * Get section title.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @return Section title.
		 */
		MAUtil::String getTitle();

		/**
		 * Set the section header text.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @param headerText The given header text.
		 */
		void setHeaderText(const MAUtil::String& headerText);

		/**
		 * Get the section header text.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @return Header text.
		 */
		MAUtil::String getHeaderText();

		/**
		 * Set the section footer text.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @param footerText The given footer text.
		 * Note: On Android you can set this to am empty string
		 * for the header area to be removed.
		 */
		void setFooterText(const MAUtil::String& footerText);

		/**
		 * Get the section footer text.
		 * Platform: iOS, Android and Windows Phone 7.
		 * @return Footer text.
		 */
		MAUtil::String getFooterText();

        /**
         * Set the background color of the header row.
         * Platform: Android and Windows Phone 7.
         * @param color A hexadecimal color value, e.g. 0xFF0000.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        int setHeaderBackgroundColor(const int color);

        /**
         * Set the background color of the footer row.
         * Platform: Android and Windows Phone 7.
         * @param color A hexadecimal color value, e.g. 0xFF0000.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the value was set.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        int setFooterBackgroundColor(const int color);

        /**
         * Set the font color of the header text.
         * Platform: Android and Windows Phone 7.
         * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
         *              red, green and blue components respectively.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
         */
        int setHeaderFontColor(const int color);

        /**
         * Set the font color of the footer text.
         * Platform: Android and Windows Phone 7.
         * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
         *              red, green and blue components respectively.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
         */
        int setFooterFontColor(const int color);

        /**
         * Set the font size in points of the header text.
         * Platform: Android and Windows Phone 7.
         * @param size A float that specifies the number of pixels for Android
         * and WP7, and number of points for iOS.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
         */
        int setHeaderFontSize(const float size);

        /**
         * Set the font size in points of the footer text.
         * Platform: Android and Windows Phone 7.
         * @param size A float that specifies the number of pixels.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
         */
        int setFooterFontSize(const float size);

		/**
		* Sets the typeface and style in which the header text should be displayed.
		* Platform: Android and Windows Phone 7.
		* @param fontHandle A font handle received from loading fonts using
		*  #maFontGetName and #maFontLoadWithName syscalls.
		* @return Any of the following result codes:
		* - #MAW_RES_OK if the property could be set.
		* - #MAW_RES_INVALID_HANDLE if the handle was invalid.
		* - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
		* - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		* - #MAW_RES_ERROR otherwise.
		*/
        int setHeaderFont(const MAHandle fontHandle);

		/**
		* Sets the typeface and style in which the footer text should be displayed.
		* Platform: Android and Windows Phone 7.
		* @param fontHandle A font handle received from loading fonts using
		*  #maFontGetName and #maFontLoadWithName syscalls.
		* @return Any of the following result codes:
		* - #MAW_RES_OK if the property could be set.
		* - #MAW_RES_INVALID_HANDLE if the handle was invalid.
		* - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
		* - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		* - #MAW_RES_ERROR otherwise.
		*/
        int setFooterFont(const MAHandle fontHandle);

        /**
         * Set the vertical alignment of the header text.
         * Platform: Android and Windows Phone 7.
         * @param alignment One of the constants:
         * - #MAW_ALIGNMENT_TOP
         * - #MAW_ALIGNMENT_CENTER
         * - #MAW_ALIGNMENT_BOTTOM
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
         */
        int setHeaderTextVerticalAlignment(const MAUtil::String& alignment);

        /**
         * Set the vertical alignment of the footer text.
         * Platform: Android and Windows Phone 7.
         * @param alignment One of the constants:
         * - #MAW_ALIGNMENT_TOP
         * - #MAW_ALIGNMENT_CENTER
         * - #MAW_ALIGNMENT_BOTTOM
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
         */
        int setFooterTextVerticalAlignment(const MAUtil::String& alignment);

        /**
         * Set the horizontal alignment of the header text.
         * Platform: Android and Windows Phone 7.
         * @param alignment one of the constants:
         * - #MAW_ALIGNMENT_LEFT
         * - #MAW_ALIGNMENT_CENTER
         * - #MAW_ALIGNMENT_RIGHT
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
         */
        int setHeaderTextHorizontalAlignment(const MAUtil::String& alignment);

        /**
         * Set the horizontal alignment of the footer text.
         * Platform: Android and Windows Phone 7.
         * @param alignment one of the constants:
         * - #MAW_ALIGNMENT_LEFT
         * - #MAW_ALIGNMENT_CENTER
         * - #MAW_ALIGNMENT_RIGHT
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the alignment value was invalid.
         */
        int setFooterTextHorizontalAlignment(const MAUtil::String& alignment);
	private:
		/**
		 * Add a widget as a child of this widget.
		 * @param widget The widget that will be added.
		 * The ownership of the widget is passed to this function.
		 * When the parent widget will be destroyed, all child widgets
		 * will be deleted.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be added to the parent.
		 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
		 * - #MAW_RES_ERROR if it could not be added for some other reason.
		 */
		int addChild(Widget* widget);

		/**
		 * Insert a widget as a child of this widget, at a given position.
		 * @param widget The widget to be added.
		 * The ownership of the widget is passed to this function.
		 * When the parent widget will be destroyed, all child widgets
		 * will be deleted.
		 * @param index The position on which to add the child.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be added to the parent.
		 * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
		 * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
		 * - #MAW_RES_ERROR if it could not be added for some other reason.
		 */
		int insertChild(Widget* widget, const int index);

		/**
		 * Remove a child widget from its parent (but does not destroy it).
		 * Removing a currently visible top-level widget causes the MoSync view
		 * to become visible.
		 * When the parent widget will be destroyed, the child widget will not
		 * be deleted.
		 * @param widget The widget to be removed.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be removed from the parent.
		 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		int removeChild(Widget* widget);

		/**
		 * Get the list view section type constant.
		 * @param sectionType Given list section type enum.
		 * @return One of the following values:
		 * - MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
		 * - MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED
		 */
		int getListViewSectionTypeFromEnum(ListViewSectionType sectionType);

		/**
		 * Get the list view section type enum from a constant.
		 * @param sectionType One of the following values:
		 * - MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
		 * - MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED
		 * @return One of the ListViewSectiontype enum values.
		 */
		ListViewSectionType getListViewSectionTypeEnum(int sectionType);
	};
} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_SECTION_H_ */
/*! @} */
