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

#include <MAUtil/String.h>

#include "IndexedListScreen.h"
#include "CountrySegmentedListItem.h"
#include "../Model/ListSection.h"
#include "../wrapper/SegmentedListViewSection.h"

using namespace MAUtil;

/**
 * Constructor.
 * @param dataSource Segmented list's data source.
 */
IndexedListScreen::IndexedListScreen(IDataSource& dataSource) :
	Screen(),
	mDataSource(dataSource),
	mSegmentedListView(NULL)
{
	this->setTitle(SCREEN_TITLE);

	this->createUI();
	this->populateList();
	mSegmentedListView->reloadData();
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

/**
 * Populate list with data.
 */
void IndexedListScreen::populateList()
{
	int countSections = mDataSource.getSectionCount();
	for (int i = 0; i < countSections; i++)
	{
		const ListSection* section = mDataSource.getSection(i);
		this->addSectionDataToList(*section);
	}
}

/**
 * Add a given section to list.
 * @param section Section to add.
 */
void IndexedListScreen::addSectionDataToList(const ListSection& section)
{
	SegmentedListViewSection* segmentedListViewSection =
		new SegmentedListViewSection();
	segmentedListViewSection->setTitle(section.getTitle());
	segmentedListViewSection->setHeaderText(section.getTitle());

	int countCells = section.getCellCount();
	for (int i = 0; i < countCells; i++)
	{
		const String* cellText = section.getCellText(i);

		// Create cell and add it to section.
		CountrySegmentedListItem* cell = new CountrySegmentedListItem(*cellText);
		segmentedListViewSection->addItem(cell);
	}

	mSegmentedListView->addSection(segmentedListViewSection);
}

/**
 * Allow editing value has changed.
 * @param state True if allowed, false otherwise.
 */
void IndexedListScreen::allowEditing(bool state)
{

}

/**
 * Allow moving list items value has changed.
 * @param state True if allowed, false otherwise.
 */
void IndexedListScreen::allowMoving(bool state)
{

}

/**
 * List mode has changed.
 * @param listMode The new list mode.
 */
void IndexedListScreen::listModeChanged(SegmentedListViewMode listMode)
{
	mSegmentedListView->setMode(listMode);
}
