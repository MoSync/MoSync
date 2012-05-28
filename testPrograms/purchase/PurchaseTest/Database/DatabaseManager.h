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
 * @file DatabaseManager.h
 * @author Bogdan Iusco
 * @date 23 May 2012
 *
 * @brief Application's database.
 * Used to store identifiers and dates for some types of products.
 */

#ifndef PURCHASE_DATABASE_MANAGER_H_
#define PURCHASE_DATABASE_MANAGER_H_

#include <MAUtil/String.h>

#include "IDatabaseManager.h"

namespace PurchaseTest
{

	class DatabaseProduct;

	/**
	 * @brief Application's database.
	 * Used to store identifiers and dates for some types of products.
	 */
	class DatabaseManager: public IDatabaseManager
	{
	public:
		/**
		 * Constructor.
		 */
		DatabaseManager();

		/**
		 * Destructor.
		 */
		virtual ~DatabaseManager();

		/**
		 * Add a new database row.
		 * @param dbProduct Row to add.
		 */
		virtual void addRow(const DatabaseProduct& dbProduct);

		/**
		 * Get a database row that contains a given product id.
		 * @param productID Identifies the desired row.
		 * @return A database row if the the productID was found, NULL otherwise.
		 * The result's ownership is passed to the caller.
		 */
		virtual DatabaseProduct* getRow(const MAUtil::String& productID);

	private:
		/**
		 * Init database connection.
		 */
		void init();

		/**
		 * Get the path to the local file system.
		 * @return Path that ends with a slash.
		 */
		MAUtil::String getLocalPath();

	private:
		/**
		 * Handle to the database.
		 */
		MAHandle mDatabase;
	};

} // namespace PurchaseTest

#endif /* PURCHASE_DATABASE_MANAGER_H_ */
