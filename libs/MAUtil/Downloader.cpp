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

#include "Downloader.h"
#include "PlaceholderPool.h"
#include <conprint.h>
#include <maassert.h>

using namespace MAUtil;

void DownloadListener::notifyProgress(Downloader* dl, int downloadedBytes, int totalBytes)
{
}

bool DownloadListener::outOfMemory(Downloader*)
{
	return false;
}

Downloader::Downloader()
  : mIsDownloading(false), mData(0), mDataOffset(0), mContentLength(0)
{
	mConn = new HttpConnection(this);
}

Downloader::~Downloader()
{
	delete mConn;
	if(mIsDownloading && !mUserData)
		PlaceholderPool::put(mData);
}

void Downloader::addDownloadListener(DownloadListener *dl)
{
	mDownloadListeners.add(dl);
}

void Downloader::removeDownloadListener(DownloadListener *dl)
{
	for(int i = 0; i < mDownloadListeners.size(); i++) {
		if(mDownloadListeners[i] == dl) {
			mDownloadListeners.remove(i);
			return;
		}
	}
	PANIC_MESSAGE("listener wasn't in here");
}

int Downloader::beginDownloading(const char *url, MAHandle placeholder)
{
	ASSERT_MSG(!mIsDownloading, "already downloading");

	mConn->close();
	int result = mConn->create(url, HTTP_GET);
	if(result<=0)
	{
		return result;
	}

	mIsDownloading = true;

	mData = placeholder ? placeholder : PlaceholderPool::alloc();

	mConn->finish();

	return 1;
}

void Downloader::cancelDownloading() {
	if(mIsDownloading) {
		mIsDownloading = false;
		for(int i = 0; i < mDownloadListeners.size(); i++)
			mDownloadListeners[i]->downloadCancelled(this);
		if(!mIsDownloading) {
			mConn->close();
		}
		if(!mUserData)
			PlaceholderPool::put(mData);
	}
	else {
		PANIC_MESSAGE("Inactive download cancelled.");
	}
}

void Downloader::fireFinishedDownloading(MAHandle handle)
{
	mIsDownloading = false;
	for(int i = 0; i < mDownloadListeners.size(); i++)
		mDownloadListeners[i]->finishedDownloading(this, handle);
	if(!mIsDownloading) {
		mConn->close();
	}
}

void Downloader::fireError(int code)
{
	mIsDownloading = false;
	for(int i = 0; i < mDownloadListeners.size(); i++)
		mDownloadListeners[i]->error(this, code);
	if(!mIsDownloading) {
		mConn->close();
	}
}

void Downloader::connRecvFinished(Connection* conn, int result)
{
	if(result>=0)
	{
		//lprintfln("dataOffset: %d\n", dataOffset);
		mDataOffset += result;
		//lprintfln("diff: %d\n", contentLength-dataOffset);

		for(int i = 0; i < mDownloadListeners.size(); i++)
			mDownloadListeners[i]->notifyProgress(this, mDataOffset, mContentLength);

		if((mContentLength - mDataOffset)>0)
		{
			conn->recvToData(mData, mDataOffset, mContentLength - mDataOffset);
		}
		else
		{
			MAHandle h = getHandle();
			if(h)
				fireFinishedDownloading(h);
		}
	}
	else
	{
		fireError(result);
	}
}

MAHandle Downloader::getHandle() {
	return mData;
}

void Downloader::httpFinished(HttpConnection* http, int result)
{
	if(result < 0 || !(result >= 200 && result < 300)) {
		fireError(result);
		return;
	}
	String str;
	int res = http->getResponseHeader("content-length", &str);
	if(res < 0) {
		fireError(res);
		return;
	}
	mContentLength = stringToInteger(str);
	//lprintfln("contentLength: %d\n", contentLength);
	if(maCreateData(mData, mContentLength) == RES_OUT_OF_MEMORY) {
		fireError(CONNERR_DOWNLOADER_OOM);
		return;
	}
	mDataOffset = 0;

	http->recvToData(mData, mDataOffset, mContentLength - mDataOffset);

	for(int i = 0; i < mDownloadListeners.size(); i++)
		mDownloadListeners[i]->notifyProgress(this, mDataOffset, mContentLength);
}

MAHandle ImageDownloader::getHandle() {
	int res = maCreateImageFromData(mPlaceholder, mData, 0, mContentLength);
	if(!mUserData)
		PlaceholderPool::put(mData);
	if(res == RES_OUT_OF_MEMORY) {
		fireError(CONNERR_DOWNLOADER_OOM);
		return 0;
	}
	return mPlaceholder;
}

ImageDownloader::~ImageDownloader() {
	if(mIsDownloading && !mUserPlaceholder) {
		mConn->close();	//gotta do this first in case placeholder is in flux.
		PlaceholderPool::put(mPlaceholder);
	}
}

void ImageDownloader::cancelDownloading() {
	Downloader::cancelDownloading();
	if(!mUserPlaceholder)
		PlaceholderPool::put(mPlaceholder);
}

int ImageDownloader::beginDownloading(const char *url, MAHandle placeholder) {
	mUserPlaceholder = placeholder != 0;
	mPlaceholder = mUserPlaceholder ? placeholder : PlaceholderPool::alloc();
	return Downloader::beginDownloading(url);
}	

int AudioDownloader::beginDownloading(const char *url, MAHandle placeholder,
	const char *mimeType, bool forceMyMime)
{
	mForce = forceMyMime;
	if(mimeType != NULL) {
		mMimeType = mimeType;
	}
	if(mForce && !mimeType)
		return CONNERR_NOHEADER;
	return Downloader::beginDownloading(url, placeholder);
}

int AudioDownloader::beginDownloading(const char *url, MAHandle placeholder) {
	return beginDownloading(url, placeholder, NULL, false);
}

void AudioDownloader::httpFinished(HttpConnection* http, int result) {
	if(result < 0 || !(result >= 200 && result < 300)) {
		fireError(result);
		return;
	}
	String str;
	int res = http->getResponseHeader("content-length", &str);
	if(res < 0) {
		fireError(res);
		return;
	}

	if(!mForce)
		http->getResponseHeader("content-type", &mMimeType);
	if(mMimeType.length() <= 3) {	//min length for any mime type (x/y)
		fireError(CONNERR_NOHEADER);
		return;
	}

	mContentLength = stringToInteger(str);
	//lprintfln("contentLength: %d\n", contentLength);
	mContentLength += mMimeType.length() + 1;
	if(maCreateData(mData, mContentLength) == RES_OUT_OF_MEMORY) {
		fireError(CONNERR_DOWNLOADER_OOM);
		return;
	}
	maWriteData(mData, mMimeType.c_str(), 0, mMimeType.length() + 1);
	mDataOffset = mMimeType.length() + 1;

	http->recvToData(mData, mDataOffset, mContentLength - mDataOffset);

	for(int i = 0; i < mDownloadListeners.size(); i++)
		mDownloadListeners[i]->notifyProgress(this, mDataOffset, mContentLength);
}
