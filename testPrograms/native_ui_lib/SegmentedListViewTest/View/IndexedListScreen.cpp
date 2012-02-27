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
 * @file IndexedListScreen.cpp
 * @author Bogdan Iusco
 * @date 27 Feb 2012
 *
 * @brief Shows an indexed segmented list view.
 * Screen shown in the application's first tab.
 */

#define SCREEN_TITLE "Indexed list"

#include "IndexedListScreen.h"

/**
 * Constructor.
 */
IndexedListScreen::IndexedListScreen() :
	Screen(),
	mSegmentedListView(NULL)
{
	this->createUI();
	this->setTitle(SCREEN_TITLE);
}

/**
 * Destructor.
 */
IndexedListScreen::~IndexedListScreen()
{

}

/**
 * Create widgets and add them to screen.
 */
void IndexedListScreen::createUI()
{
	mSegmentedListView = new SegmentedListView();
	this->setMainWidget(mSegmentedListView);

}
