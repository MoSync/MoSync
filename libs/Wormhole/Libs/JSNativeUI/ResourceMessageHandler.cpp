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
 * @file ResourceMessageHandler.cpp
 * @author Ali Sarrafi
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#include <mastdlib.h> // C string conversion functions
#include <conprint.h>
#include "ResourceMessageHandler.h"

// NameSpaces we want to access.
using namespace MAUtil; // Class Moblet, String
using namespace NativeUI; // WebView widget

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	ResourceMessageHandler::ResourceMessageHandler(NativeUI::WebView* webView) :
		mWebView(webView)
	{
		// A new instance of ImageDownloader is created.
		mImageDownloader = new ImageDownloader();
		mImageDownloader->addDownloadListener(this);
	}

	/**
	 * Destructor.
	 */
	ResourceMessageHandler::~ResourceMessageHandler()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Implementation of standard API exposed to JavaScript
	 * This function is used to detect different messages from JavaScript
	 * and call the respective function in MoSync.
	 *
	 * @return true if stream was handled, false if not.
	 */
	bool ResourceMessageHandler::handleMessage(Wormhole::MessageStream& stream)
	{

		char buffer[128];
		const char * action = stream.getNext();
		if(0 == strcmp("loadImage", action))
		{
			const char *imagePath = stream.getNext();
			const char* imageID = stream.getNext();
			//Call for loading an Image resource
			MAHandle imageHandle =
					loadImageResource(imagePath);

			sprintf(buffer,
					"mosync.resource.imageLoaded(\"%s\", %d)",
					imageID,
					imageHandle);
			mWebView->callJS(buffer);
		}
		else if(0 == strcmp("loadRemoteImage", action))
		{
			const char* imageURL = stream.getNext();
			const char* imageID = stream.getNext();

			MAHandle imageHandle = maCreatePlaceholder();
			mImageDownloader->beginDownloading(imageURL,imageHandle);

			sprintf(buffer,
					"mosync.resource.imageDownloadStarted(\"%s\", %d)",
					imageID,
					imageHandle);
			mWebView->callJS(buffer);
		}

		return true;
	}

	/**
	 * Loads an image from a file and returns the handle to it.
	 *
	 * @param imagePath relative path to the image file.
	 */
	MAHandle ResourceMessageHandler::loadImageResource(const char *imagePath)
	{

		int bufferSize = 1024;
		char buffer[bufferSize];

		//Get the local path which is the same path as the root of HTML apps
		maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		//Construct a full path by concatenating the relative path and local path
		char completePath[2048];

		sprintf(completePath,
				"%s%s",
				buffer,
				imagePath);

		//Load the image and create a data handle from it
		MAHandle imageFile = maFileOpen(completePath, MA_ACCESS_READ);

		int fileSize = maFileSize(imageFile);

		MAHandle fileData = maCreatePlaceholder();

		int res = maCreateData(fileData, fileSize);

		res = maFileReadToData(imageFile, fileData, 0, fileSize);
		maFileClose(imageFile);

		MAHandle imageHandle = maCreatePlaceholder();

		res = maCreateImageFromData(
				imageHandle,
				fileData,
				0,
				maGetDataSize(fileData));
		maDestroyObject(fileData);
		//return the handle to the loaded image
		return imageHandle;
	}


	/**
	 * Is called if the downloads is canceled.
	 */
	void ResourceMessageHandler::downloadCancelled(Downloader* downloader)
	{
		//Cancled
	}

	/**
	 * Called if an error occurs when downloading
	 */
	void ResourceMessageHandler::error(Downloader* downloader, int code)
	{
		//Cancled
	}

	/**
	 * On successful download completion, send the event to the JavaScript side.
	 */
	void ResourceMessageHandler::finishedDownloading(Downloader* downloader,
			MAHandle data) {
		char buffer[256];
		sprintf(buffer, "mosync.resource.imageDownloadFinished(%d)", data);
		mWebView->callJS(buffer);

	}
} // namespace
