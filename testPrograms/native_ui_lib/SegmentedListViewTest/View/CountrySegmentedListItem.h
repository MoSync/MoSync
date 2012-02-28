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
 * @file CountrySegmentedListItem.h
 * @author Bogdan Iusco
 * @date 28 Feb 2012
 *
 * @brief Country item class for segmented list widget.
 */

#ifndef COUNTRYSEGMENTEDLISTITEM_H_
#define COUNTRYSEGMENTEDLISTITEM_H_

#include <MAUtil/String.h>

#include "../wrapper/SegmentedListViewItem.h"

using namespace NativeUI;
using namespace MAUtil;

class CountrySegmentedListItem : public SegmentedListViewItem
{
public:
	/**
	 * Constructor.
	 * @param countryName String displayed on the list item.
	 */
	CountrySegmentedListItem(const String& countryName);

	/**
	 * Destructor.
	 */
	virtual ~CountrySegmentedListItem();

private:
	/**
	 * Create UI widgets and add them to item.
	 */
	void createUI();

private:
	/**
	 * Country name displayed on the list item.
	 */
	String* mCountryName;
};

#endif /* COUNTRYSEGMENTEDLISTITEM_H_ */
