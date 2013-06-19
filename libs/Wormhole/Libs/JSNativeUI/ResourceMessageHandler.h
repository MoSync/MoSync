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
 * @file ResourceMessageHandler.h
 * @author Ali Sarrafi
 *
 * Implementation of Resource calls made from JavaScript.
 * Used for downloading images and loading local images as
 * resources for UI from JavaScript.
 */

#ifndef WORMHOLE_LIBS_JSNATIVEUI_RESOURCEMESSAGEHANDLER_H_
#define WORMHOLE_LIBS_JSNATIVEUI_RESOURCEMESSAGEHANDLER_H_

//#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include <MAUtil/String.h>
#include <MAUtil/Downloader.h>
#include <mavsprintf.h>
#include "../../MessageStream.h"
#include "../../FileUtil.h"

namespace Wormhole
{
	class LogMessageListener
	{
		public:
			virtual void onLogMessage(
				const char* message,
				const char* url) = 0;
	};

	/**
	 * Class that implements JavaScript calls.
	 *
	 * The JavaScript side is in file extendedbridge.js.
	 */
	class ResourceMessageHandler:
		public MAUtil::DownloadListener
	{
	public:
		/**
		 * Constructor.
		 */
		ResourceMessageHandler(NativeUI::WebView* webView);

		/**
		 * Destructor.
		 */
		virtual ~ResourceMessageHandler();

		/**
		 * Get the FileUtil object to be used for File APIs.
		 */
		FileUtil* getFileUtil();

		/**
		 * Set the FileUtil object to be used for File APIs.
		 */
		void setFileUtil(FileUtil* fileUtil);

		/**
		 * Implementation of standard API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		virtual bool handleMessage(Wormhole::MessageStream& message);

		/**
		 * Is called if the downloads is canceled.
		 */
		virtual void downloadCancelled(MAUtil::Downloader* downloader);

		/**
		 * Called if an error occurs when downloading
		 */
		virtual void error(MAUtil::Downloader* downloader, int code);

		/**
		 * On successful download completion, send the
		 * event to the JavaScript side.
		 */
		virtual void finishedDownloading(
			MAUtil::Downloader* downloader,
			MAHandle data);

		/**
		 * Set the object to get notified when log messages are sent.
		 *
		 * Note that the ResourceMessageHandler will take ownership of
		 * the listener and delete it upon destruction. Also, when a new
		 * listener is set, the old listener will be deleted.
		 *
		 * @param listener The log message listener.
		 */
		virtual void setLogMessageListener(LogMessageListener* listener);

		/**
		 * Send a log message to a server.
		 * @param message The log message, for example: "Hello World".
		 * @param url The url to use for the remote logging service.
		 */
		virtual void sendRemoteLogMessage(
			const MAUtil::String& message,
			const MAUtil::String& url);

	protected:
		/**
		 * Loads an image from a file and returns the handle to it.
		 *
		 * @param imagePath relative path to the image file.
		 */
		virtual MAHandle loadImageResource(const char* imagePath);

		/**
		 * An instance of ImageDownloader that will be used for
		 * downloading image resources
		 */
		MAUtil::ImageDownloader* mImageDownloader;

		/**
		 * A Pointer to the main webview
		 * Used for communicating with NativeUI
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Listener that will be notified when log messages
		 * are sent from JavaScript.
		 */
		LogMessageListener* mLogMessageListener;

		/**
		 * File system helper object.
		 */
		FileUtil* mFileUtil;
	};
} // namespace

#endif
