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

/*
 * UploadRequest.cpp
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
