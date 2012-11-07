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
 * @file IDataSource.h
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Interface class for application data.
 */

#ifndef IDATASOURCE_H_
#define IDATASOURCE_H_

#include <MAUtil/String.h>

// Forward declaration.
class ListSection;

using namespace MAUtil;

/**
 * @brief Abstract class for DataSource.
 */
class IDataSource
{
public:
	/**
	 * Get the number of sections.
	 * @return The number of sections.
	 */
	virtual const int getSectionCount() = 0;

	/**
	 * Get a specified section.
	 * @param index Section's index to get.
	 * @return The specified section if the index is valid, NULL otherwise.
	 * The ownership of the result is NOT passed to the caller.
	 */
	virtual const ListSection* getSection(const int index) = 0;
};


#endif /* IDATASOURCE_H_ */
