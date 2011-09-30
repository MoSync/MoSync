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
 * High-level utility functions for working with files.
 */

#ifndef JOSYNC_FILE_UTIL_H_
#define JOSYNC_FILE_UTIL_H_

#include <ma.h>
#include <MAUtil/String.h>

namespace josync
{
/**
 * Class that contains utility methods.
 * TODO: Add copy constructor and assignment operator.
 */
class FileUtil
{
	public:
		/**
		 * Constructor.
		 */
		FileUtil();

	public:
		/**
		 * Destructor.
		 */
		virtual ~FileUtil();

		/**
		 * Get the path to the local file system.
		 * @return Path that ends with a slash.
		 */
		virtual MAUtil::String getLocalPath();

		/**
		 * Extract a file system bundle to a directory in the
		 * local file system on the device/emulator.
		 * @param handle The resource handle of the bundled
		 * file system. Bundles are created with the Bundle tool
		 * that comes with MoSync.
		 * @param destinationPath The path of the directory where
		 * the file system will be extracted.
		 */
		virtual bool extractFileSystem(
			MAHandle handle,
			const MAUtil::String& destinationPath);

		/**
		 * Extract the files in the file system bundle defined
		 * as the first resource in the resource file. Files are
		 * extracted to the root of the local file system on the
		 * device/emulator.
		 *
		 * This function is intended as a high-level way to
		 * extract files packaged in the standard way used in
		 * the HTML5 Project Template.
		 */
		virtual bool extractLocalFiles();

		/**
		 * Open a file for writing.
		 * Create the file if it does not exist.
		 * Note: Will truncate the file if it exists.
		 * @return Handle to the open file, <0 on error.
		 */
		virtual MAHandle openFileForWriting(const MAUtil::String& filePath);

		/**
		 * Open a file for reading.
		 * @return Handle to the open file, <0 on error.
		 */
		virtual MAHandle openFileForReading(const MAUtil::String& filePath);
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
	};

} // namespace

#endif
