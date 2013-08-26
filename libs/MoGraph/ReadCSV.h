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

#ifndef READCSV_H_
#define READCSV_H_

#pragma once
//#include "MAHeaders.h"
#include <maapi.h>
#include <MAUtil/Moblet.h>

#include <vector>
#include <string>

#include "Utils.h"

/**
 * IReadCSV, interface class for reading comma separated values
 */
class IReadCSV
{
public:
	/**
	 * \brief IReadCSV, Destructor
	 */
	virtual ~IReadCSV() {}
	/**
	 * \brief load, load file with a CSV format.
	 * @param resource,	file
	 * @param delim, delimiter
	 * @param trim, is triming boolean true/false
	 * @return bSuccess true/false
	 */
	virtual bool load(MAHandle resource,char delim=',',bool trim=true) = 0;
};

//typedef std::pair<std::string, std::vector<std::string>> CSVElm;
/**
 * \brief Class ReadCSV
 *
 * \description: Read comma separated values (String), simple form of database container
 * Vector array with vector... or 2 dimensional arrays e.g. array[x] => vector of string => array[x][y] => string
 */
class ReadCSV : public IReadCSV
{
protected:
	std::vector<std::vector<std::string> >  m_db;

public:
	/**
	 * \brief ReadCSV, Destructor
	 */
	virtual ~ReadCSV() {}
	/**
	 * \brief load, load file with a CSV format.
	 * @param resource,	file
	 * @param delim, delimiter
	 * @param trim, is triming boolean true/false
	 * @return bSuccess true/false
	 */
	virtual bool load(MAHandle resource,char delim=',',bool trim=true);
	/**
	 * \brief getDB, returning the database
	 * @return table
	 */
	std::vector<std::vector<std::string> >& getDB() {return m_db;}
};

/**
 * \brief Class: ReadCSV_hash
 *
 * \description: Read Comma Separated Values (String), hash based data base (dictionary)
 * where the lookup key is a string table[ "test" ] => vector of strings
 */
typedef std::pair<std::string, std::vector<std::string> > CSV_pair;
typedef std::hash_map<std::string,std::vector<std::string> > CSVhashTable;
class ReadCSV_hash : public ReadCSV
{
protected:
	std::hash_map<std::string,std::vector<std::string> >  m_table;

public:
	/**
	 * \brief ~ReadCSV_hash, Destructor
	 */
	virtual ~ReadCSV_hash() {}
	/**
	 * \brief load,	loads a csv based resource file, useing delimiters optional trim
	 * @param resource,	source of csv file
	 * @param delim,	delimiter char
	 * @param trim,		optional what space trimming
	 * @return bSuccess, true/false
	 */
	virtual bool load(MAHandle resource,char delim=',',bool trim=true);
	/**
	 * \brief getDB, returning the database
	 * @return table
	 */
	CSVhashTable& getTable() {return m_table;}
};



#endif /* READCSV_H_ */
