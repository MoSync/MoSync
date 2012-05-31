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
 * @file DatabaseProduct.cpp
 * @author Bogdan Iusco
 * @date 25 May 2012
 *
 * @brief Defines a product stored in the database.
 * A DatabaseProduct represents a table row for database.
 */

#include "DatabaseProduct.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 */
	DatabaseProduct::DatabaseProduct():
		mProductID(NULL),
		mDate(0)
	{
		mProductID = new String();
	}

	/**
	 * Destructor.
	 */
	DatabaseProduct::~DatabaseProduct()
	{
		delete mProductID;
	}

	/**
	 * Set the product id.
	 * @param productID Product id to set.
	 */
	void DatabaseProduct::setProductID(const String& productID)
	{
		delete mProductID;
		mProductID = new String(productID);
	}

	/**
	 * Get the product id.
	 * @return The product id.
	 */
	String DatabaseProduct::getProductID() const
	{
		return *mProductID;
	}

	/**
	 * Set the date then the product was purchased.
	 * @param seconds The number of seconds that elapsed since
	 * 1st January 1970.
	 */
	void DatabaseProduct::setDate(const int seconds)
	{
		mDate = seconds;
	}

	/**
	 * Get the date then the product was purchased.
	 * @return The number of seconds that elapsed since 1st January 1970.
	 */
	int DatabaseProduct::getDate() const
	{
		return mDate;
	}

} // namespace PurchaseTest
