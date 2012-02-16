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

/**
 * @file FileUtil.h
 * @author Mikael Kindborg
 *
 * Utility library for working with files.
 */

#ifndef MOSYNC_FILEUTIL_H_
#define MOSYNC_FILEUTIL_H_

#include <ma.h>
#include <MAUtil/String.h>

namespace MoSync {

/**
 * Class that contains utility methods.
 */
class Platform
{
public:
	/**
	 * Create an instance for the current platform.
	 */
	static Platform* create();

	/**
	 * Constructor.
	 */
	Platform();

	/**
	 * Destructor.
	 */
	virtual ~Platform();

	/**
	 * Get the path to the local file system.
	 * @return Path that ends with a slash.
	 */
	virtual MAUtil::String getLocalPath();

	/**
	 * Write a data object to a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeDataToFile(
		const MAUtil::String& filePath,
		MAHandle outData);

	/**
	 * Write a text string to a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeTextToFile(
		const MAUtil::String& filePath,
		const MAUtil::String& outText);

	/**
	 * Read a data object from a file.
	 * @param filePath Full path of file to read.
	 * @param inPlaceholder Placeholder handle for data object.
	 * @return true on success, false on error.
	 */
	virtual bool readDataFromFile(
		const MAUtil::String& filePath,
		MAHandle inPlaceholder);

	/**
	 * Read a text string from a file.
	 * @return true on success, false on error.
	 */
	virtual bool readTextFromFile(
		const MAUtil::String& filePath,
		MAUtil::String& inText);

	/**
	 * Create a text string from data handle.
	 */
	virtual MAUtil::String createTextFromHandle(MAHandle data);

	/**
	 * Open a file for write (and read) access.
	 * Create the file if it does not exist.
	 * Note: Will truncate the file if it exists.
	 * @return Handle to the open file, <0 on error.
	 */
	virtual MAHandle openFileForWriting(const MAUtil::String& filePath);

	/**
	 * Open a file for read access.
	 * @return Handle to the open file, <0 on error.
	 */
	virtual MAHandle openFileForReading(const MAUtil::String& filePath);
};

} // namespace MoSync

#endif
