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

