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
 * @file ResourceMessageHandler.h
 * @author Ali Sarrafi
 *
 * Implementation of Resource calls made from JavaScript.
 * Used for downloading images and loading local images as
 * resources for UI from JavaScript
 */

#ifndef RESOURCE_MESSAGE_HANDLER_H_
#define RESOURCE_MESSAGE_HANDLER_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include <MAUtil/String.h>
#include <MAUtil/Downloader.h>
#include <Wormhole/MessageStream.h>

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
	 * On successful download completion, send the event to the JavaScript side.
	 */
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data);

private:

	/**
	 * An instance of ImageDownloader that will be used for downloading image resources
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
};

#endif
