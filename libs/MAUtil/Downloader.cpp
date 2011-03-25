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
#include <conprint.h>
#include <maassert.h>
#include "Downloader.h"
#include "PlaceholderPool.h"


using namespace MAUtil;

// Perform cleanup on closing connection, 
// deallocate any downloaded data.
#define CLEANUP 1

// Perform no cleanup on closing connection.
// Downloaded data is now owned by the user.
#define NO_CLEANUP 0

// *************** Local functions *************** //

/**
 * Allocate a handle from the placeholder pool.
 */
static MAHandle allocateHandle();

/**
 * Return handle to placehoder pool, which deallocates it
 * (PlaceholderPool will call maDestroyObject).
 */
static void deallocateHandle(MAHandle handle);

static MAHandle allocateHandle()
{
	return PlaceholderPool::alloc();
}

static void deallocateHandle(MAHandle handle)
{
	PlaceholderPool::put(handle);
}

// *************** Class DownloadListener *************** //

void DownloadListener::notifyProgress(Downloader* dl, int downloadedBytes, int totalBytes)
{
}

bool DownloadListener::outOfMemory(Downloader*)
{
	return false;
}

// *************** Class Downloader *************** //

Downloader::Downloader()
: mIsDownloading(false),
  mIsDataPlaceholderSystemAllocated(false),
  mDataPlaceholder(NULL),
  mReader(NULL)
{
	mConn = new HttpConnection(this);
}

Downloader::~Downloader()
{
	closeConnection(CLEANUP);
	delete mConn;
}

void Downloader::addDownloadListener(DownloadListener* listener)
{
	mDownloadListeners.add(listener);
}

void Downloader::removeDownloadListener(DownloadListener* listener)
{
	for (int i = 0; i < mDownloadListeners.size(); ++i) 
	{
		if (mDownloadListeners[i] == listener) 
		{
			mDownloadListeners.remove(i);
			return;
		}
	}
}

int Downloader::beginDownloading(const char *url, MAHandle placeholder)
{
	ASSERT_MSG(!mIsDownloading, "Download already in progress");

	// Make sure connection is closed.
	closeConnection(NO_CLEANUP);
	
	// Create new connection.
	int result = mConn->create(url, HTTP_GET);
	if (result <= 0)
	{
		return result;
	}

	mIsDownloading = true;

	// If the supplied placeholder is zero, we will use a 
	// system allocated placeholder.
	mIsDataPlaceholderSystemAllocated = (0 == placeholder);
	
	// Allocate placeholder if no one was supplied.
	mDataPlaceholder = placeholder ? placeholder : allocateHandle();

	mConn->finish();

	return 1;
}

void Downloader::cancelDownloading()
{
	ASSERT_MSG(mIsDownloading, "Inactive download cancelled.");
	
	closeConnection(CLEANUP);
	
	// Notify listeners
	for (int i = 0; i < mDownloadListeners.size(); ++i)
	{
		mDownloadListeners[i]->downloadCancelled(this);
	}
}

void Downloader::fireFinishedDownloading(MAHandle handle)
{
	closeConnection(NO_CLEANUP);
	
	// Broadcast finishedDownloading to listeners.
	for (int i = 0; i < mDownloadListeners.size(); ++i)
	{
		mDownloadListeners[i]->finishedDownloading(this, handle);
	}
}

void Downloader::fireError(int code)
{
	mIsDownloading = false;

	// Broadcast error to listeners.
	for (int i = 0; i < mDownloadListeners.size(); ++i)
	{
		mDownloadListeners[i]->error(this, code);
	}
	
	if(!mIsDownloading)
		closeConnection(CLEANUP);
}

void Downloader::closeConnection(int cleanup)
{
	mConn->close();
	
	deleteReader();
	
	// We only cleanup if there is an ongoing download.
	if (cleanup 
		&& mIsDownloading 
		&& mDataPlaceholder 
		&& mIsDataPlaceholderSystemAllocated)
	{
		// Return system allocated placeholder.
		deallocateHandle(mDataPlaceholder);
		mDataPlaceholder = NULL;
	}
	
	mIsDownloading = false;
}

void Downloader::deleteReader()
{
	if (mReader)
	{
		delete mReader;
		mReader = NULL;
	}
}

/**
 * Called from the HttpConnection via HttpConnectionListener interface.
 */
void Downloader::httpFinished(HttpConnection* http, int result)
{
	// If there is an error, then broadcast the error and return.
	if (result < 0 || !(result >= 200 && result < 300))
	{
		fireError(result);
		return;
	}

	// Check if we have a content-length header.
	String str;
	int res = http->getResponseHeader("content-length", &str);
	if (res >= 0)
	{
		// *** Content length is known *** //

		// Get the content length.
		int contentLength = stringToInteger(str);

		// Allocate space for data to be received.
		int errorCode = maCreateData(mDataPlaceholder, contentLength);
		if (RES_OUT_OF_MEMORY == errorCode)
		{
			fireError(CONNERR_DOWNLOADER_OOM);
			return;
		}

		// Delete old reader and create new one and start receiving
		// data via the reader.
		deleteReader();
		mReader = new DownloaderReaderWithKnownContentLength(
			this,
			0, // data offset
			contentLength);
		mReader->startRecvToData(http);
	}
	else
	{
		// *** Content length is NOT known *** //

		// Delete old reader and create new one and start receiving
		// data via the reader.
		deleteReader();
		mReader = new DownloaderReaderThatReadsChunks(this);
		mReader->startRecvToData(http);
	}
}

void Downloader::fireNotifyProgress(int dataOffset, int contentLength)
{
	for (int i = 0; i < mDownloadListeners.size(); ++i)
	{
		mDownloadListeners[i]->notifyProgress(this, dataOffset, contentLength);
	}
}

/**
 * Delegate handling to reader.
 */
void Downloader::connRecvFinished(Connection* conn, int result)
{
	ASSERT_MSG(mReader, "Downloader has no reader.");

	mReader->connRecvFinished(conn, result);
}

MAHandle Downloader::getHandle()
{
	return mDataPlaceholder;
}

MAHandle Downloader::getDataPlaceholder()
{
	return mDataPlaceholder;
}

// *************** Class ImageDownloader *************** //

ImageDownloader::ImageDownloader() :
	mIsImagePlaceholderSystemAllocated(false),
	mIsImageCreated(false),
	mImagePlaceholder(NULL)
{
}
		
ImageDownloader::~ImageDownloader()
{
}

MAHandle ImageDownloader::getHandle()
{
	// If the image is already created, return its placeholder.
	if (mIsImageCreated)
	{
		return mImagePlaceholder;
	}
	
	// If the image is not created, we do so now.
	int res = maCreateImageFromData(
		mImagePlaceholder, 
		mDataPlaceholder, 
		0, 
		mReader->getContentLength());
	
	if (RES_OUT_OF_MEMORY == res)
	{
		fireError(CONNERR_DOWNLOADER_OOM);
		return 0;
	}
	
	// Now that the image is created, we can deallocate the data handle.
	if (mIsDataPlaceholderSystemAllocated)
	{
		// Return system allocated data placeholder to pool.
		deallocateHandle(mDataPlaceholder);
		mDataPlaceholder = NULL;
	}
	
	// Image is created.
	mIsImageCreated = true;
	
	return mImagePlaceholder;
}

int ImageDownloader::beginDownloading(const char *url, MAHandle placeholder)
{
	mIsImageCreated = false;

	// If the supplied placeholder is zero, we will use a 
	// system allocated placeholder for the image.
	mIsImagePlaceholderSystemAllocated = (0 == placeholder);
	
	// Allocate placeholder for image if no one was supplied.
	mImagePlaceholder = placeholder ? placeholder : allocateHandle();
	
	return Downloader::beginDownloading(url);
}

void ImageDownloader::closeConnection(int cleanup)
{
	bool downloading = mIsDownloading;
	
	Downloader::closeConnection(cleanup);
	
	// We only cleanup if there is an ongoing download.
	if (cleanup 
		&& downloading 
		&& mImagePlaceholder 
		&& mIsImagePlaceholderSystemAllocated)
	{
		// Return system allocated placeholder to pool.
		deallocateHandle(mImagePlaceholder);
		mImagePlaceholder = NULL;
	}
	
	mIsImageCreated = false;
}

// *************** Class AudioDownloader *************** //

int AudioDownloader::beginDownloading(
	const char *url, 
	MAHandle placeholder,
	const char *mimeType, 
	bool forceMime)
{
	mForceMimeType = forceMime;
	
	if (mimeType != NULL) 
	{
		mMimeType = mimeType;
	}

	if (mForceMimeType && !mimeType)
	{
		return CONNERR_NOHEADER;
	}
	
	return Downloader::beginDownloading(url, placeholder);
}

int AudioDownloader::beginDownloading(const char *url, MAHandle placeholder)
{
	return beginDownloading(url, placeholder, NULL, false);
}

/**
 * Note that when using AudioDownloader we must have a content-length header.
 */
void AudioDownloader::httpFinished(HttpConnection* http, int result)
{
	// Do we have an error?
	if (result < 0 || !(result >= 200 && result < 300)) 
	{
		fireError(result);
		return;
	}
	
	// Do we have a content-length header?
	String str;
	int res = http->getResponseHeader("content-length", &str);
	if(res < 0) 
	{
		fireError(res);
		return;
	}

	if (!mForceMimeType)
	{
		// Update mime type to the type send from the server.
		http->getResponseHeader("content-type", &mMimeType);
	}
	
	// Do a sanity check on the mime type string.
	if (mMimeType.length() <= 3) // min length for any mime type string (x/y)
	{
		fireError(CONNERR_NOHEADER);
		return;
	}

	// Allocate data object with space for mime type plus content.
	int contentLength = stringToInteger(str);
	contentLength += mMimeType.length() + 1;
	if (maCreateData(mDataPlaceholder, contentLength) == RES_OUT_OF_MEMORY)
	{
		fireError(CONNERR_DOWNLOADER_OOM);
		return;
	}
	
	// Write mime type string into data.
	maWriteData(mDataPlaceholder, mMimeType.c_str(), 0, mMimeType.length() + 1);
	int dataOffset = mMimeType.length() + 1;

	// Delete old reader and create new one and start receiving data.
	deleteReader();
	mReader = new DownloaderReaderWithKnownContentLength(
		this,
		dataOffset,
		contentLength);
	mReader->startRecvToData(http);
}


// *************** Class DownloaderReader *************** //

DownloaderReader::DownloaderReader(Downloader* downloader)
: mDownloader(downloader),
  mContentLength(0)
{
}

int DownloaderReader::getContentLength()
{
	return mContentLength;
}

// *************** Class DownloaderReaderWithKnownContentLength *************** //

/**
 * Constructor for reader that knows the length of the content to be downloaded.
 */
DownloaderReaderWithKnownContentLength::DownloaderReaderWithKnownContentLength(
	Downloader* downloader,
	int dataOffset,
	int contentLength)
: DownloaderReader(downloader),
  mDataOffset(dataOffset)
{
	mContentLength = contentLength;
}

/**
 * Tell connection to receive data.
 */
void DownloaderReaderWithKnownContentLength::startRecvToData(Connection* conn)
{
	// Receive data that is left (we might not get all of what we ask for).
	conn->recvToData(
			mDownloader->getDataPlaceholder(),
			mDataOffset,
			mContentLength - mDataOffset);
	//http->recvToData(mDataPlaceholder, mDataOffset, mContentLength - mDataOffset);

	// Broadcast progress to listeners.
	mDownloader->fireNotifyProgress(mDataOffset, mContentLength);
}

void DownloaderReaderWithKnownContentLength::connRecvFinished(
	Connection* conn,
	int result)
{
	// Have we got an error?
	if (result <= 0)
	{
		mDownloader->fireError(result);
		return;
	}
	// Update number of bytes read with the result.
	mDataOffset += result;

	// Broadcast progress status to listeners.
	mDownloader->fireNotifyProgress(mDataOffset, mContentLength);

	// Have we got all data?
	if ((mContentLength - mDataOffset) > 0)
	{
		// No we hav not, continue to read data.
		conn->recvToData(
			mDownloader->getDataPlaceholder(),
			mDataOffset,
			mContentLength - mDataOffset);
	}
	else
	{
		// We have got all data, finish download.
		MAHandle handle = mDownloader->getHandle();
		ASSERT_MSG(handle, "No data handle in standard reader");
		mDownloader->fireFinishedDownloading(handle);
	}
}

// *************** Class DownloaderReaderThatReadsChunks *************** //

DownloaderReaderThatReadsChunks::DownloaderReaderThatReadsChunks(Downloader* downloader)
: DownloaderReader(downloader),
  mDataChunkSize(2048),
  mDataChunkOffset(0)
{
}

DownloaderReaderThatReadsChunks::~DownloaderReaderThatReadsChunks()
{
	while (0 < mDataChunks.size())
	{
		// Remove first remaining chunk.
		MAHandle chunk = mDataChunks[0];

		// Return chunk to pool.
		deallocateHandle(chunk);

		// Remove chunk from list.
		mDataChunks.remove(0);
	}
}

void DownloaderReaderThatReadsChunks::startRecvToData(Connection* conn)
{
	// Content length is unknown, read data in chunks until we get CONNERR_CLOSED.
	bool success = readNextChunk(conn);
	if (!success)
	{
		mDownloader->fireError(CONNERR_DOWNLOADER_OOM);
	}
}

void DownloaderReaderThatReadsChunks::connRecvFinished(Connection* conn, int result)
{
	// Have we completed reading data?
	if (CONNERR_CLOSED == result)
	{
		finishedDownloadingChunkedData();
		return;
	}

	// Have we got an error?
	if (result <= 0)
	{
		mDownloader->fireError(result);
		return;
	}

	// We have new data.
	mDataChunkOffset += result;
	mContentLength += result;
	int leftToRead = mDataChunkSize - mDataChunkOffset;

	// Broadcast progress status to listeners.
	// Here we just send in the accumulated content length as
	// the current value. Zero means "unknown content length".
	mDownloader->fireNotifyProgress(mContentLength, 0);

	if (leftToRead > 0)
	{
		// Read more data into current chunk.
		int currentChunkIndex = mDataChunks.size() - 1;
		MAHandle chunk = mDataChunks[currentChunkIndex];
		conn->recvToData(chunk, mDataChunkOffset, leftToRead);
	}
	else
	{
		// Read next chunk.
		bool success = readNextChunk(conn);
		if (!success)
		{
			mDownloader->fireError(CONNERR_DOWNLOADER_OOM);
		}
	}
}

bool DownloaderReaderThatReadsChunks::readNextChunk(Connection* conn)
{
	// Allocate new a chunk of data.
	MAHandle chunk = allocateHandle();
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
		conn->recvToData(chunk, mDataChunkOffset, mDataChunkSize);
		return true;
	}
}

void DownloaderReaderThatReadsChunks::finishedDownloadingChunkedData()
{
	// Allocate big handle and copy the chunks to it.
	// mContentLength holds the accumulated size of read data.
	int errorCode = maCreateData(
		mDownloader->getDataPlaceholder(),
		mContentLength);

	if (RES_OUT_OF_MEMORY == errorCode)
	{
		mDownloader->fireError(CONNERR_DOWNLOADER_OOM);
		return;
	}

	// Copy the chunks to the data object.	
	int offset = 0;
	char *buf = new char[mDataChunkSize];
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
		maWriteData(mDownloader->getDataPlaceholder(), buf, offset, size);		

		// Return chunk to pool.
		deallocateHandle(chunk);

		// Remove chunk from list.
		mDataChunks.remove(0);

		// Increment offset.
		offset += mDataChunkSize;
	}
	delete[] buf;

	MAHandle handle = mDownloader->getHandle();
	ASSERT_MSG(handle, "No data handle in chunked reader");
	mDownloader->fireFinishedDownloading(handle);
}
