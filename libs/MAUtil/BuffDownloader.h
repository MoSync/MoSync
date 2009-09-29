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

/** \file BuffDownloader.h
 *
 * \brief A class facilitating simple, asynchronous HTTP download to main memory.
 *
 * \author Ann-Sofie Lindblom
 *
 */

#ifndef _SE_MSAB_MAUTIL_BUFFDOWNLOADER_H_
#define _SE_MSAB_MAUTIL_BUFFDOWNLOADER_H_

#include <MAUtil/Connection.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>

namespace MAUtil {

	class BuffDownloader;

	/**
	* \brief A listener for events from the BuffDownloader class.
	*/
	class BuffDownloadListener
	{
	public:
		/**
		* Called when some data has been downloaded to notify about the progress of the download.
		* \param downloader A pointer to the downloader sending this event.
		* \param downloadedBytes The amount of bytes downloaded.
		* \param totalBytes The amount of bytes to be downloaded.
		*/
		virtual void notifyProgress(BuffDownloader* downloader, int downloadedBytes, int totalBytes);

		/**
		* Called when all data has been downloaded.
		* \param downloader A pointer to the downloader sending this event.
		* \param data C string containing the downloaded data.
		*/
		virtual void finishedDownloading(BuffDownloader* downloader, char* data) = 0;

		/**
		* Called if the download has been cancelled.
		* \param downloader A pointer to the downloader sending this event.
		*/
		virtual void downloadCancelled(BuffDownloader* downloader) = 0;

		/**
		* Called if an error occured.
		* \param downloader A pointer to the downloader sending this event.
		* \param code The error code describing the error. See \link #CONNERR_GENERIC CONNERR \endlink.
		*/
		virtual void error(BuffDownloader* downloader, int code) = 0;
	};


	/**
	* \brief Useful to simplify asynchronous downloading of data to a memory buffer.
	*/
	class BuffDownloader : public HttpConnectionListener
	{
	public:
		/**
		* The constructor.
		*/
		BuffDownloader();

		/**
		* The destructor.
		*/
		virtual ~BuffDownloader();

		/**
		* Registers a BuffDownloadListener, to which all events for this BuffDownloader is distributed.
		* \param bdl Pointer to the BuffDownloadListener instance.
		*/
		virtual void addBuffDownloadListener(BuffDownloadListener *bdl);

		/**
		* Begins downloading a file.
		* \param url C string containing a null terminated url to the file.
		* \return \>0 on success, or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
		*/
		virtual int beginDownloading(const char *url);

		/**
		* Cancels the current download.
		*/
		void cancelDownloading();

		/**
		* Indicates whether the BuffDownloader is currently downloading something.
		* \return True if the function is currently downloading, false otherwise.
		*/
		bool isDownloading() const { return mDownloading; }

	protected:
		// Should return the final product of the download.
		virtual char* getData();
		void fireFinishedDownloading(char* data);
		void fireError(int code);
		void connRecvFinished(Connection* conn, int result);
		virtual void httpFinished(HttpConnection* http, int result);

		HttpConnection *mConn;
		bool mDownloading;
		char* mData;
		int mDataOffset;
		int mContentLength;
		Vector<BuffDownloadListener*> mBuffDownloadListeners;
	};

}

#endif /* _SE_MSAB_MAUTIL_BUFFDOWNLOADER_H_ */
