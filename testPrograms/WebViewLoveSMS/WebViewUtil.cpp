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
 * @file WebViewUtil.cpp
 * @author Mikael Kindborg
 *
 * This file contains a utility library for working with WebViews.
 */

#include <ma.h>				// MoSync API
#include <maprofile.h>		// Profile database
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <MAUtil/String.h>	// C++ String class
#include <IX_WIDGET.h>		// Widget API
#include <conprint.h>
#include "WebViewUtil.h"

using namespace MoSync;

// ================= Class PlatformUtil =================

/**
 * Create a PlatformHandler for the current platform.
 * The caller has the responsibility of deallocating the
 * returned instance.
 */
Platform* Platform::create()
{
	Platform::checkNativeUISupport();

	if (Platform::isAndroid())
	{
		return new PlatformAndroid();
	}
	else if (Platform::isIOS())
	{
		return new PlatformIOS();
	}
	else
	{
		return NULL;
	}
}

/**
 * Error handling for devices that do not support NativeUI.
 * Here we throw a panic if NativeUI is not supported.
 */
void Platform::checkNativeUISupport()
{
	int widget = maWidgetCreate(MAW_WEB_VIEW);
	if (-1 == widget)
	{
		maPanic(0,
			"NativeUI is only available on Android and iPhone. "
			"You must run directly on the device or devices emulator.");
	}
	else
	{
		maWidgetDestroy(widget);
	}
}

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool Platform::isAndroid()
{
	return NULL != strstr(MA_PROF_STRING_PLATFORM, "android");
}

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool Platform::isIOS()
{
	// TODO: Find a proper way to detect iOS.
	return ! Platform::isAndroid();
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

// ================= Class Platform =================

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
 * Write a data object to a file.
 * @return true on success, false on error.
 */
bool Platform::writeDataToFile(
	const MAUtil::String& filePath,
	MAHandle outData)
{
	MAHandle file = openFileHelper(filePath, true);
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
	MAHandle file = openFileHelper(filePath, true);
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
 * @return true on success, false on error.
 */
bool Platform::readDataFromFile(
	const MAUtil::String& filePath,
	MAHandle inData)
{

	MAHandle file = openFileHelper(filePath, false);
	if (file < 0)
	{
		return false;
	}

	int size = maFileSize(file);
	if (size < 1)
	{
		return false;
	}

	int result = maFileReadToData(file, inData, 0, size);

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
	MAHandle file = openFileHelper(filePath, false);
	if (file < 0)
	{
		lprintfln("Platform::readTextFromFile: file < 0");
		return false;
	}

	int size = maFileSize(file);
	if (size < 1)
	{
		lprintfln("Platform::readTextFromFile: maFileSize size: %d", size);
		return false;
	}

	// Allocate buffer with space for a null termination character.
	char* buffer = (char*) malloc(sizeof(char) * (size + 1));

	int result = maFileRead(file, buffer, size);

	lprintfln("Platform::readTextFromFile: maFileRead result: %d", result);

	maFileClose(file);

	buffer[size] = 0;
	inText = buffer;

	return result == 0;
}

/**
 * Open a file for read/write access. Create the file if it does not exist.
 * @return Handle to the open file, <0 on error.
 */
MAHandle Platform::openFileHelper(
	const MAUtil::String& filePath,
	bool truncate)
{
	MAHandle file = maFileOpen(filePath.c_str(), MA_ACCESS_READ_WRITE);
	if (file < 0)
	{
		return -1;
	}

	if (maFileExists(file))
	{
		// If the file exists, truncate it to zero size.
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

// ================= Class PlatformUtilAndroid =================

/**
 * Constructor.
 */
PlatformAndroid::PlatformAndroid()
{
}

/**
 * Destructor.
 */
PlatformAndroid::~PlatformAndroid()
{
	// Nothing needs to be explicitly destroyed.
}

// ================= Class PlatformUtilIOS =================

/**
 * Constructor.
 */
PlatformIOS::PlatformIOS()
{
}

/**
 * Destructor.
 */
PlatformIOS::~PlatformIOS()
{
	// Nothing needs to be explicitly destroyed.
}

// ================= Class WebViewMessage =================

/**
 * Registers a url hook for the given web view.
 * @param webView The web view to get messages from.
 */
void WebViewMessage::getMessagesFor(MAWidgetHandle webView)
{
	maWidgetSetProperty(
		webView,
		MAW_WEB_VIEW_HARD_HOOK,
		"mosync://.*");
}

/**
 * Constructor.
 */
WebViewMessage::WebViewMessage(MAHandle dataHandle)
{
	if (NULL != dataHandle)
	{
		// Get length of the data, it is not zero terminated.
		int dataSize = maGetDataSize(dataHandle);

		// Allocate buffer for string data.
		char* stringData = (char*) malloc(dataSize + 1);

		// Get the data.
		maReadData(dataHandle, stringData, 0, dataSize);

		// Zero terminate.
		stringData[dataSize] = 0;

		// Set string data.
		char* p = stringData + strlen("mosync://");
		mMessageString = p;

		// Destroy string data.
		free(stringData);

		// Destroy the data handle
		maDestroyObject(dataHandle);
	}
}

/**
 * Destructor.
 */
WebViewMessage::~WebViewMessage()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Returns the message string.
 */
MAUtil::String WebViewMessage::getMessageString()
{
	return mMessageString.c_str();
}

/**
 * Checks if this message matches the given message name.
 */
bool WebViewMessage::is(const MAUtil::String& messageName)
{
	// Start of messageName should be found at start of message string.
	return 0 == mMessageString.find(messageName);
}

/**
 * Returns the parameter part of a message.
 */
MAUtil::String WebViewMessage::getParams()
{
	// Must be at least three characters in a message
	// that has a data part.
	if (mMessageString.length() < 3)
	{
		return "";
	}

	// Find first slash.
	int index = mMessageString.find("/");
	if (MAUtil::String::npos == index)
	{
		return "";
	}

	// Return the substring after the slash.
	return mMessageString.substr(index + 1);
}

/**
 * Returns a message parameter by index.
 * Parameters are separated by slashes.
 */
MAUtil::String WebViewMessage::getParam(int index)
{
	// Get params.
	MAUtil::String params = getParams();

	lprintfln("getParam params: %s", params.c_str());

	// Find start slash of the param we look for.
	int start = 0;
	for (int i = 0; i < index; ++i)
	{
		start = params.find("/", start);
		if (MAUtil::String::npos == start)
		{
			// Param not found.
			return "";
		}
		// Move to position after slash.
		start = start + 1;
	}

	// Is this the last param?
	int end = params.find("/", start);
	if (MAUtil::String::npos == end)
	{
		// Yes, last param, return rest of the string.
		return params.substr(start);
	}
	else
	{
		// No, not last param, return param part of the string.
		return params.substr(start, end);
	}
}
