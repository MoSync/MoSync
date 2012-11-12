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
 * @file ListSection.h
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Class that stores data for a segmented list section(e.g. title, cells
 * text).
 */

#ifndef LISTSECTION_H_
#define LISTSECTION_H_

#include  <MAUtil/Vector.h>
#include <MAUtil/String.h>

using namespace MAUtil;

/**
 * Class that stores data for a segmented list section(e.g. title, cells
 * text).
 */
class ListSection
{
public:
	/**
	 * Constructor.
	 * @param sectionTitle Title assigned to section.
	 */
	ListSection(const String& sectionTitle);

	/**
	 * Destructor.
	 */
	virtual ~ListSection();

	/**
	 * Add a cell text to section.
	 * @param cellText Cell text to add.
	 */
	void addCellText(const String& cellText);

	/**
	 * Get section's title.
	 * @return Section's title.
	 */
	const String& getTitle() const;

	/**
	 * Get the number of contained cells.
	 * @return The number of contained cells.
	 */
	const int getCellCount() const;

	/**
	 * Get cell's text at a given index.
	 * @param index Cell's index.
	 * @return Cell's text if index is valid, NULL otherwise.
	 */
	const String* getCellText(const int index) const;

private:
	/**
	 * Section title.
	 */
	String* mTitle;

	/**
	 *
	 */
	Vector<String*> mCellsText;
};


#endif /* LISTSECTION_H_ */
