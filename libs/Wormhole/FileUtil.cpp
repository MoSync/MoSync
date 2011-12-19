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
 * @file FileUtil.cpp
 * @author Mikael Kindborg
 *
 * High-level utility functions for working with files.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions
#include <MAUtil/String.h>	// C++ String class

// Since MAFS redefines types used in newlib, we need to prevent
// them from being defined when using newlib.
#ifdef USE_NEWLIB
#define DONT_USE_TYPE_DEFINES
#endif

#include <MAFS/File.h>		// Library for file system bundles

#include "FileUtil.h"

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	FileUtil::FileUtil()
	{
	}

	/**
	 * Destructor.
	 */
	FileUtil::~FileUtil()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Get the path to the local file system.
	 * @return Path that ends with a slash.
	 */
	MAUtil::String FileUtil::getLocalPath()
	{
		int bufferSize = 1024;
		char buffer[bufferSize];

		int size = maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		// If there was an error, return empty string.
		if (size < 0 || size > bufferSize)
		{
			return "";
		}

		return buffer;
	}

	/**
	 * Extract a file system bundle to a directory in the
	 * local file system on the device/emulator.
	 * @param handle The resource handle of the bundled
	 * file system. Bundles are created with the Bundle tool
	 * that comes with MoSync.
	 * @param destinationPath The path of the directory where
	 * the file system will be extracted.
	 */
	bool FileUtil::extractFileSystem(
		MAHandle handle,
		const MAUtil::String& destinationPath)
	{
		setCurrentFileSystem(handle, 0);
		int result = MAFS_extractCurrentFileSystem(destinationPath.c_str());
		freeCurrentFileSystem();
		return 1 == result;
	}

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
	bool FileUtil::extractLocalFiles()
	{
		return extractFileSystem(1, getLocalPath());
	}

	/**
	 * Get the checksum of a file bundle.
	 * @param handle The resource handle of the bundled
	 * file system. Bundles are created with the Bundle tool
	 * that comes with MoSync.
	 * @return A 32-bit checksum.
	 */
	int FileUtil::getFileSystemChecksum(MAHandle handle)
	{
		return MAFS_getFileSystemChecksum(handle);
	}

	/**
	 * Open a file for writing.
	 * Create the file if it does not exist.
	 * Note: Will truncate the file if it exists.
	 * @return Handle to the open file, <0 on error.
	 */
	MAHandle FileUtil::openFileForWriting(const MAUtil::String& filePath)
	{
		MAHandle file = maFileOpen(filePath.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return -1;
		}

		if (maFileExists(file))
		{
			// If the file exists, truncate it to zero size.
			// We do this to prevent problems with old data
			// at the end of the file if the new file is
			// shorter than the old file.
			maFileTruncate(file, 0);
		}
		else
		{
			// If the file does not exist, create it.
			int result = maFileCreate(file);
			if (result < 0)
			{
				return -1;
			}
		}

		return file;
	}

	/**
	 * Open a file for reading.
	 * @return Handle to the open file, <0 on error.
	 */
	MAHandle FileUtil::openFileForReading(const MAUtil::String& filePath)
	{
		MAHandle file = maFileOpen(filePath.c_str(), MA_ACCESS_READ);
		if (file < 0)
		{
			return -1;
		}

		if (!maFileExists(file))
		{
			return -1;
		}

		return file;
	}

	/**
	 * Write a data object to a file.
	 * @return true on success, false on error.
	 */
	bool FileUtil::writeDataToFile(
		const MAUtil::String& filePath,
		MAHandle outData)
	{
		MAHandle file = openFileForWriting(filePath);
		if (file < 0)
		{
			return false;
		}

		int result = maFileWriteFromData(
			file,
			outData,
			0,
			maGetDataSize(outData));

		maFileClose(file);

		return result == 0;
	}

	/**
	 * Write a text string to a file.
	 * @return true on success, false on error.
	 */
	bool FileUtil::writeTextToFile(
		const MAUtil::String& filePath,
		const MAUtil::String& outText)
	{
		MAHandle file = openFileForWriting(filePath);
		if (file < 0)
		{
			return false;
		}

		int result = maFileWrite(file, outText.c_str(), outText.length());

		maFileClose(file);

		return result == 0;
	}

	/**
	 * Read a data object from a file.
	 * @param filePath Full path of file to read.
	 * @param inPlaceholder Placeholder handle for data object.
	 * @return true on success, false on error.
	 */
	bool FileUtil::readDataFromFile(
		const MAUtil::String& filePath,
		MAHandle inPlaceholder)
	{

		MAHandle file = openFileForReading(filePath);
		if (file < 0)
		{
			return false;
		}

		int size = maFileSize(file);
		if (size < 1)
		{
			return false;
		}

		int result = maCreateData(inPlaceholder, size);
		if (RES_OK != result)
		{
			return false;
		}

		result = maFileReadToData(file, inPlaceholder, 0, size);

		maFileClose(file);

		return result == 0;
	}

	/**
	 * Read a text string from a file.
	 * @return true on success, false on error.
	 */
	bool FileUtil::readTextFromFile(
		const MAUtil::String& filePath,
		MAUtil::String& inText)
	{
		MAHandle file = openFileForReading(filePath);
		if (file < 0)
		{
			return false;
		}

		int size = maFileSize(file);
		if (size < 1)
		{
			return false;
		}

		// Allocate buffer with space for a null termination character.
		char* buffer = (char*) malloc(sizeof(char) * (size + 1));

		int result = maFileRead(file, buffer, size);

		maFileClose(file);

		buffer[size] = 0;
		inText = buffer;

		return result == 0;
	}

	/**
	 * Create a text string from a handle.
	 */
	MAUtil::String FileUtil::createTextFromHandle(MAHandle handle)
	{
		// Get size of data.
		int size = maGetDataSize(handle);

		// Allocate space for text plus zero termination character.
		char* tempText = (char*) malloc(size + 1);
		if (NULL == tempText)
		{
			return "";
		}

		// Read text data from handle.
		maReadData(handle, tempText, 0, size);

		// Zero terminate string.
		tempText[size] = 0;

		// Create String object.
		MAUtil::String text = tempText;

		// Free temporary text.
		free(tempText);

		// Return text object.
		return text;
	}

} // namespace
