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
 * @file IDatabaseManager.h
 * @author Bogdan Iusco
 * @date 25 May 2012
 *
 * @brief Interface to application's database.
 * Used to access the database.
 */

#ifndef PURCHASE_I_DATABASE_MANAGER_H_
#define PURCHASE_I_DATABASE_MANAGER_H_

#include <MAUtil/String.h>

namespace PurchaseTest
{

	class DatabaseProduct;

	/**
	 * @brief Interface to application's database.
	 * Used to access the database.
	 */
	class IDatabaseManager
	{
	public:
		/**
		 * Add a new database row.
		 * @param dbProduct Row to add.
		 */
		virtual void addRow(const DatabaseProduct& dbProduct) = 0;

		/**
		 * Get a database row that contains a given product id.
		 * @param productID Identifies the desired row.
		 * @return A database row if the the productID was found, NULL otherwise.
		 */
		virtual DatabaseProduct* getRow(const MAUtil::String& productID) = 0;
	};
};


#endif /* PURCHASE_I_DATABASE_MANAGER_H_ */
