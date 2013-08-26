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
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * The PhotoGallery application lets you take pictures using
 * the device camera and upload them to a server. It also lets
 * you view the most recent pictures uploaded to the server.
 *
 * The main purpose is to demonstrate file upload, but the
 * program illustrates several other programming techniques.
 *
 * This application demonstrates the following functionality:
 *
 * - Take pictures using the JavaScript Capture API
 * - Upload of files using the FileTransfer.upload() JavaScript API
 * - Saving uploaded files on a web server using a PHP script.
 * - Downloading data from a server, using a custom function added
 *   to JavaScript.
 * - A simplified way of adding custom JavaScript message handling
 *   functions in C++.
 * - How to send messages between WebViews from JavaScript.
 *
 * The folder "Server" contains the server side code. Note that this
 * code is not part of the actual mobile application, but is intended
 * to run on a web server.
 */

#include <conprint.h>
#include <Wormhole/HybridMoblet.h>
#include <Wormhole/HighLevelTextDownloader.h>
#include "MAHeaders.h"

using namespace Wormhole;

/**
 * Helper class for downloading a list of photos from a server.
 * This downloader will delete itself upon completion.
 */
class MyPhotoListDownloader : public HighLevelTextDownloader
{
public:
	MAWidgetHandle mGalleryWebViewHandle;

	MyPhotoListDownloader(MAWidgetHandle galleryWebViewHandle) :
		mGalleryWebViewHandle(galleryWebViewHandle)
	{
	}

	virtual ~MyPhotoListDownloader()
	{
	}

	/**
	 * Called when download is complete.
	 * @param text Contains JSON with list of image urls, NULL on error.
	 */
	virtual void onDownloadComplete(char* text)
	{
		// URL array.
		MAUtil::String result;

		if (NULL != text)
		{
			// Download success, set result string (text contains a JSON array).
			result = "'";
			result += text;
			result += "'";

			// We need to free downloaded data.
			HighLevelTextDownloader::freeData(text);
		}
		else
		{
			// Download error.
			result = "null";
		}

		// Call JavaScript with the result.
		MAUtil::String url = "javascript:gallery.setPhotoList(";
		url += result;
		url += ")";

		maWidgetSetProperty(
			mGalleryWebViewHandle,
			MAW_WEB_VIEW_URL,
			url.c_str());

		// Delete me!
		delete this;
	}
};

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet
{
public:
	MyMoblet()
	{
		// Show the page with NativeUI definitions.
		showNativeUI("index.html");

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		setBeepSound(BEEP_WAV);

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"DownloadPhotoList",
			(FunTable::MessageHandlerFun)&MyMoblet::downloadPhotoList);
	}

	virtual ~MyMoblet()
	{
	}

	/**
	 * Called from JavaScript to download a list of photos.
	 */
	void downloadPhotoList(Wormhole::MessageStream& stream)
	{
		// Get the Gallery WebView widget handle.
		int webViewHandle = MAUtil::stringToInteger(stream.getNext());

		// Get download url.
		const char* url = stream.getNext();

		// Initiate download. The downloader will delete itself.
		(new MyPhotoListDownloader(webViewHandle))->get(url);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new MyMoblet())->enterEventLoop();
	return 0;
}
