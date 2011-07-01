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

#include "WebViewUtil.h"

using namespace MoSync::UI;

// ================= Class PlatformUtil =================

/**
 * Error handling for devices that do not support NativeUI.
 * Here we throw a panic if NativeUI is not supported.
 */
void PlatformUtil::checkNativeUISupport()
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
bool PlatformUtil::isAndroid()
{
	return NULL != strstr(MA_PROF_STRING_PLATFORM, "android");
}

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool PlatformUtil::isIOS()
{
	// TODO: Find a proper way to detect iOS.
	return ! PlatformUtil::isAndroid();
}

/**
 * Helper method that reads a text string from data handle.
 */
MAUtil::String PlatformUtil::getTextFromDataHandle(MAHandle data)
{
	// Get size of data.
    int size = maGetDataSize(data);

    // Allocate space for text plus zero termination character.
    char* text = (char*) malloc(size + 1);
    if (NULL == text)
    {
    	return "";
    }

    // Read data.
    maReadData(data, text, 0, size);

    // Zero terminate string.
    text[size] = 0;

    MAUtil::String textString = text;

    free(text);

    return textString;
}

/**
 * Helper method that writes a text string to a file.
 */
void PlatformUtil::writeTextToFile(
	const MAUtil::String& filePath,
	const MAUtil::String& text)
{
	MAHandle file = maFileOpen(filePath.c_str(), MA_ACCESS_READ_WRITE);
	if (! maFileExists(file))
	{
		maFileCreate(file);
	}

	maFileWrite(file, text.c_str(), text.length());

	maFileClose(file);
}

/**
 * Constructor.
 */
PlatformUtil::PlatformUtil()
{
}

/**
 * Destructor.
 */
PlatformUtil::~PlatformUtil()
{
	// Nothing needs to be explicitly destroyed.
}

// ================= Class PlatformUtilAndroid =================

/**
 * Constructor.
 */
PlatformUtilAndroid::PlatformUtilAndroid()
{
	char path[1024];

	MAWidgetHandle widget = maWidgetCreate(MAW_WEB_VIEW);
	if (widget < 0)
	{
		maPanic(0, "Could not create web view in PlatformUtilAndroid");
	}

	int result = maWidgetGetProperty(
		widget,
		"localFilesDirectory",
		path,
		1024);
	if (result > 0)
	{
		mLocalFilesDirectory = path;
	}
	else
	{
		char message[128];
		sprintf(message, "Could not get localFilesDirectory, result: %d", result);
		maPanic(0, message);
	}

	maWidgetDestroy(widget);
}

/**
 * Destructor.
 */
PlatformUtilAndroid::~PlatformUtilAndroid()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Get the path to the local file directory on Android.
 */
MAUtil::String PlatformUtilAndroid::getLocalFileDirectory()
{
	return mLocalFilesDirectory;
}

/**
 * Helper method that writes a text string to a local file.
 */
void PlatformUtilAndroid::writeTextToLocalFile(
	const MAUtil::String& fileName,
	const MAUtil::String& text)
{
	PlatformUtil::writeTextToFile(
		getLocalFileDirectory() + fileName,
		text);
}

// ================= Class PlatformUtilIOS =================

/**
 * Constructor.
 */
PlatformUtilIOS::PlatformUtilIOS()
{
}

/**
 * Destructor.
 */
PlatformUtilIOS::~PlatformUtilIOS()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Helper method that writes a text string to a local file.
 */
void PlatformUtilIOS::writeTextToLocalFile(
	const MAUtil::String& fileName,
	const MAUtil::String& text)
{
	// TODO: Implement.
	maPanic(0, "PlatformUtilIOS::writeTextToLocalFile() is not implemented.");
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
		MAW_WEB_VIEW_URL_HOOK_PATTERN,
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
 * Returns the data part of a message.
 */
MAUtil::String WebViewMessage::getData()
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
 */
MAUtil::String WebViewMessage::getParam(int index)
{
	// Get params.
	MAUtil::String params = getData();

	// Find start slash of the param we look for.
	int start = 0;
	for (int i = 0; i < index; ++i)
	{
		start = mMessageString.find("/", start);
		if (MAUtil::String::npos == start)
		{
			// Param not found.
			return "";
		}
	}

	// Is this the last param?
	int end = mMessageString.find("/", start);
	if (MAUtil::String::npos == end)
	{
		// Yes, last param, return rest of the string.
		return mMessageString.substr(start + 1);
	}
	else
	{
		// No, not last param, return param part of the string.
		return mMessageString.substr(start + 1, end);
	}
}
