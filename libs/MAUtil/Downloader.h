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

/** 
 * \file Downloader.h
 * \brief Simple asynchronous HTTP download to resources.
 * \author Patrick Broman and Niklas Nummelin.
 */
 
/*
Downloader state specification
------------------------------

This diagram shows the allowed state transitions for
a downloader and its listener. When having multiple
listeners, initiation of a new download from a listener
will cause a panic (beginDownloading can only be called
if a download is not in progress).

downloader.beginDownloading -> 
  precondition: closeConnection(NO_CLEANUP)
  downloader.beginDownloading -> Error
  downloader.cancelDownloading -> Ok
  
downloader.cancelDownloading ->
  precondition: closeConnection(CLEANUP)
  downloader.beginDownloading -> Ok
  downloader.cancelDownloading -> Error

listener.notifyProgress ->
  precondition: none
  downloader.beginDownloading -> Error
  downloader.cancelDownloading -> Ok

listener.finishedDownloading ->
  precondition: closeConnection(NO_CLEANUP)
  downloader.beginDownloading -> Ok
  downloader.cancelDownloading -> Error

listener.downloadCancelled ->
  precondition: closeConnection(CLEANUP)
  downloader.beginDownloading -> Ok
  downloader.cancelDownloading -> Error

listener.error ->
  precondition: closeConnection(CLEANUP)
  downloader.beginDownloading -> Ok
  downloader.cancelDownloading -> Error
*/

#ifndef _SE_MSAB_MAUTIL_DOWNLOADER_H_
#define _SE_MSAB_MAUTIL_DOWNLOADER_H_

#include "Connection.h"
#include "String.h"
#include "util.h"

/// The system has run out of memory.
#define CONNERR_DOWNLOADER_OOM (CONNERR_USER - 1)

/**
 * Use values below this for your own error codes when you inherit the Downloader class.
 * \see #CONNERR_USER
 */
#define CONNERR_DOWNLOADER_USER (CONNERR_USER * 2)

namespace MAUtil {

	// Forward declarations.
	class Downloader;
	class DownloaderReader;

	/**
	 * \brief A listener for events from the Downloader class.
	 */
	class DownloadListener
	{
	public:
		/**
		 * Called when some data has been downloaded to notify about the
		 * progress of the download.
		 * \param downloader A pointer to the downloader sending this event.
		 * \param downloadedBytes The amount of bytes downloaded.
		 * \param totalBytes The amount of bytes to be downloaded. Zero means
		 * that the total content length is unknown.
		 */
		virtual void notifyProgress(
			Downloader* downloader,
			int downloadedBytes,
			int totalBytes);

		/**
		 * Called if the Downloader runs out of memory.
		 * Try to free some resource memory.
		 * If you managed to free some, return true to make the Downloader try again.
		 * If you can't free any more, return false to make the Downloader fail with
		 * #CONNERR_DOWNLOADER_OOM.
		 * The default implementation does nothing and returns false.
		 * Note: This feature is not yet implemented, outOfMemory() is
		 * never called by download framework.
		 */
		virtual bool outOfMemory(Downloader* downloader);	//todo: call

		/**
		 * Called when all data has been downloaded.
		 * \param downloader A pointer to the downloader sending this event.
		 * \param data MAHandle to the binary data resource with the downloaded data.
		 * It is the responsibility of the application to free the data handle 
		 * once it is not needed (use maDestroyObject() for this). In case a
		 * handle was supplied to beginDownloading() the data parameter will refer
		 * to that handle.
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
		 * \param code The error code describing the error.
		 * See \link #CONNERR_GENERIC CONNERR \endlink.
		 */
		virtual void error(Downloader* downloader, int code) = 0;
	};

	/**
	 * \brief The Downloader class. Use it to simplify asynchronous downloading 
	 * of files to binary resources.
	 */
	class Downloader : public HttpConnectionListener
	{
		friend class DownloaderReader;
		friend class DownloaderReaderThatReadsChunks;
		friend class DownloaderReaderWithKnownContentLength;

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
		 * Function to register a DownloadListener, to which all events for
		 * this Downloader is distributed.
		 * \param dl Pointer to the DownloadListener instance.
		 */
		void addDownloadListener(DownloadListener *dl);

		/**
		 * Remove a DownloadListener.
		 * \param dl Pointer to the DownloadListener instance.
		 */
		void removeDownloadListener(DownloadListener *dl);

		/**
		 * Function to begin downloading a file.
		 * \param url C string containing a null terminated url to the file.
		 * \param placeholder Optional argument to specify a placeholder to
		 * use for the binary resource of the downloaded data.
		 * \return \>0 on success, or a \link #CONNERR_GENERIC CONNERR \endlink
		 * code \< 0 on failure.
		 */
		virtual int beginDownloading(const char *url, MAHandle placeholder=0);

		/**
		 * Function to cancel the current download.
		 * Do cleanup and send downloadCancelled to listeners.
		 */
		virtual void cancelDownloading();

		/**
		 * Function to retrieve if the Downloader is currently
		 * downloading something.
		 * \return a boolean value, if true the function is currently
		 * downloading otherwise it isn't.
		 */
		bool isDownloading() const { return mIsDownloading; }

	protected:
		/**
		 * Should return a handle to the final product of the download.
		 * May return zero if a handle could not be obtained.
		 * Do NOT call explicitly. Use the handle provided in
		 * finishedDownloading.
		 */
		virtual MAHandle getHandle();

		/**
		 * Helper method to get the data handle. Used by friend classes.
		 */
		MAHandle getDataPlaceholder();

		/**
		 * Send notifyProgress to listeners.
		 */
		void fireNotifyProgress(int dataOffset, int contentLength);

		/**
		 * Close connection and send finishedDownloading to listeners.
		 */
		void fireFinishedDownloading(MAHandle data);

		/**
		 * Do cleanup and send error to listeners.
		 */
		void fireError(int code);

		/**
		 * Close the connection used by the downloader.
		 * This is part of the normal finishing of the download,
		 * and is also done on errors and on cancelling the download.
		 * \param cleanup if 1 free downloaded resources, if 0 do not do
		 * free downloaded resources. Cleanup is done on errors and on 
		 * cancelling a download.
		 */
		virtual void closeConnection(int cleanup);

		/**
		 * Delete the reader object (this is the object that
		 * performs the download).
		 */
		void deleteReader();
		
		/**
		 * Callback method in HttpConnectionListener.
		 */
		virtual void httpFinished(HttpConnection* http, int result);

		/**
		 * Callback method.
		 */
		void connRecvFinished(Connection* conn, int result);

	protected:
		HttpConnection *mConn;
		bool mIsDownloading;
		bool mIsDataPlaceholderSystemAllocated;
		MAHandle mDataPlaceholder;
		Vector<DownloadListener*> mDownloadListeners;
		
		/**
		 * Object that performs the actual download. A downloader
		 * is configured with different readers depending on the
		 * how the server sends the data to the client.
		 */
		DownloaderReader* mReader;
	};

	/**
	 * \brief The ImageDownloader class. Use it to simplify asynchronous
	 * downloading of images to image resources.
	 */
	class ImageDownloader : public Downloader {
	public:

		ImageDownloader();
		virtual ~ImageDownloader();

		/**
		 * \see Downloader::beginDownloading(const char*, MAHandle);
		 */
		int beginDownloading(const char *url, MAHandle placeholder=0);
		
	protected:
		/**
		 * Return the image handle of the downloader.
		 * The caller of this method should fire an error to listeners.
		 * @return The image handle (> 0) if successful, 0 if there is 
		 * an error (out of memory).
		 */
		virtual MAHandle getHandle();
		
		virtual void closeConnection(int cleanup);

	protected:
		bool mIsImagePlaceholderSystemAllocated;
		bool mIsImageCreated;
		MAHandle mImagePlaceholder;
	};

	/**
	 * \brief The AudioDownloader class. Use it to simplify asynchronous
	 * downloading of audio to audio resources.
	 * Note that for AudioDownloader to work, the server must return a
	 * content-length header in the HTTP response.
	 */
	class AudioDownloader : public Downloader {
	public:
		/**
		 * Same as Downloader::beginDownloading() with an additional
		 * optional parameter to specify mime type of the downloaded file.
		 * If neither the server or you provide a mime type, #CONNERR_NOHEADER 
		 * will be thrown. If both the server and you provide a mime type, 
		 * by default the server's will be picked, but you can force use of 
		 * your mime type.
		 */
		int beginDownloading(
			const char *url,
			MAHandle placeholder=0,
			const char *mimeType=NULL,
			bool forceMime=false);

		virtual int beginDownloading(const char *url, MAHandle placeholder);

	protected:
		virtual void httpFinished(HttpConnection* http, int result);

	protected:
		String mMimeType;
		bool mForceMimeType; // Force user supplied mime type.
	};

	/**
	 * \brief Base class for classes that perform the download.
	 */
	class DownloaderReader
	{
	public:
		DownloaderReader(Downloader* downloader);
		virtual void startRecvToData(Connection* conn) = 0;
		virtual void connRecvFinished(Connection* conn, int result) = 0;
		int getContentLength();
	protected:
		Downloader* mDownloader;
		int mContentLength;
	};

	/**
	 * \brief Class that handles download when content-length is known.
	 */
	class DownloaderReaderWithKnownContentLength : public DownloaderReader
	{
	public:
		DownloaderReaderWithKnownContentLength(
			Downloader* downloader,
			int dataOffset,
			int contentLength);
		virtual void startRecvToData(Connection* conn);
		virtual void connRecvFinished(Connection* conn, int result);
	protected:
		int mDataOffset;
	};

	/**
	 * \brief Class that handles download when content-length is NOT known.
	 * Here we read in chunks until we get result CONNERR_CLOSED in
	 * connRecvFinished.
	 */
	class DownloaderReaderThatReadsChunks : public DownloaderReader
	{
	public:
		DownloaderReaderThatReadsChunks(Downloader* downloader);
		virtual ~DownloaderReaderThatReadsChunks();
		virtual void startRecvToData(Connection* conn);
		virtual void connRecvFinished(Connection* conn, int result);
	protected:
		bool readNextChunk(Connection* conn);
		void finishedDownloadingChunkedData();
	protected:
		MAUtil::Vector<MAHandle> mDataChunks;
		int mDataChunkSize;
		int mDataChunkOffset;
	};
}

#endif /* _SE_MSAB_MAUTIL_DOWNLOADER_H_ */
