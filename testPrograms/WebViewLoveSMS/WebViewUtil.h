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
 * @file WebViewUtil.h
 * @author Mikael Kindborg
 *
 * This file contains a utility library for working with WebViews.
 */

#ifndef MOSYNC_UI_WEBVIEWUTIL_H_
#define MOSYNC_UI_WEBVIEWUTIL_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>

namespace MoSync {
namespace UI {

/**
 * Class that contains utility methods.
 */
class PlatformHandler
{
public:
	/**
	 * Create a PlatformHandler for the current platform.
	 */
	static PlatformHandler* create();

	/**
	 * Error handling for devices that do not support NativeUI.
	 * Here we throw a panic if NativeUI is not supported.
	 */
	static void checkNativeUISupport();

	/**
	 * Detects if the current platform is Android.
	 * @return true if the platform is Android, false otherwise.
	 */
	static bool isAndroid();

	/**
	 * Detects if the current platform is iOS.
	 * @return true if the platform is iOS, false otherwise.
	 */
	static bool isIOS();

	/**
	 * Constructor.
	 */
	PlatformHandler();

	/**
	 * Destructor.
	 */
	virtual ~PlatformHandler();
};

/**
 * Class that contains Android platform utility methods.
 */
class PlatformHandlerAndroid : public PlatformHandler
{
public:
	/**
	 * Constructor.
	 */
	PlatformHandlerAndroid();

	/**
	 * Destructor.
	 */
	virtual ~PlatformHandlerAndroid();
};

/**
 * Class that contains iOS platform utility methods.
 */
class PlatformHandlerIOS : public PlatformHandler
{
public:
	/**
	 * Constructor.
	 */
	PlatformHandlerIOS();

	/**
	 * Destructor.
	 */
	virtual ~PlatformHandlerIOS();
};

/**
 * Class that handles data access.
 */
class DataHandler
{
	/**
	 * Return path to local files directory. Path ends with a slash.
	 */
	virtual MAUtil::String getLocalFileSystemPath();

	/**
	 * Write a data object to a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeDataToFile(
		const MAUtil::String& fileName,
		MAHandle outData);

	/**
	 * Write a text string to a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeTextToFile(
		const MAUtil::String& fileName,
		const MAUtil::String& outText);


	/**
	 * Read a data object from a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeDataToFile(
		const MAUtil::String& fileName,
		MAHandle inData);

	/**
	 * Read a text string from a file.
	 * @return true on success, false on error.
	 */
	virtual bool writeTextToFile(
		const MAUtil::String& fileName,
		const MAUtil::String& inText);

	/**
	 * Create a text string from data handle.
	 */
	MAUtil::String createTextFromDataHandle(MAHandle data);
};

/**
 * Class that reads and parses messages in the form of urls
 * sent from a web view as MAW_EVENT_WEB_VIEW_HOOK_INVOKED events.
 *
 * Message (urls) used with this class has the format:
 *
 *   mosync://MessageName/Param1/Param2/...
 *
 * Example JavaScript call (with no parameters):
 *
 *   document.location = "mosync://GetGeoLocation";
 *
 * To receive messages from a web from do an initial call to:
 *
 *   WebViewUtil::getMessagesFor(webView);
 */
class WebViewMessage
{
private:
	/**
	 * The message string.
	 */
	MAUtil::String mMessageString;

public:
	/**
	 * Registers a url hook for the given web view.
	 * @param webView The web view to get messages from.
	 */
	static void getMessagesFor(MAWidgetHandle webView);

public:
	/**
	 * Constructor.
	 */
	WebViewMessage(MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~WebViewMessage();

	/**
	 * Returns the message string.
	 */
	MAUtil::String getMessageString();

	/**
	 * Checks if this message matches the given message name.
	 */
	bool is(const MAUtil::String& messageName);

	/**
	 * Returns the parameter part of a message.
	 */
	MAUtil::String getParams();

	/**
	 * Returns a message parameter by index.
	 * Parameters are separated by slashes.
	 */
	MAUtil::String getParam(int index);
};

} // namespace UI
} // namespace MoSync

#endif
