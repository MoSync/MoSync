/*
Copyright (C) 2011 MoSync AB

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


#include "ReadCSV.h"
#include "Utils.h"
#include <mavsprintf.h>

/**
 * \brief GetString,	read whole textfile resource in to a string buffer
 * @param resource,		to a text file.
 * @return	char *,		return the text buffer
 */
char* GetString(MAHandle resource)
{
    // Get the length of the string data.
    int length = maGetDataSize(resource);

    // Allocate space for the string data plus the
    // null termination character.
    char* buffer = new char[length + 1];

    // Read data.
    maReadData(resource, buffer, 0, length);

    // Null terminate the string.
    buffer[length] = '\0';

    // Return the string.
    return buffer;
}

/**
 * \brief ReadCSV::load,
 * loading a CSV = Comma (Delimited) Separated Valued text file from a resource,
 * and store all data into a double array[][]
 *
 * @param resource,		resource is pointing to a csv file
 * @param delim,		use delimiter for separation
 * @param trim,			trim string remove spaces.
 * @return bool true/false, true = success, false = failure
 */
bool ReadCSV::load( MAHandle resource, char delim, bool trim)
{
	char *data = GetString(resource);	// will allocate buffer
	if (data == 0)
	{
	    lprintfln("Error ReadCSV::load failed to load resource\n");
		return false;
	}

	std::string line, tmp;
	std::vector<std::string> lineData;
	char *pch = strtok(data,"\r\n");
    line = pch;
//    lprintfln("ReadCSV::Load Line=%s",line.c_str());
	lineData = Utils::split(line,delim,trim);
	if (lineData.size() > /*1*/0)
		m_db.push_back(lineData);
	else
	    lprintfln("WARNING ReadCSV::load missing (First Line) lineData =%s\n",line.c_str());

	for (int i=0;pch != NULL;i++)
	{
	    pch = strtok(NULL, "\r\n");
		if (pch)
		{
			line = pch;
//			lprintfln("ReadCSV::Load Line=%s",line.c_str());
			lineData = Utils::split(line,delim,trim);
			if (lineData.size() > /*1*/0)
				m_db.push_back(lineData);
			else
				lprintfln("WARNING ReadCSV::load missing lineData from line %i => %s\n",i,line.c_str());
		}
	}

	delete [] data;						// need to free temp buffer.
	return true;
}

/**
 * \brief ReadCSV_hash::load,
 * loading a CSV = Comma (Delimited) Separated Valued text file from a resource,
 * and store all data into a hash table containing array of strings ["string is key"][]
 *
 * @param resource,		resource is pointing to a csv file
 * @param delim,		use delimiter for separation
 * @param trim,			trim string remove spaces.
 * @return bool true/false, true = success, false = failure
 */
bool ReadCSV_hash::load(MAHandle resource, char delim, bool trim)
{
	ReadCSV::load(resource,delim,trim);
	for(size_t i=0; i<m_db.size(); i++)
		m_table.insert( CSV_pair(m_db[i][0], m_db[i]) );

	return true;
}
