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

/**
 * @file HighLevelHttpConnection.cpp
 * @author Mikael Kindborg
 *
 * @brief A high-level HTTP connection object.
 */

#include <mastdlib.h>
#include <mastring.h>
#include <mavsprintf.h>

#include "HighLevelHttpConnection.h"

using namespace MAUtil;

namespace Wormhole
{

// *************** Local functions *************** //

/**
 * Allocate a handle from the placeholder pool.
 */
static MAHandle AllocateHandle();

/**
 * Return handle to placehoder pool, which deallocates it
 * (PlaceholderPool will call maDestroyObject).
 */
static void DeallocateHandle(MAHandle handle);

static MAHandle AllocateHandle()
{
	return maCreatePlaceholder();
}

static void DeallocateHandle(MAHandle handle)
{
	maDestroyPlaceholder(handle);
}

// *************** Class HighLevelHttpConnection *************** //

HighLevelHttpConnection::HighLevelHttpConnection() :
	HttpConnection(this),
	mReader(NULL)
{
}

HighLevelHttpConnection::~HighLevelHttpConnection()
{
	deallocateData();
}

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
int HighLevelHttpConnection::postRequest(
	const char* url,
	MAUtil::Vector<MAUtil::String>& httpParams,
	const void* requestBody,
	int requestLength)
{
	// Create request.
	int result = create(url, HTTP_POST);
	if (result < 0)
	{
		return WORMHOLE_HTTP_ERROR;
	}

	// Write headers.
	for (int i = 0; i < httpParams.size(); i = i + 2)
	{
		setRequestHeader(
			httpParams[i].c_str(),
			httpParams[i + 1].c_str());
	}

	// Write request data.
	write(requestBody, requestLength);

	// Next that happens is that connWriteFinished is called.

	return WORMHOLE_HTTP_SUCCESS;
}

/**
 * This is the starting point of a JSON request.
 * \return WORMHOLE_HTTP_SUCCESS if successful, WORMHOLE_HTTP_ERROR on error.
 */
int HighLevelHttpConnection::postJsonRequest(const char* url, const char* jsonData)
{
	int result = create(url, HTTP_POST);
	if (result < 0)
	{
		return WORMHOLE_HTTP_ERROR;
	}

	// Get length of content (request body).
	char contentLength[16];
	sprintf(contentLength, "%i", (int) strlen(jsonData));

	// Set request headers.
	setRequestHeader("Content-type", "application/json");
	setRequestHeader("Charset", "UTF-8");
	setRequestHeader("Content-Length", contentLength);

	// Write request data.
	write(jsonData, strlen(jsonData));

	// Next that happens is that connWriteFinished is called.

	return WORMHOLE_HTTP_SUCCESS;
}

/**
 * This is the starting point of a GET request.
 */
int HighLevelHttpConnection::get(const char* url)
{
	int result = create(url, HTTP_GET);
	if (result > 0)
	{
		finish();

		// Next this that happens is that httpFinished is called.
	}

	return result;
}

void HighLevelHttpConnection::connWriteFinished(
	MAUtil::Connection* connection,
	int result)
{
	// Call finish to execute the post request if
	// write was successful.
	if (result > 0)
	{
		finish();

		// Next that happens is that httpFinished is called.
	}
	else
	{
		// Inform about the error.
		dataDownloaded(0, result);
	}
}

/**
 * This method is called when the HTTP request is complete.
 * Now all data is sent to the server and we can start reading
 * the reply and download data.
 */
void HighLevelHttpConnection::httpFinished(
	MAUtil::HttpConnection* connection,
	int result)
{
	// Known success status codes are 200 and 201.
	if (200 != result && 201 != result)
	{
		// There was an error.
		dataDownloaded(0, result);
		return;
	}

	// Start to read the result using a DownloadReader helper object.
	deleteReader();
	mReader = new HighLevelReaderThatReadsChunks(this);
	mReader->startRecvToData();

	// Next this that happens is that connReadFinished is called.
}

void HighLevelHttpConnection::connRecvFinished(
	MAUtil::Connection* connection,
	int result)
{
	// Delegate to reader.
	mReader->connRecvFinished(result);
}

void HighLevelHttpConnection::connReadFinished(
	MAUtil::Connection* connection,
	int result)
{
	// TODO: Document why this should not happen.
	maPanic(
		1,
		"HighLevelHttpConnection::connReadFinished: "
		"This was not supposed to happen.");
}

/**
 * Called by an HighLevelReader when there is a download error.
 */
void HighLevelHttpConnection::downloadError(int result)
{
	close();
	deleteReader();
	deallocateData();
	dataDownloaded(0, result);
}

/**
 * Called by an HighLevelReader when download is successfully finished.
 */
void HighLevelHttpConnection::downloadSuccess(MAHandle handle)
{
	close();
	deleteReader();
	deallocateData();
	dataDownloaded(handle, RES_OK);
}

void HighLevelHttpConnection::deallocateData()
{
}

void HighLevelHttpConnection::deleteReader()
{
	if (mReader)
	{
		delete mReader;
		mReader = NULL;
	}
}

// *************** Class HighLevelReader *************** //

/**
 * Constructor.
 */
HighLevelReader::HighLevelReader(HighLevelHttpConnection* connection)
: mConnection(connection),
  mContentLength(0)
{
}

/**
 * Destructor.
 */
HighLevelReader::~HighLevelReader()
{
}

int HighLevelReader::getContentLength()
{
	return mContentLength;
}

// *************** Class HighLevelReaderThatReadsChunks *************** //

/**
 * Constructor.
 */
HighLevelReaderThatReadsChunks::HighLevelReaderThatReadsChunks(
		HighLevelHttpConnection* connection)
: HighLevelReader(connection),
  mDataChunkSize(2048),
  mDataChunkOffset(0)
{
}

/**
 * Destructor.
 */
HighLevelReaderThatReadsChunks::~HighLevelReaderThatReadsChunks()
{
	// Deallocate chunks.
	while (0 < mDataChunks.size())
	{
		// Remove first remaining chunk.
		MAHandle chunk = mDataChunks[0];

		// Return chunk to pool.
		DeallocateHandle(chunk);

		// Remove chunk from list.
		mDataChunks.remove(0);
	}
}

/**
 * Start downloading data.
 */
void HighLevelReaderThatReadsChunks::startRecvToData()
{
	// Content length is unknown, read data in chunks until we get
	// CONNERR_CLOSED.
	bool success = readNextChunk();
	if (!success)
	{
		mConnection->downloadError(RES_OUT_OF_MEMORY);
	}
}

/**
 * Called when the new data is available.
 */
void HighLevelReaderThatReadsChunks::connRecvFinished(int result)
{
	// If the connection is closed we have completed reading the data.
	if (CONNERR_CLOSED == result)
	{
		finishedDownloadingChunkedData();
		return;
	}

	// Have we got an error?
	if (result <= 0)
	{
		mConnection->downloadError(result);
		return;
	}

	// We have new data.
	mDataChunkOffset += result;
	mContentLength += result;
	int leftToRead = mDataChunkSize - mDataChunkOffset;

	if (leftToRead > 0)
	{
		// Read more data into current chunk.
		int currentChunkIndex = mDataChunks.size() - 1;
		MAHandle chunk = mDataChunks[currentChunkIndex];
		mConnection->recvToData(chunk, mDataChunkOffset, leftToRead);
	}
	else
	{
		// Read next chunk.
		bool success = readNextChunk();
		if (!success)
		{
			mConnection->downloadError(RES_OUT_OF_MEMORY);
		}
	}
}

bool HighLevelReaderThatReadsChunks::readNextChunk()
{
	// Allocate new a chunk of data.
	MAHandle chunk = AllocateHandle();
	int result = maCreateData(chunk, mDataChunkSize);
	if (RES_OUT_OF_MEMORY == result)
	{
		return false;
	}
	else
	{
		// Start reading into the new chunk.
		mDataChunks.add(chunk);
		mDataChunkOffset = 0;
		mConnection->recvToData(chunk, mDataChunkOffset, mDataChunkSize);
		return true;
	}
}

void HighLevelReaderThatReadsChunks::finishedDownloadingChunkedData()
{
	// Allocate big handle and copy the chunks to it.
	// mContentLength holds the accumulated size of read data.
	// We create a new placeholder here, not using the pool.
	// TODO: Consider using PlaceholderPool also for this handle.
	MAHandle dataHandle = maCreatePlaceholder();
	int errorCode = maCreateData(dataHandle, mContentLength);
	if (RES_OUT_OF_MEMORY == errorCode)
	{
		mConnection->downloadError(RES_OUT_OF_MEMORY);
		return;
	}

	// Copy chunks to the data object.
	int offset = 0;
	char* buf = new char[mDataChunkSize];
	while (0 < mDataChunks.size())
	{
		// Last chunk should only be partially written.
		int dataLeftToWrite = mContentLength - offset;

		// Set size to min(dataLeftToWrite, mDataChunkSize)
		int size = (dataLeftToWrite < mDataChunkSize
			? dataLeftToWrite : mDataChunkSize);

		// Copy first remaining chunk.
		MAHandle chunk = mDataChunks[0];
		maReadData(chunk, buf, 0, size);
		maWriteData(dataHandle, buf, offset, size);

		// Return chunk to pool.
		DeallocateHandle(chunk);

		// Remove chunk from list.
		mDataChunks.remove(0);

		// Increment offset.
		offset += mDataChunkSize;
	}
	delete[] buf;

	// Download is finished! Tell the connection about this.
	mConnection->downloadSuccess(dataHandle);
}

} // namespace
