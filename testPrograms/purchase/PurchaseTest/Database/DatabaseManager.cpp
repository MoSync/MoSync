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

#define DATABASE_NAME "PurchaseTest"
#define TABLE_NAME "ProductTable"

#include <conprint.h>

#include "DatabaseManager.h"
#include "DatabaseProduct.h"
#include "../Util.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 */
	DatabaseManager::DatabaseManager():
		mDatabase(-1)
	{
		this->init();
	}

	/**
	 * Destructor.
	 */
	DatabaseManager::~DatabaseManager()
	{
		maDBClose(mDatabase);
	}

	/**
	 * Add a new database row.
	 * @param dbProduct Row to add.
	 */
	void DatabaseManager::addRow(const DatabaseProduct& dbProduct)
	{
		char buffer[BUF_SIZE];
		sprintf(buffer, "INSERT INTO %s VALUES ('%s', %d)",
			TABLE_NAME,
			dbProduct.getProductID().c_str(),
			dbProduct.getDate());
		printf("Insert statement = %s", buffer);
		int result = maDBExecSQL(mDatabase, buffer);
		printf("Result insert = %d", result);
	}

	/**
	 * Get a database row that contains a given product id.
	 * @param productID Identifies the desired row.
	 * @return A database row if the the productID was found, NULL otherwise.
	 * The result's ownership is passed to the caller.
	 */
	DatabaseProduct* DatabaseManager::getRow(const MAUtil::String& productID)
	{
		DatabaseProduct* product = NULL;
		char buffer[BUF_SIZE];
		sprintf(buffer, "SELECT (date) FROM %s WHERE productid = '%s'",
			TABLE_NAME,
			productID.c_str());
		printf("Select statement: %s", buffer);
		MAHandle cursorx = maDBExecSQL(mDatabase, buffer);
		if (cursorx <= 0)
		{
			return NULL;
		}
		printf("cursorx = %d", cursorx);
		int result = maDBCursorNext(cursorx);
		printf("result maDBCursorNext = %d", result);
		if (result == MA_DB_OK)
		{
			int date;
			result = maDBCursorGetColumnInt(cursorx, 0, &date);
			printf("result maDBCursorGetColumnInt = %d", result);
			if (result == MA_DB_OK)
			{
				printf("date = %d", date);
				product = new DatabaseProduct();
				product->setProductID(productID);
				product->setDate(date);
			}
		}
		printf("before maDBCursorDestroy");
		maDBCursorDestroy(cursorx);
		return product;
	}

	/**
	 * Init database connection.
	 */
	void DatabaseManager::init()
	{
		MAUtil::String path = getLocalPath() + DATABASE_NAME;
		printf("Database path: %s", path.c_str());
		mDatabase = maDBOpen(path.c_str());
		printf("mDatabase = %d", mDatabase);

		// Create table if not exists.

		char buffer[BUF_SIZE];
		sprintf(buffer, "CREATE TABLE IF NOT EXISTS %s (productid TEXT, date INTEGER)",
			TABLE_NAME);
		printf("create table query = %s", buffer);
		int result = maDBExecSQL(mDatabase, buffer);
		printf("result = %d", result);
	}

	/**
	 * Get the path to the local file system.
	 * @return Path that ends with a slash.
	 */
	MAUtil::String DatabaseManager::getLocalPath()
	{
		int bufferSize = 1024;
		char buffer[bufferSize];

		int size = maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		// If there was an error, return default root path.
		if (size < 0 || size > bufferSize)
		{
			return "/";
		}

		return buffer;
	}
}
