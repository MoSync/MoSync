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
 * @file IndexedListScreen.h
 * @author Bogdan Iusco
 * @date 27 Feb 2012
 *
 * @brief Shows an indexed segmented list view.
 * Screen shown in the application's first tab.
 */

#ifndef INDEXEDLISTSCREEN_H_
#define INDEXEDLISTSCREEN_H_

#include <NativeUI/Screen.h>
#include "../wrapper/SegmentedListView.h"

using namespace NativeUI;

class IndexedListScreen: public Screen
{
public:
	/**
	 * Constructor.
	 */
	IndexedListScreen();

	/**
	 * Destructor.
	 */
	~IndexedListScreen();

private:
	/**
	 * Create widgets and add them to screen.
	 */
	void createUI();

private:
	/**
	 * Used for displaying items in a alphabetical list.
	 */
	SegmentedListView* mSegmentedListView;
};


#endif /* INDEXEDLISTSCREEN_H_ */
