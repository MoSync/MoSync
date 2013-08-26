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
 * @file DatabaseProduct.h
 * @author Bogdan Iusco
 * @date 25 May 2012
 *
 * @brief Defines a product stored in the database.
 * A DatabaseProduct represents a table row for database.
 */

#ifndef PURCHASE_DATABASE_PRODUCT_H_
#define PURCHASE_DATABASE_PRODUCT_H_

#include <MAUtil/String.h>

using namespace MAUtil;

class DatabaseProduct {
public:
	/**
	 * Constructor.
	 */
	DatabaseProduct();

	/**
	 * Destructor.
	 */
	virtual ~DatabaseProduct();

	/**
	 * Set the product id.
	 * @param productID Product id to set.
	 */
	void setProductID(const String& productID);

	/**
	 * Get the product id.
	 * @return The product id.
	 */
	String getProductID() const;

	/**
	 * Set the date then the product was purchased.
	 * @param seconds The number of seconds that elapsed since
	 * 1st January 1970.
	 */
	void setDate(const int seconds);

	/**
	 * Get the date then the product was purchased.
	 * @return The number of seconds that elapsed since 1st January 1970.
	 */
	int getDate() const;

private:
	/**
	 * Product id.
	 */
	String* mProductID;

	/**
	 * Date(in seconds, unix time) when the product was purchased.
	 */
	int mDate;
};

#endif /* PURCHASE_DATABASE_PRODUCT_H_ */
