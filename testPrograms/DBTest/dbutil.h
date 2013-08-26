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

/*
 * dbutil.h
 *
 *  Created on: May 15, 2013
 *      Author: ovidel
 */

#ifndef DBUTIL_H_
#define DBUTIL_H_

#include <MAUtil/String.h>

/**
 * Utility class that simplifies using the Database API.
 */
class DBUtil
{
public:
	/**
	 * Get the path to the local file system.
	 * @return Path that ends with a slash.
	 */
	static const MAUtil::String getLocalPath();

	/**
	 * Utility function that gets a text field as a string, using the
	 * maDBCursorGetColumnText function.
	 * @param cursor Handle to a cursor object.
	 * @param column Zero-based index to the column to retrieve.
	 * @param path Reference to string that will receive the field data.
	 * @return MA_DB_OK on success, MA_DB_NULL if the field value is NULL,
	 * or MA_DB_ERROR on error.
	 */
	static int getColumnString(MAHandle cursor, int column, MAUtil::String& text);

	/**
	 * Utility function that gets a text field as a string, using the
	 * maDBCursorGetColumnData function.
	 * @param cursor Handle to a cursor object.
	 * @param column Zero-based index to the column to retrieve.
	 * @param path Reference to string that will receive the field data.
	 * @return MA_DB_OK on success, MA_DB_NULL if the field value is NULL,
	 * or MA_DB_ERROR on error.
	 */
	static int getColumnDataAsString(MAHandle cursor, int column, MAUtil::String& text);
};


#endif /* DBUTIL_H_ */
