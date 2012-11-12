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
 * @file DataSource.cpp
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Class that stores application data(e.g. country name).
 */

#include "DataSource.h"
#include "ListSection.h"

static const int sCountryNameCount = 27;
static String sCountryNameArray[sCountryNameCount] =
{
	"Austria",
	"Belgium",
	"Bulgaria",
	"Cyprus",
	"Czech Republic",
	"Denmark",
	"Estonia",
	"Finland",
	"France",
	"Germany",
	"Greece",
	"Hungary",
	"Ireland",
	"Italy",
	"Latvia",
	"Lithuania",
	"Luxembourg",
	"Malta",
	"Netherlands",
	"Poland",
	"Portugal",
	"Romania",
	"Slovakia",
	"Slovenia",
	"Spain",
	"Sweden",
	"United Kingdom"
};

/**
 * Contructor.
 */
DataSource::DataSource()
{
	this->createSource();
}

/**
 * Destructor.
 */
DataSource::~DataSource()
{
	for (int i = 0; i < mSections.size(); i++)
	{
		delete mSections[i];
	}
}

/**
 * Get the number of sections.
 * @return The number of sections.
 */
const int DataSource::getSectionCount()
{
	return mSections.size();
}

/**
 * Get a specified section.
 * @param index Section's index to get.
 * @return The specified section if the index is valid, NULL otherwise.
 * The ownership of the result is NOT passed to the caller.
 */
const ListSection* DataSource::getSection(const int index)
{
	ListSection* section = NULL;
	if (index >= 0 && index < mSections.size())
	{
		section = mSections[index];
	}

	return section;
}

/**
 * Create list sections.
 */
void DataSource::createSource()
{
	for (int i = 0; i < sCountryNameCount; i++)
	{
		String countryName = sCountryNameArray[i];
		String sectionTitle = "";
		if (countryName.size() > 0)
		{
			sectionTitle.append(countryName.c_str(), 1);
		}

		// Check if the section needed to add cell text exist.
		ListSection* section = this->getSection(sectionTitle);
		if (!section)
		{
			// Section does not exist. Create a new one.
			section = this->createSection(sectionTitle);
			// Ownership of the section object is passed to the array.
			mSections.add(section);
		}

		section->addCellText(countryName);
	}
}

/**
 * Get a section using section title.
 * @param sectionTitle Section title to get.
 * @return Section with the given title if found, NULL otherwise.
 * Ownership of the result is NOT passed to the caller.
 */
ListSection* DataSource::getSection(const String& sectionTitle)
{
	ListSection* returnedSection = NULL;
	for (int i = 0; i < mSections.size(); i++)
	{
		ListSection* section = mSections[i];
		if (section->getTitle() == sectionTitle)
		{
			returnedSection = section;
			break;
		}
	}

	return returnedSection;
}

/**
 * Create a new section.
 * @param sectionTitle Section's title.
 * @return The new section.
 * Ownership of the result is passed to the caller.
 */
ListSection* DataSource::createSection(const String& sectionTitle)
{
	ListSection* section = new ListSection(sectionTitle);
	return section;
}
