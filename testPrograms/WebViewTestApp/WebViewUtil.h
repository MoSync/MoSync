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
class PlatformUtil
{
public:
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
	 * Create an instance of a PlatformUtil for the current platform.
	 */
	static PlatformUtil createPlatformUtil();

	/**
	 * Helper method that reads a text string from data handle.
	 */
	static MAUtil::String getTextFromDataHandle(MAHandle data);

	/**
	 * Helper method that writes a text string to a file.
	 */
	static void PlatformUtil::writeTextToFile(
		const MAUtil::String& filePath,
		const MAUtil::String& text);

	/**
	 * Constructor.
	 */
	PlatformUtil();

	/**
	 * Destructor.
	 */
	virtual ~PlatformUtil();

	/**
	 * Helper method that writes a text string to a local file.
	 */
	virtual void writeTextToLocalFile(
		const MAUtil::String& fileName,
		const MAUtil::String& text) = 0;
};

/**
 * Class that contains Android platform utility methods.
 */
class PlatformUtilAndroid : public PlatformUtil
{
private:
	/**
	 * The Android package name.
	 */
	MAUtil::String mLocalFilesDirectory;

public:
	/**
	 * Constructor.
	 */
	PlatformUtilAndroid();

	/**
	 * Destructor.
	 */
	virtual ~PlatformUtilAndroid();

	/**
	 * Helper method that writes a text string to a local file.
	 */
	virtual void writeTextToLocalFile(
		const MAUtil::String& fileName,
		const MAUtil::String& text);

private:
	/**
	 * Get the path to the local file directory.
	 */
	MAUtil::String getLocalFileDirectory();
};

/**
 * Class that contains iOS platform utility methods.
 */
class PlatformUtilIOS : public PlatformUtil
{
public:
	/**
	 * Constructor.
	 */
	PlatformUtilIOS();

	/**
	 * Destructor.
	 */
	virtual ~PlatformUtilIOS();

	/**
	 * Helper method that writes a text string to a local file.
	 */
	virtual void writeTextToLocalFile(
		const MAUtil::String& fileName,
		const MAUtil::String& text);
};

/**
 * Class that reads and parses custom messages from a web view.
 *
 * NOTE: By "messages" is meant MAW_EVENT_CUSTOM_MESSAGE events.
 *
 * Message used with this class has the format:
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
 *
 * Messages will then be delivered as MAW_EVENT_CUSTOM_MESSAGE events.
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
	 * Returns the data part of a message.
	 */
	MAUtil::String getData();

	/**
	 * Returns a message parameter by index.
	 */
	MAUtil::String getParam(int index);
};

} // namespace UI
} // namespace MoSync

#endif
