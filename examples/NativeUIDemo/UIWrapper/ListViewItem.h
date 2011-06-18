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
 * @file ListView.h
 * @author Emma Tresanszki
 *
 * Class for list view items.
 */
#ifndef MOSYNC_UI_LIST_VIEW_ITEM_H_
#define MOSYNC_UI_LIST_VIEW_ITEM_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for list views.
	 */
	class ListViewItem : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		ListViewItem();

		/**
		 * Destructor.
		 */
		virtual ~ListViewItem();

		/*
		 * Sets the text part of the list view item.
		 * @param text The text of the item.
		 */
		virtual void setText(const MAUtil::String& text);

		/*
		 * Sets an icon of the list view item that is placed to the left of the text.
		 * @param resource MoSync handle to an uncompressed image.
		 */
		virtual void setIcon(MAHandle imageHandle);
	};

	} // namespace UI
} // namespace MoSync

#endif
