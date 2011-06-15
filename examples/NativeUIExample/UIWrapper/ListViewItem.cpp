/*
Copyright (C) 2011 MoSync AB

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
 * @file ListViewItem.cpp
 * @author Emma Tresanszki
 *
 * Class for list view items.
 */

#include "ListViewItem.h"

namespace MoSync
{
	namespace UI
	{

	ListViewItem::ListViewItem() :
		Widget(MAW_LIST_VIEW_ITEM)
	{

		// Set some common default values to
		// simplify when creating list view items.
		this->fillSpaceHorizontally();
		this->wrapContentVertically();
	}

	/**
	 * Destructor.
	 */
	ListViewItem::~ListViewItem()
	{
	}

	/*
	 * Sets the text part of the list view item.
	 * @param text The text of the item.
	 */
	void ListViewItem::setText(const MAUtil::String& text)
	{
		setProperty(MAW_LIST_VIEW_ITEM_TEXT, text);
	}

	/*
	 * Sets an icon of the list view item that is placed to the left of the text.
	 * @param resource MoSync handle to an uncompressed image.
	 */
	void ListViewItem::setIcon(MAHandle imageHandle)
	{
		setProperty(MAW_LIST_VIEW_ITEM_ICON, imageHandle);
	}

	} // namespace UI
} // namespace MoSync
