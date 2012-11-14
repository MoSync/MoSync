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
 * @file DataSource.h
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Class that stores application data(e.g. country name).
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#include "IDataSource.h"

using namespace MAUtil;

// Forward declaration.
class ListSection;

/**
 * @brief Store application data(e.g. country name).
 */
class DataSource : public IDataSource
{
public:
	/**
	 * Contructor.
	 */
	DataSource();

	/**
	 * Destructor.
	 */
	virtual ~DataSource();

	/**
	 * Get the number of sections.
	 * @return The number of sections.
	 */
	virtual const int getSectionCount();

	/**
	 * Get a specified section.
	 * @param index Section's index to get.
	 * @return The specified section if the index is valid, NULL otherwise.
	 * The ownership of the result is NOT passed to the caller.
	 */
	virtual const ListSection* getSection(const int index);

private:
	/**
	 * Create list sections.
	 */
	void createSource();

	/**
	 * Get a section using section title.
	 * @param sectionTitle Section title to get.
	 * @return Section with the given title if found, NULL otherwise.
	 * Ownership of the result is NOT passed to the caller.
	 */
	ListSection* getSection(const String& sectionTitle);

	/**
	 * Create a new section.
	 * @param sectionTitle Section's title.
	 * @return The new section.
	 * Ownership of the result is passed to the caller.
	 */
	ListSection* createSection(const String& sectionTitle);

private:
	/**
	 * Sections for country segmented list.
	 */
	Vector<ListSection*> mSections;
};

#endif /* DATASOURCE_H_ */
