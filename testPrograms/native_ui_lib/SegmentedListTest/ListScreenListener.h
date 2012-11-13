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
 * ListScreenListener.h
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */

#ifndef LISTSCREENLISTENER_H_
#define LISTSCREENLISTENER_H_

/**
 * \brief Listener for list screen events.
 */
class ListScreenListener
{
public:
	/**
	 * This method is called when a list item is clicked.
	 * @param parentSection The parent section of the list view item clicked.
	 * @param item The list view item clicked.
	 */
	virtual void listScreenItemClicked(NativeUI::ListViewSection* &parentSection, NativeUI::ListViewItem* &listViewItem) = 0;
};


#endif /* LISTSCREENLISTENER_H_ */
