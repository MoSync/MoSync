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
 * Utility library for working with files.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <MAUtil/String.h>	// C++ String class
#include <conprint.h>
#include "FileUtil.h"

using namespace MoSync;

// ================= Class Platform =================

/**
 * Create a Platform instance.
 */
Platform* Platform::create()
{
	return new Platform();
}


/**
 * Constructor.
 */
Platform::Platform()
{
}

/**
 * Destructor.
 */
Platform::~Platform()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Get the path to the local file system.
 * @return Path that ends with a slash.
 */
MAUtil::String Platform::getLocalPath()
{
	int bufferSize = 512;
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
 * Get the path to the SD card on the Android file system.
 * @return Path that ends with a slash.
 */
MAUtil::String Platform::getAndroidSDCardPath()
{
	return "/sdcard/";
}

/**
 * Write a data object to a file.
 * @return true on success, false on error.
 */
bool Platform::writeDataToFile(
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
bool Platform::writeTextToFile(
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
bool Platform::readDataFromFile(
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
bool Platform::readTextFromFile(
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
		lprintfln("Platform::readTextFromFile: size < 1");
		return false;
	}

	// Allocate buffer with space for a null termination character.
	char* buffer = (char*) malloc(sizeof(char) * (size + 1));

	int result = maFileRead(file, buffer, size);
	lprintfln("Platform::readTextFromFile: maFileRead result: %i", result);
	if (0 == result)
	{
		// Success, copy string data.
		buffer[size] = 0;
		inText = buffer;
	}

	free(buffer);

	maFileClose(file);

	return result == 0;
}

/**
 * Open a file for write (and read) access.
 * Create the file if it does not exist.
 * Note: Will truncate the file if it exists.
 * @return Handle to the open file, <0 on error.
 */
MAHandle Platform::openFileForWriting(const MAUtil::String& filePath)
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
 * Open a file for read access.
 * @return Handle to the open file, <0 on error.
 */
MAHandle Platform::openFileForReading(const MAUtil::String& filePath)
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
 * Create a text string from a handle.
 */
MAUtil::String Platform::createTextFromHandle(MAHandle handle)
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
