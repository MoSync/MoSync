/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "BuffDownloader.h"
#include <conprint.h>
#include <maassert.h>

using namespace MAUtil;

void BuffDownloadListener::notifyProgress(BuffDownloader* bd, int downloadedBytes, int totalBytes)
{
}

BuffDownloader::BuffDownloader()
  : mDownloading(false), mData(0), mDataOffset(0), mContentLength(0)
{
	mConn = new HttpConnection(this);
}

BuffDownloader::~BuffDownloader()
{
	delete mConn;
	delete mData;
}

void BuffDownloader::addBuffDownloadListener(BuffDownloadListener *bdl)
{
	mBuffDownloadListeners.add(bdl);
}

int BuffDownloader::beginDownloading(const char *url)
{
	ASSERT_MSG(!mDownloading, "Already downloading.");

	mConn->close();
	int result = mConn->create(url, HTTP_GET);
	if(result<=0)
	{
		return result;
	}

	mDownloading = true;

	mConn->finish();

	return 1;
}

void BuffDownloader::cancelDownloading() {
	if(mDownloading) {
		mDownloading = false;
		for(int i = 0; i < mBuffDownloadListeners.size(); i++)
			mBuffDownloadListeners[i]->downloadCancelled(this);
		if(!mDownloading) {
			mConn->close();
		}
	}
	else {
		PANIC_MESSAGE("Inactive download cancelled.");
	}
}

void BuffDownloader::fireFinishedDownloading(char* data)
{
	mDownloading = false;
	for(int i = 0; i < mBuffDownloadListeners.size(); i++)
		mBuffDownloadListeners[i]->finishedDownloading(this, data);
	if(!mDownloading) {
		mConn->close();
	}
}

void BuffDownloader::fireError(int code)
{
	mDownloading = false;
	for(int i = 0; i < mBuffDownloadListeners.size(); i++)
		mBuffDownloadListeners[i]->error(this, code);
	if(!mDownloading) {
		mConn->close();
	}
}

void BuffDownloader::connRecvFinished(Connection*, int result)
{
	if(result>=0)
	{
		mDataOffset+=result;

		for(int i = 0; i < mBuffDownloadListeners.size(); i++)
			mBuffDownloadListeners[i]->notifyProgress(this, mDataOffset, mContentLength);

		if((mContentLength-mDataOffset)>0)
		{
			mConn->recv(mData, mContentLength-mDataOffset);
		}
		else
		{
			fireFinishedDownloading(getData());
		}
	}
	else
	{
		fireError(result);
	}
}


char* BuffDownloader::getData() {
	return mData;
}

void BuffDownloader::httpFinished(HttpConnection* http, int result)
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
	if(mData) {
		delete mData;
	}
	mData = new char[mContentLength];
	mData[mContentLength] = '\0';
	mDataOffset = 0;

	http->recv(mData, mContentLength-mDataOffset);

	for(int i = 0; i < mBuffDownloadListeners.size(); i++)
		mBuffDownloadListeners[i]->notifyProgress(this, mDataOffset, mContentLength);
}

