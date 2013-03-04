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

/**
 * @file ListViewSection.h
 * @author Bogdan Iusco
 *
 * @brief A ListViewSection widget is used to group one or more
 * ListViewItem objects.
 */

#include <mavsprintf.h>
#include "ListViewSection.h"
#include "ListViewItem.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	ListViewSection::ListViewSection(ListViewSectionType type) :
		Widget(MAW_LIST_VIEW_SECTION)
	{
		this->setPropertyInt(
			MAW_LIST_VIEW_SECTION_TYPE,
			this->getListViewSectionTypeFromEnum(type));
	}

	/**
	 * Get the list view section type.
	 * @return List view section type.
	 */
	ListViewSectionType ListViewSection::getType()
	{
		 int type = this->getPropertyInt(MAW_LIST_VIEW_SECTION_TYPE);
		 return this->getListViewSectionTypeEnum(type);
	}

	/**
	 * Add an list item to this section.
	 * @param item The item that will be added.
	 * The ownership of the item object is passed to this method.
	 */
	void ListViewSection::addItem(ListViewItem* item)
	{
		this->addChild(item);
	}

	/**
	 * Insert a list item to this section at a certain index.
	 * @param item The item that will be added.
	 * @param index The index at which the item will be added
	 * The ownership of the item object is passed to this method.
	 */
	void ListViewSection::insertItem(ListViewItem* item, int index)
	{
		this->insertChild(item, index);
	}

	/**
	 * Removes a list item from this section.
	 * @param item The item that will be removed.
	 */
	void ListViewSection::removeItem(ListViewItem* item)
	{
		this->removeChild(item);
	}

	/**
	 * Removes a list item from this section.
	 * @param index The index of the item that will be removed.
	 */
	void ListViewSection::removeItem(int index)
	{
		if (this->countChildWidgets() > index)
		{
			this->removeChild(this->getChild(index));
		}
	}

	/**
	 * Gets a list item from this section.
	 * @param index The index of the item.
	 */
	ListViewItem* ListViewSection::getItem(int index)
	{
		return (ListViewItem*) this->getChild(index);
	}

	/**
	 * Set section title.
	 * On iOS section title will appear on the right side of the list.
	 * On Android section title will appear on the fast scroll thumb.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @param title The given title.
	 */
	void ListViewSection::setTitle(const MAUtil::String& title)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_TITLE, title);
	}

	/**
	 * Get section title.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @return Section title.
	 */
	MAUtil::String ListViewSection::getTitle()
	{
		return this->getPropertyString(MAW_LIST_VIEW_SECTION_TITLE);
	}

	/**
	 * Set the section header text.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @param headerText The given header text.
	 */
	void ListViewSection::setHeaderText(
		const MAUtil::String& headerText)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_HEADER, headerText);
	}

	/**
	 * Get the section header text.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @return Header text.
	 */
	MAUtil::String ListViewSection::getHeaderText()
	{
		return this->getPropertyString(MAW_LIST_VIEW_SECTION_HEADER);
	}

	/**
	 * Set the section footer text.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @param footerText The given footer text.
	 * Note: On Android you can set this to am empty string
	 * for the header area to be removed.
	 */
	void ListViewSection::setFooterText(
		const MAUtil::String& footerText)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_FOOTER, footerText);
	}

	/**
	 * Get the section footer text.
	 * Platform: iOS, Android and Windows Phone 7.
	 * @return Footer text.
	 */
	MAUtil::String ListViewSection::getFooterText()
	{
		return this->getPropertyString(MAW_LIST_VIEW_SECTION_FOOTER);
	}

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
	int ListViewSection::addChild(Widget* widget)
	{
		return Widget::addChild(widget);
	}

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
	int ListViewSection::insertChild(Widget* widget, const int index)
	{
		return Widget::insertChild(widget, index);
	}

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
	int ListViewSection::removeChild(Widget* widget)
	{
		return Widget::removeChild(widget);
	}

	/**
	 * Get the list view section type constant.
	 * @param sectionType Given list section type enum.
	 * @return One of the following values:
	 * - MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
	 * - MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED
	 */
	int ListViewSection::getListViewSectionTypeFromEnum(ListViewSectionType sectionType)
	{
		int type;
		switch (sectionType)
		{
		case LIST_VIEW_SECTION_TYPE_ALPHABETICAL:
			type = MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
			break;
		case LIST_VIEW_SECTION_TYPE_SEGMENTED:
			type = MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED;
			break;
		default:
			type = MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
			break;
		}

		return type;
	}

	/**
	 * Get the list view section type enum from a constant.
	 * @param sectionType One of the following values:
	 * - MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
	 * - MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED
	 * @return One of the ListViewSectionType enum values.
	 */
	ListViewSectionType ListViewSection::getListViewSectionTypeEnum(int sectionType)
	{
		ListViewSectionType type;
		switch (sectionType)
		{
		case MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL:
			type = LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
			break;
		case MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED:
			type = LIST_VIEW_SECTION_TYPE_SEGMENTED;
			break;
		default:
			type = LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
			break;
		}

		return type;
	}

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
    int ListViewSection::setHeaderBackgroundColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return setProperty(MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND, buffer);
    }

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
    int ListViewSection::setFooterBackgroundColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return setProperty(MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND, buffer);
    }

    /**
     * Set the font color of the header text.
     * Platform: Android and Windows Phone 7.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
     */
    int ListViewSection::setHeaderFontColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return this->setProperty(MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR, buffer);
    }

    /**
     * Set the font color of the footer text.
     * Platform: Android and Windows Phone 7.
     * @param color A hexadecimal value 0xRRGGBB, where R, G and B are the
     *              red, green and blue components respectively.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the color value was invalid.
     */
    int ListViewSection::setFooterFontColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return this->setProperty(MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR, buffer);
    }

    /**
     * Set the font size in points of the header text.
     * Platform: Android and Windows Phone 7.
     * @param size A float that specifies the number of pixels for Android
     * and WP7.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
     */
    int ListViewSection::setHeaderFontSize(const float size)
    {
        return this->setPropertyFloat(MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE, size);
    }

    /**
     * Set the font size in points of the footer text.
     * Platform: Android and Windows Phone 7.
     * @param size A float that specifies the number of pixels for Android
     * and WP7.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the size value was invalid.
     */
    int ListViewSection::setFooterFontSize(const float size)
    {
        return this->setPropertyFloat(MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE, size);
    }

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
    int ListViewSection::setHeaderFont(const MAHandle fontHandle)
    {
		return this->setPropertyInt(MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE, fontHandle);
    }

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
    int ListViewSection::setFooterFont(const MAHandle fontHandle)
    {
		return this->setPropertyInt(MAW_LIST_VIEW_SECTION_FOOTER_FONT_HANDLE, fontHandle);
    }

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
    int ListViewSection::setHeaderTextVerticalAlignment(const MAUtil::String& alignment)
    {
		return this->setProperty(MAW_LIST_VIEW_SECTION_HEADER_VERTICAL_ALIGNMENT, alignment);
    }

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
    int ListViewSection::setFooterTextVerticalAlignment(const MAUtil::String& alignment)
    {
		return this->setProperty(MAW_LIST_VIEW_SECTION_FOOTER_VERTICAL_ALIGNMENT, alignment);
    }

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
    int ListViewSection::setHeaderTextHorizontalAlignment(const MAUtil::String& alignment)
    {
		return this->setProperty(MAW_LIST_VIEW_SECTION_HEADER_HORIZONTAL_ALIGNMENT, alignment);
    }

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
    int ListViewSection::setFooterTextHorizontalAlignment(const MAUtil::String& alignment)
    {
		return this->setProperty(MAW_LIST_VIEW_SECTION_FOOTER_HORIZONTAL_ALIGNMENT, alignment);
    }
}
