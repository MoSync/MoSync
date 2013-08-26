/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
