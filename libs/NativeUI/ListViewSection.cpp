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

#include "ListViewSection.h"
#include "ListViewItem.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	ListViewSection::ListViewSection() :
		Widget(MAW_LIST_VIEW_SECTION)
	{

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
	 * Set section title.
	 * Section title will appear on the right side of the list.
	 * Platform: iOS.
	 * @param title The given title.
	 */
	void ListViewSection::setTitle(const MAUtil::String& title)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_TITLE, title);
	}

	/**
	 * Get section title.
	 * Platform: iOS.
	 * @return Section title.
	 */
	MAUtil::String ListViewSection::getTitle()
	{
		return this->getPropertyString(MAW_LIST_VIEW_SECTION_TITLE);
	}

	/**
	 * Set the section header text.
	 * Platform: iOS.
	 * @param headerText The given header text.
	 */
	void ListViewSection::setHeaderText(
		const MAUtil::String& headerText)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_HEADER, headerText);
	}

	/**
	 * Get the section header text.
	 * Platform: iOS.
	 * @return Header text.
	 */
	MAUtil::String ListViewSection::getHeaderText()
	{
		return this->getPropertyString(MAW_LIST_VIEW_SECTION_HEADER);
	}

	/**
	 * Set the section footer text.
	 * Platform: iOS.
	 * @param footerText The given footer text.
	 */
	void ListViewSection::setFooterText(
		const MAUtil::String& footerText)
	{
		this->setProperty(MAW_LIST_VIEW_SECTION_FOOTER, footerText);
	}

	/**
	 * Get the section footer text.
	 * Platform: iOS.
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
}
