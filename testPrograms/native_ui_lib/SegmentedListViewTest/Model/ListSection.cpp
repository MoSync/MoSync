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
 * @file ListSection.cpp
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Class that stores data for a segmented list section(e.g. title, cells
 * text).
 */

#include "ListSection.h"

/**
 * Constructor.
 * @param sectionTitle Title assigned to section.
 */
ListSection::ListSection(const String& sectionTitle) :
	mTitle(NULL)
{
	mTitle = new String(sectionTitle);
}

/**
 * Destructor.
 */
ListSection::~ListSection()
{
	delete mTitle;
	for (int i = 0; i < mCellsText.size(); i++)
	{
		delete mCellsText[i];
	}
}

/**
 * Add a cell text to section.
 * @param cellText Cell text to add.
 */
void ListSection::addCellText(const String& cellText)
{
	String* cellTextToAdd = new String(cellText);
	// Ownership of the cellTextToAdd is passed to the vector object.
	mCellsText.add(cellTextToAdd);
}

/**
 * Get section's title.
 * @return Section's title.
 */
const String& ListSection::getTitle() const
{
	return *mTitle;
}

/**
 * Get the number of contained cells.
 * @return The number of contained cells.
 */
const int ListSection::getCellCount() const
{
	return mCellsText.size();
}

/**
 * Get cell's text at a given index.
 * @param index Cell's index.
 * @return Cell's text if index is valid, NULL otherwise.
 */
const String* ListSection::getCellText(const int index) const
{
	String* returnedText = NULL;
	if (index >=0 && index < mCellsText.size())
	{
		returnedText = mCellsText[index];
	}

	return returnedText;
}
