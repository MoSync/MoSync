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
 * @file CountrySegmentedListItem.cpp
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Country item class for segmented list widget.
 */

#define LABEL_COORD_LEFT 10
#define LABEL_COORD_TOP 10

#include <NativeUI/Label.h>
#include <NativeUI/RelativeLayout.h>

#include "CountrySegmentedListItem.h"

/**
 * Constructor.
 * @param countryName String displayed on the list item.
 */
CountrySegmentedListItem::CountrySegmentedListItem(const String& countryName) :
	SegmentedListViewItem()
{
	mCountryName = new String(countryName);
	this->createUI();
}

/**
 * Destructor.
 */
CountrySegmentedListItem::~CountrySegmentedListItem()
{
	delete mCountryName;
}

/**
 * Create UI widgets and add them to item.
 */
void CountrySegmentedListItem::createUI()
{
	RelativeLayout* layout = new RelativeLayout();
	layout->setWidth(this->getWidth());
	this->addChild(layout);

	Label* label = new Label();
	label->setWidth(this->getWidth()- LABEL_COORD_LEFT);
	label->setText(*mCountryName);
	label->setTopPosition(LABEL_COORD_TOP);
	label->setLeftPosition(LABEL_COORD_LEFT);
	layout->addChild(label);
}
