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
	 * Get the path to the SD card on the Android file system.
	 * @return Path that ends with a slash.
	 */
	virtual MAUtil::String getAndroidSDCardPath();

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
