/*
 * UploadRequest.cpp
 *
 *  Created on: Aug 12, 2011
 *      Author: gabi
 */


#include "UploadRequest.h"


UploadRequest::UploadRequest(	const String& path,
								String& accessToken,
								const byte *fileRawData,
								int fileSize,
								const MAUtil::String &contentType,
								const MAUtil::String &fileName,
								const MAUtil::String &message):
								FacebookRequest(PUBLISH, STRING, path, accessToken, HTTP_POST)
{
	mFileRawData = fileRawData;
	mFileSize = fileSize;
	mContentType = contentType;
	mFileName = fileName;

	if(message.size()>0)
	{
		this->mMessageBodyParams["message"] = message;
	}

	this->mRequestHeaders["Host"] = "graph.facebook.com";
	this->mRequestHeaders["User-Agent"] = "MOSYNC_SDK / Not a web browser";
}

const byte *UploadRequest::getFileRawData() const
{
	return mFileRawData;
}

int UploadRequest::getFileRawDataSize() const
{
	return mFileSize;
}

const MAUtil::String &UploadRequest::getContentType() const
{
	return mContentType;
}

const MAUtil::String &UploadRequest::getFileName() const
{
	return mFileName;
}