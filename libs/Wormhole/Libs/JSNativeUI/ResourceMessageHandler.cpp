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
 * Implementation of Resource calls made from JavaScript.
 * Used for downloading images and loading local images as
 * resources for UI from JavaScript.
 */

#include <mastdlib.h>
#include <mastring.h>
#include <MAUtil/String.h>
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
		mWebView(webView),
		mLogMessageListener(NULL)
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
		if (NULL != mLogMessageListener)
		{
			delete mLogMessageListener;
		}
	}

	/**
	 * Get the FileUtil object to be used for File APIs.
	 */
	FileUtil* ResourceMessageHandler::getFileUtil()
	{
		return mFileUtil;
	}

	/**
	 * Set the FileUtil object to be used for File APIs.
	 */
	void ResourceMessageHandler::setFileUtil(FileUtil* fileUtil)
	{
		mFileUtil = fileUtil;
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
		char buffer[512];

		const char * action = stream.getNext();

		if (0 == strcmp("loadImage", action))
		{
			const char* imagePath = stream.getNext();
			const char* imageID = stream.getNext();

			// Load the Image resource.
			MAHandle imageHandle = loadImageResource(imagePath);
			if (imageHandle > 0)
			{
				sprintf(buffer,
						"mosync.resource.imageLoaded(\"%s\", %d)",
						imageID,
						imageHandle);
				mWebView->callJS(buffer);
			}
			else
			{
				// TODO: Better way to inform about the error?
				// Call JS function with error code?
				// mosync.resource.imageLoaded(<imageID>, -1) ??
				char errorMessage[1024];
				sprintf(errorMessage,
					"@@@ MoSync: ResourceMessageHandler could not load image: %s",
					imagePath);
				maWriteLog(errorMessage, strlen(errorMessage));
			}
		}
		else if (0 == strcmp("loadRemoteImage", action))
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
		else if (0 == strcmp("sendRemoteLogMessage", action))
		{
			const char* url = stream.getNext();
			const char* message = stream.getNext();
			sendRemoteLogMessage(message, url);
		}

		return true;
	}

	/**
	 * Loads an image from a file and returns the handle to it.
	 *
	 * @param imagePath relative path to the image file.
	 */
	MAHandle ResourceMessageHandler::loadImageResource(const char* imagePath)
	{
		if (!getFileUtil())
		{
			return 0;
		}

		// Get the current apllication directory path.
		String appPath = getFileUtil()->getAppPath();

		// Construct image path.
		char completePath[2048];
		sprintf(completePath,
				"%s%s",
				appPath.c_str(),
				imagePath);

		// Load the image and create a data handle from it.
		MAHandle imageFile = maFileOpen(completePath, MA_ACCESS_READ);
		if (imageFile < 0)
		{
			return 0;
		}

		int fileSize = maFileSize(imageFile);
		if (fileSize < 1)
		{
			return 0;
		}

		// Create buffer to hold file data.
		MAHandle fileData = maCreatePlaceholder();
		int result = maCreateData(fileData, fileSize);
		if (RES_OK != result)
		{
			maDestroyPlaceholder(fileData);
			return 0;
		}

		// Read data from file.
		result = maFileReadToData(imageFile, fileData, 0, fileSize);
		maFileClose(imageFile);
		if (result < 0)
		{
			maDestroyPlaceholder(fileData);
			return 0;
		}

		// Create image.
		MAHandle imageHandle = maCreatePlaceholder();
		result = maCreateImageFromData(
				imageHandle,
				fileData,
				0,
				maGetDataSize(fileData));
		maDestroyPlaceholder(fileData);
		if (RES_OK != result)
		{
			maDestroyPlaceholder(imageHandle);
			return 0;
		}

		// Return the handle to the loaded image.
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
	 * On successful download completion, send the
	 * event to the JavaScript side.
	 */
	void ResourceMessageHandler::finishedDownloading(
		Downloader* downloader,
		MAHandle data)
	{
		char buffer[256];
		sprintf(buffer, "mosync.resource.imageDownloadFinished(%d)", data);
		mWebView->callJS(buffer);
	}

	/**
	 * Set the object to get notified when log messages are sent.
	 *
	 * Note that the ResourceMessageHandler will take ownership of
	 * the listener and delete it upon destruction. Also, when a new
	 * listener is set, the old listener will be deleted.
	 *
	 * @param listener The log message listener.
	 */
	void ResourceMessageHandler::setLogMessageListener(
		LogMessageListener* listener)
	{
		if (NULL == mLogMessageListener)
		{
			delete mLogMessageListener;
			mLogMessageListener = NULL;
		}

		mLogMessageListener = listener;
	}

	/**
	 * Send a log message to a server.
	 * @param message The log message, for example: "Hello World".
	 * @param url The url to use for the remote logging service.
	 */
	void ResourceMessageHandler::sendRemoteLogMessage(
		const MAUtil::String& message,
		const MAUtil::String& url)
	{
		if (NULL != mLogMessageListener)
		{
			mLogMessageListener->onLogMessage(message.c_str(), url.c_str());
		}
	}
} // namespace
