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
