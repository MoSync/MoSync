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
#include "../../MessageStream.h"

namespace Wormhole
{
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
		 * Implementation of standard API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		bool handleMessage(Wormhole::MessageStream& message);

		/**
		 * Is called if the downloads is canceled.
		 */
		void downloadCancelled(MAUtil::Downloader* downloader);

		/**
		 * Called if an error occurs when downloading
		 */
		void error(MAUtil::Downloader* downloader, int code);

		/**
		 * On successful download completion, send the
		 * event to the JavaScript side.
		 */
		void finishedDownloading(
			MAUtil::Downloader* downloader,
			MAHandle data);

		/**
		 * Set the url to be used for remote log messages.
		 * @param url The url to use for the remote logging service,
		 * for example: "http://localhost:8282/log/"
		 */
		void setRemoteLogURL(const MAUtil::String& url);

		/**
		 * Send a log message to a server.
		 * @param url The url to use for the remote logging service.
		 * @param url The log message, for example: "Hello World".
		 */
		void sendRemoteLogMessage(
			const MAUtil::String& url,
			const MAUtil::String& message);

	private:

		/**
		 * An instance of ImageDownloader that will be used for
		 * downloading image resources
		 */
		MAUtil::ImageDownloader* mImageDownloader;

		/**
		 * Loads an image from a file and returns the handle to it.
		 *
		 * @param imagePath relative path to the image file.
		 */
		MAHandle loadImageResource(const char *imagePath);

		/**
		 * A Pointer to the main webview
		 * Used for communicating with NativeUI
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Address of url that will receive remote log messages.
		 */
		MAUtil::String mRemoteLogURL;
	};
} // namespace

#endif
