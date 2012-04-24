/*
Copyright (C) 2012 MoSync AB

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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file EasyHttpConnection.h
 * @author Mikael Kindborg
 *
 * @brief A high-level HTTP connection object.
 */

#ifndef WORMHOLE_EASY_HTTP_CONNECTION_H
#define WORMHOLE_EASY_HTTP_CONNECTION_H

#include <ma.h>
#include <maheap.h>
#include <MAUtil/Connection.h>

namespace Wormhole
{

// Constants.
#define EASY_HTTP_SUCCESS 1
#define EASY_HTTP_ERROR -1

// Forward declaration.
class EasyHttpConnection;

/**
 * \brief Base class for helper classes that handle the download.
 * We have only one such class right now, EasyReaderThatReadsChunks.
 */
class EasyReader
{
public:
	/**
	 * Constructor.
	 */
	EasyReader(EasyHttpConnection* connection);

	/**
	 * Start downloading data.
	 */
	virtual void startRecvToData() = 0;

	/**
	 * Called when the new data is available.
	 */
	virtual void connRecvFinished(int result) = 0;

	int getContentLength();

protected:
	/**
	 * The connection object served by this reader.
	 */
	EasyHttpConnection* mConnection;

	/**
	 * Total length of downloaded data (accumulated value).
	 */
	int mContentLength;
};

/**
 * \brief Class that handles download when content-length
 * is NOT known. Here we read in chunks until we get result
 * CONNERR_CLOSED in connRecvFinished.
 */
class EasyReaderThatReadsChunks : public EasyReader
{
public:
	/**
	 * Constructor.
	 */
	EasyReaderThatReadsChunks(EasyHttpConnection* downloader);

	/**
	 * Destructor.
	 */
	virtual ~EasyReaderThatReadsChunks();

	/**
	 * Start downloading data.
	 */
	virtual void startRecvToData();

	/**
	 * Called when the new data is available.
	 */
	virtual void connRecvFinished(int result);

protected:
	bool readNextChunk();
	void finishedDownloadingChunkedData();

protected:
	/**
	 * Vector with chunks used while downloading data.
	 */
	MAUtil::Vector<MAHandle> mDataChunks;

	/**
	 * Size of a chunk of data.
	 */
	int mDataChunkSize;

	/**
	 * Current location (write offset) in the current chunk.
	 */
	int mDataChunkOffset;
};

/**
 * A high-level HTTP connection object that is easier to use
 * than MAUTIL::HttpConnection. Has an integrated listener.
 * This class does not use the "content-length" HTTP header
 * and thus works also when this header is not set.
 *
 * Inherit this class and override dataDownloaded:
 *
 *   virtual void dataDownloaded(MAHandle data, int result);
 */
class EasyHttpConnection :
	public MAUtil::HttpConnection,
	public MAUtil::HttpConnectionListener
{
public:
	EasyHttpConnection();
	virtual ~EasyHttpConnection();

	/**
	 * This is the starting point of the JSON request.
	 * \return EASY_HTTP_SUCCESS if successful, EASY_HTTP_ERROR on error.
	 */
	int postJsonRequest(const char* url, const char* jsonData);

	/**
	 * This is the starting point of a GET request.
	 */
	int get(const char* url);

	/**
	 * Called by an EasyReader when download is successfully finished.
	 */
	void downloadSuccess(MAHandle handle);

	/**
	 * Called by an EasyReader when there is a download error.
	 */
	void downloadError(int result);

protected:
	/**
	 * Implement this method in a subclass of this class.
	 * Called when the HTTP connection has finished downloading data.
	 * \param data Handle to the data, will be 0 on error, > 0 on success.
	 * \param result Result code, RES_OK on success, otherwise an HTTP error code.
	 * The subclass takes ownership of this data and has the responsibility
	 * of deallocating the data.
	 */
	virtual void dataDownloaded(MAHandle data, int result) = 0;

	/**
	 * This method is called when the HTTP request is complete.
	 * Now all data is sent to the server and we can start reading
	 * the reply and download data.
	 */
	void httpFinished(MAUtil::HttpConnection* connection, int result);

	void connWriteFinished(MAUtil::Connection* connection, int result);

	void connRecvFinished(MAUtil::Connection* connection, int result);

	void ATTRIBUTE(noreturn, connReadFinished(MAUtil::Connection* connection, int result));

	void deallocateData();

	/**
	 * Delete the reader object (this is the object that
	 * performs the download).
	 */
	void deleteReader();

private:
	/**
	 * Object that performs the actual download.
	 */
	EasyReader* mReader;
};

} // namespace

#endif

 /*! @} */
