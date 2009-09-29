/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file Downloader.h
 *
 * \brief Classes facilitating simple, asynchronous HTTP downloads to resources.
 *
 * \author Patrick Broman, Niklas Nummelin
 *
 */

#ifndef _SE_MSAB_MAUTIL_DOWNLOADER_H_
#define _SE_MSAB_MAUTIL_DOWNLOADER_H_

#include <MAUtil/Connection.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>

/// The system has run out of memory.
#define CONNERR_DOWNLOADER_OOM (CONNERR_USER - 1)

/**
* Use values below this for your own error codes when you inherit the Downloader class.
* \see #CONNERR_USER
*/
#define CONNERR_DOWNLOADER_USER (CONNERR_USER * 2)

namespace MAUtil {

	class Downloader;

	/**
	* \brief A listener for events from the Downloader class.
	*/
	class DownloadListener
	{
	public:
		/**
		* Called when some data has been downloaded to notify about the progress of the download.
		* \param downloader A pointer to the downloader sending this event.
		* \param downloadedBytes The amount of bytes downloaded.
		* \param totalBytes The amount of bytes to be downloaded.
		*/
		virtual void notifyProgress(Downloader* downloader, int downloadedBytes, int totalBytes);

		/**
		* Called if the Downloader runs out of memory.
		* Try to free some resource memory.
		* If you managed to free some, return true to make the Downloader try again.
		* If you can't free any more, return false to make the Downloader fail with
		* #CONNERR_DOWNLOADER_OOM.
		* The default implementation does nothing and returns false.
		*/
		virtual bool outOfMemory(Downloader* downloader);	//todo: call

		/**
		* Called when all data has been downloaded.
		* \param downloader A pointer to the downloader sending this event.
		* \param data MAHandle to the binary data resource with the downloaded data.
		*/
		virtual void finishedDownloading(Downloader* downloader, MAHandle data) = 0;

		/**
		* Called if the download has been cancelled.
		* \param downloader A pointer to the downloader sending this event.
		*/
		virtual void downloadCancelled(Downloader* downloader) = 0;

		/**
		* Called if an error occured.
		* \param downloader A pointer to the downloader sending this event.
		* \param code The error code describing the error. See \link #CONNERR_GENERIC CONNERR \endlink.
		*/
		virtual void error(Downloader* downloader, int code) = 0;
	};


	/**
	* \brief The Downloader class. Use it to simplify asynchronous downloading of files to binary resources.
	*/
	class Downloader : public HttpConnectionListener
	{
	public:
		/**
		* The constructor.
		*/
		Downloader();

		/**
		* The destructor.
		*/
		virtual ~Downloader();

		/**
		* Function to register a DownloadListener, to which all events for this Downloader is distributed.
		* \param dl Pointer to the DownloadListener instance.
		*/
		void addDownloadListener(DownloadListener *dl);

		void removeDownloadListener(DownloadListener *dl);

		/**
		* Function to begin downloading a file.
		* \param url C string containing a null terminated url to the file.
		* \param placeholder Optional argument to specify a placeholder to use for the binary resource of the downloaded data. 
		* \return \>0 on success, or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
		*/
		virtual int beginDownloading(const char *url, MAHandle placeholder=0);

		/**
		* Function to cancel the current download.
		*/
		virtual void cancelDownloading();

		/**
		* Function to retrieve if the Downloader is currently downloading something.
		* \return a boolean value, if true the function is currently downloading otherwise it isn't.
		*/
		bool isDownloading() const { return mIsDownloading; }

	protected:
		// Should return a handle to the final product of the download.
		// May return zero if a handle could not be obtained.
		virtual MAHandle getHandle();
		void fireFinishedDownloading(MAHandle data);
		void fireError(int code);
		void connRecvFinished(Connection* conn, int result);
		virtual void httpFinished(HttpConnection* http, int result);

		HttpConnection *mConn;
		bool mIsDownloading;
		bool mUserData;
		MAHandle mData;
		int mDataOffset;
		int mContentLength;
		Vector<DownloadListener*> mDownloadListeners;
	};

	/**
	* \brief The ImageDownloader class. Use it to simplify asynchronous downloading of images to image resources.
	*/
	class ImageDownloader : public Downloader {
	public:

		virtual ~ImageDownloader();

		/**
		* \see Downloader::beginDownloading(const char*, MAHandle);
		*/
		int beginDownloading(const char *url, MAHandle placeholder=0);

		void cancelDownloading();
		
	protected:
		bool mUserPlaceholder;
		MAHandle mPlaceholder;
		MAHandle getHandle();
	};

	/**
	* \brief The AudioDownloader class. Use it to simplify asynchronous downloading of audio to audio resources.
	*/
	class AudioDownloader : public Downloader {
	public:
		/**
		* Same as Downloader::beginDownloading() with an additional optional parameter
		* to specify mime type of the downloaded file.
		* If neither the server or you provide a mime type, #CONNERR_NOHEADER will be thrown.
		* If both the server and you provide a mime type, by default the server's will be picked,
		* but you can force use of your mime type.
		*/
		int beginDownloading(const char *url, MAHandle placeholder=0,
			const char *mimeType=NULL, bool forceMyMime=false);

		virtual int beginDownloading(const char *url, MAHandle placeholder);

	protected:
		String mMimeType;
		bool mForce;

		virtual void httpFinished(HttpConnection* http, int result);
	};
}

#endif /* _SE_MSAB_MAUTIL_DOWNLOADER_H_ */
