/* Copyright (C) 2011 MoSync AB

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

/*
 * UploadRequest.cpp
 *
 *  Created on: Aug 12, 2011
 *      Author: gabi
 */


#include "UploadRequest.h"


UploadRequest::UploadRequest(const String& path,
	String& accessToken,
	const int *fileRawData,
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

UploadRequest::UploadRequest(const String& path,
	String& accessToken,
	const MAUtil::String &picture,
	const MAUtil::String &contentType,
	const MAUtil::String &fileName,
	const MAUtil::String &message):
	FacebookRequest(PUBLISH, STRING, path, accessToken, HTTP_POST)
{
	//mFileRawData = fileRawData;
	//mFileSize = fileSize;
	mContentType = contentType;
	mFileName = fileName;
	mPicture = picture;

	if(message.size()>0)
	{
		this->mMessageBodyParams["message"] = message;
	}

	this->mRequestHeaders["Host"] = "graph.facebook.com";
	this->mRequestHeaders["User-Agent"] = "MOSYNC_SDK / Not a web browser";
}


const int *UploadRequest::getFileRawData() const
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
