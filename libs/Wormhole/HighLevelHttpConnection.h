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
 * @file HighLevelHttpConnection.h
 * @author Mikael Kindborg
 *
 * @brief A high-level HTTP connection object.
 */

#ifndef WORMHOLE_HIGH_LEVEL_HTTP_CONNECTION_H
#define WORMHOLE_HIGH_LEVEL_HTTP_CONNECTION_H

#include <ma.h>
#include <maheap.h>
#include <MAUtil/Connection.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

namespace Wormhole
{

// Constants.
#define WORMHOLE_HTTP_SUCCESS 1
#define WORMHOLE_HTTP_ERROR -1

// Forward declaration.
class HighLevelHttpConnection;

/**
 * \brief Base class for helper classes that handle the download.
 * We have only one such class right now, HighLevelReaderThatReadsChunks.
 */
class HighLevelReader
{
public:
	/**
	 * Constructor.
	 */
	HighLevelReader(HighLevelHttpConnection* connection);

	/**
	 * Destructor.
	 */
	virtual ~HighLevelReader();

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
	HighLevelHttpConnection* mConnection;

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
class HighLevelReaderThatReadsChunks : public HighLevelReader
{
public:
	/**
	 * Constructor.
	 */
	HighLevelReaderThatReadsChunks(HighLevelHttpConnection* downloader);

	/**
	 * Destructor.
	 */
	virtual ~HighLevelReaderThatReadsChunks();

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
 * An HTTP connection object that is more high-level compared
 * to MAUTIL::HttpConnection. It has an integrated listener.
 * This class does not use the "content-length" HTTP header
 * and thus works also when this header is not set.
 *
 * Inherit this class and override dataDownloaded:
 *
 *   virtual void dataDownloaded(MAHandle data, int result);
 */
class HighLevelHttpConnection :
	public MAUtil::HttpConnection,
	public MAUtil::HttpConnectionListener
{
public:
	HighLevelHttpConnection();
	virtual ~HighLevelHttpConnection();

	/**
	 * Post an HTTP request.
	 *
	 * The HTTP header parameters must be a vector (possibly empty)
	 * of strings, ordered as key/value pairs.
	 *
	 * @param url The url of the POST request.
	 * @param httpParams HTTP header parameters.
	 * @param requestBody Data written to the request body.
	 * @param requestLength Length of the request body.
	 *
	 * @return WORMHOLE_HTTP_SUCCESS if successful, WORMHOLE_HTTP_ERROR on error.
	 */
	int postRequest(
		const char* url,
		MAUtil::Vector<MAUtil::String>& httpParams,
		const void* requestBody,
		int requestLength);

	/**
	 * This is the starting point of the JSON request.
	 * \return WORMHOLE_HTTP_SUCCESS if successful, WORMHOLE_HTTP_ERROR on error.
	 */
	int postJsonRequest(const char* url, const char* jsonData);

	/**
	 * This is the starting point of a GET request.
	 */
	int get(const char* url);

	/**
	 * Called by an HighLevelReader when download is successfully finished.
	 */
	void downloadSuccess(MAHandle handle);

	/**
	 * Called by an HighLevelReader when there is a download error.
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
	HighLevelReader* mReader;
};

} // namespace

#endif

 /*! @} */
